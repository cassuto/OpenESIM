/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include <QtWidgets>
#include <instrument/instrumentbase.h>
#include <instrument/libentry.h>
#include <frontend/instrument-lib.h>
#include "instrumentmanagement.h"
#include "instrumentrackcellwidget.h"
#include "instrumentpickdialog.h"
#include "mainwindow.h"
#include "instrumentrackform.h"

namespace dsim
{

InstrumentRackForm::InstrumentRackForm( InstrumentManagement *rack, QWidget *parent /*= 0l*/ )
                   : QMainWindow( parent )
                   , m_rack( rack )
{
  setWindowTitle( tr("Instrument Rack View") );

  createActions();
  createToolBar();
  createRackBox();

  int index=0;
  foreach( InstrumentPair pair, rack->instruments() ) insertInstrument( pair, index++ );
}

void InstrumentRackForm::createActions()
{
  actAddInstrument = new QAction( QIcon(":/bitmaps/modeinst.png"), tr("Add Instrument"), this );
  actAddInstrument->setToolTip( tr("Add a new instrument to the rack...") );

  connect( actAddInstrument, SIGNAL(triggered()), this, SLOT(onAddInstrument()) );
}

void InstrumentRackForm::createToolBar()
{
  QToolBar *toolBar = new QToolBar( tr("File") ); // toolBar file
  toolBar->addAction( actAddInstrument );
  toolBar->setIconSize( QSize(16, 16) );
  addToolBar( toolBar );
}

void InstrumentRackForm::createRackBox()
{
  toolBox = new QToolBox( this );
  toolBox->setSizePolicy( QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored) );
  toolBox->setMinimumWidth( sizeHint().width() );
  setCentralWidget( toolBox );
}

void InstrumentRackForm::insertInstrument( const InstrumentPair &inst, int index )
{
  toolBox->addItem( new InstrumentRackCellWidget( inst, index, this ), /*text*/inst.entry->classname );
}

void InstrumentRackForm::onAddInstrument()
{
  InstrumentPickDialog *instDialog = new InstrumentPickDialog( m_rack, this );
  if( instDialog->exec() == QDialog::Accepted && !instDialog->selectedClassname().isEmpty() )
    {
      if( InstrumentLibraryEntry *entry = instrument_lib_entry( instDialog->selectedClassname().toLocal8Bit().data() ) )
        {
          InstrumentBase *instance;
          int index;
          if( MainWindow::instance()->processRc( m_rack->addInstrument( entry, &instance, &index ) ) )
            {
              insertInstrument( InstrumentPair( entry, instance ), index );
            }
        }
      else
        MainWindow::instance()->processRc( -DS_CREATE_INSTRUMENT );
    }
  instDialog->deleteLater();
}

void InstrumentRackForm::closeEvent( QCloseEvent *closeEvent )
{ closeEvent->accept(); emit closed(); }

}

#include "instrumentrackform.moc"
