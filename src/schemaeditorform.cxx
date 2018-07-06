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

#include <QAction>
#include <QToolBar>
#include <QFileDialog>

#include <fstream>
#include <dsim/error.h>
#include <device/libentry.h>

#include "domdataset.h"
#include "schemasheet.h"
#include "mainwindow.h"
#include "componentpickwidget.h"
#include "schemawidget.h"
#include "schemaeditorform.h"

namespace dsim
{

SchemaEditorForm::SchemaEditorForm( QWidget *parent_ )
                : QMainWindow( parent_ )
                , dom( new DomDataset( MainWindow::instance()->dompool() ) )
                , schsheet( new SchemaSheet() )
{
  dom->ref();

  createActions();
  createToolbars();
  createWidgets();
}

SchemaEditorForm::~SchemaEditorForm()
{
  dom->release();
  delete schsheet;
}

void SchemaEditorForm::createActions()
{
  /*
   * Mode actions
   */
  modeSelection = new QAction(QIcon((":/arts/modeselection.png")), tr("Selection mode"), this);
  modeSelection->setStatusTip(tr("Enter the selection drawing mode."));
  modeDrawComponent = new QAction(QIcon((":/arts/modecomponent.png")), tr("Drawing the component mode"), this);
  modeDrawComponent->setStatusTip(tr("Enter the drawing the component mode."));
  modeDrawJunctionDot = new QAction(QIcon((":/arts/modejunctiondot.png")), tr("Drawing the junction dot mode"), this);
  modeDrawJunctionDot->setStatusTip(tr("Enter the drawing the junction dot mode mode."));
  modeDrawLable = new QAction(QIcon((":/arts/modelabel.png")), tr("Drawing the label mode"), this);
  modeDrawLable->setStatusTip(tr("Enter the drawing the label mode."));
  modeDrawScript = new QAction(QIcon((":/arts/modescript.png")), tr("Drawing the script mode"), this);
  modeDrawScript->setStatusTip(tr("Enter the drawing the script mode mode."));
  modeDrawBus = new QAction(QIcon((":/arts/modebus.png")), tr("Drawing the bus mode"), this);
  modeDrawBus->setStatusTip(tr("Enter the drawing the bus mode mode."));
  modeDrawSubCircuit = new QAction(QIcon((":/arts/modesubcircuit.png")), tr("Drawing the sub circuit mode"), this);
  modeDrawSubCircuit->setStatusTip(tr("Enter the drawing the sub circuit mode mode."));
}

void SchemaEditorForm::createToolbars()
{
  /*
   * ToolBar drawing mode.
   */
  modeToolBar = new QToolBar( tr("Mode") );
  modeToolBar->addAction( modeSelection );
  modeToolBar->addAction( modeDrawComponent );
  modeToolBar->addAction( modeDrawJunctionDot );
  modeToolBar->addAction( modeDrawLable );
  modeToolBar->addAction( modeDrawScript );
  modeToolBar->addAction( modeDrawBus );
  modeToolBar->addAction( modeDrawSubCircuit );
  modeToolBar->addSeparator();
  modeToolBar->setIconSize( QSize(16, 16) );
  this->addToolBar( Qt::LeftToolBarArea, modeToolBar );
}

void SchemaEditorForm::createWidgets()
{
  /*
   * Device Pick Dock
   */
  componentPick = new ComponentPickWidget( this );
  componentPick->setAllowedAreas( Qt::AllDockWidgetAreas );
  this->addDockWidget( Qt::RightDockWidgetArea, componentPick );

  /*
   * Schema Editor View
   */
  schema = new SchemaWidget( this, schsheet, dom );
  this->setCentralWidget( schema );

  connect( componentPick, SIGNAL(deviceInserted( const DeviceLibraryEntry * )), this, SLOT(onDeviceInserted( const DeviceLibraryEntry * )) );
}

void SchemaEditorForm::onDeviceInserted( const DeviceLibraryEntry *entry )
{
  dom->addPickedDevice( entry );
}

void SchemaEditorForm::onFileSave()
{
  using namespace std;

  QString filename = QFileDialog::getSaveFileName( this, tr("Save schema..."), QString(), tr("Schema file(*.sch)") );
  if( filename.length() )
    {
      std::string fn = filename.toStdString();
      ofstream stream( fn.c_str() );
      if( stream.good() )
        {
          MainWindow::instance()->processRc( schema->view()->serialize( DOM_SCHEMA, stream ) );
        }
      else
        MainWindow::instance()->processRc( -DS_CREATE_FILE );
    }
}

int SchemaEditorForm::openSchemaFile( std::ifstream & stream )
{
  using namespace std;

  int rc = schema->view()->deserialize( DOM_SCHEMA, stream ); UPDATE_RC(rc);

  componentPick->clearDevices();

  const list<const DeviceLibraryEntry *> &pickedDevices = dom->pickedDevices();

  for( list<const DeviceLibraryEntry *>::const_iterator it=pickedDevices.begin(); it!=pickedDevices.end(); it++ )
    {
      const DeviceLibraryEntry *entry = *it;
      componentPick->addComponent( entry );
    }

  return 0;
}

} // namespace dsim
