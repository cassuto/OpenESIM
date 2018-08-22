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

#include <instrument/instrumentbase.h>
#include <instrument/libentry.h>
#include <device/device.h>
#include "instrumentmanagement.h"
#include "probepickdialog.h"
#include "instrumentrackcellwidget.h"

namespace dsim
{

InstrumentRackCellWidget::InstrumentRackCellWidget( SchemaView *schemaView, const InstrumentPair &inst, int index, QWidget *parent /*= 0l*/ )
                   : QWidget( parent )
                   , m_schemaView( schemaView )
                   , m_inst( inst )
                   , m_index( index )
{
  QVBoxLayout *layout = new QVBoxLayout( this );

  QLabel *addProbe = new QLabel( tr("<a href = 'onAddProbe()'>Click here to attach a probe...</a>"), this );
  QLabel *removeProbe = new QLabel( tr("<a href = 'onRemoveProbe()'>Click here to remove attached probe...</a>"), this );
  QLabel *openPannel = new QLabel( tr("<a href = 'onOpenPanel()'>Open main panel...</a>"), this );

  connect( addProbe, SIGNAL(linkActivated( const QString& )), this, SLOT(onAddProbe( const QString& )) );
  connect( removeProbe, SIGNAL(linkActivated( const QString& )), this, SLOT(onRemoveProbe( const QString& )) );
  connect( openPannel, SIGNAL(linkActivated( const QString& )), this, SLOT(onOpenPanel( const QString& )) );

  layout->addWidget( addProbe );
  layout->addWidget( removeProbe );
  layout->addWidget( openPannel );

  m_probeName = new QLabel( this );
  layout->addWidget( m_probeName );
  layout->addStretch( 1 );

  updateProbeStatus();
}

void InstrumentRackCellWidget::updateProbeStatus()
{
  QString probeName = m_inst.base->probeDevice() && m_inst.base->probeDevice()->valid() ? m_inst.base->probeDevice()->get()->probe_attrs() : tr("none");
  m_probeName->setText( QString( tr("Probe: %1") ).arg( probeName ) );
}

void InstrumentRackCellWidget::onAddProbe( const QString& )
{
  ProbePickDialog *probeDialog = new ProbePickDialog( m_schemaView, this );
  if( probeDialog->exec() == QDialog::Accepted )
    {
      if( PointerProxy<IDevice> *deviceProxy = probeDialog->selectedProbe() )
        {
          m_inst.base->setProbeDevice( deviceProxy );
          updateProbeStatus();
        }
    }
  probeDialog->deleteLater();
}

void InstrumentRackCellWidget::onRemoveProbe( const QString& )
{
  m_inst.base->setProbeDevice( 0l );
  updateProbeStatus();
}

void InstrumentRackCellWidget::onOpenPanel( const QString& )
{ m_inst.base->open(); }

}

#include "instrumentrackcellwidget.moc"
