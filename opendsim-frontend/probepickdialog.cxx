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
#include <device/device.h>
#include <frontend/instrument-lib.h>
#include "instrumentmanagement.h"
#include "instrumentrackcellwidget.h"
#include "elementbase.h"
#include "componentgraphitem.h"
#include "probepickdialog.h"

namespace dsim
{

enum { NODE_ROLE = Qt::UserRole + 100 };

DECLARE_ELEMENT_CAST( ComponentGraphItem, "component" )

ProbePickDialog::ProbePickDialog( SchemaView *schemaView, QWidget *parent /*= 0l*/ )
                   : QDialog( parent )
                   , m_selectedProbe( 0l )
{
  setWindowTitle( tr("Instrument Rack View") );

  QVBoxLayout *layout = new QVBoxLayout( this );

  QLabel *label = new QLabel( tr("Select an probe from the following list:"), this );
  layout->addWidget( label );

  m_instList = new QTreeWidget( this );
  m_instList->headerItem()->setHidden( true );
  m_instList->setIconSize( QSize( 16, 16));

  m_currentRoot = new QTreeWidgetItem();
  m_currentRoot->setText( 0, tr("Current probes") );
  m_instList->addTopLevelItem( m_currentRoot );
  m_currentRoot->setExpanded( true );
  m_voltageRoot = new QTreeWidgetItem();
  m_voltageRoot->setText( 0, tr("Voltage probes") );
  m_instList->addTopLevelItem( m_voltageRoot );
  m_voltageRoot->setExpanded( true );

  foreach( ElementBase *element, schemaView->elements() )
    {
      if( ComponentGraphItem *component = element_cast<ComponentGraphItem *>(element) )
        {
          QTreeWidgetItem *root;
          switch( component->device()->get()->probe_type() )
          {
            case PROBE_CURRENT:
              root = m_currentRoot;
              break;
            case PROBE_VOLTAGE:
              root = m_voltageRoot;
              break;
            default:
              root = 0l;
          }
          if( root )
            {
              QTreeWidgetItem *leaf = new QTreeWidgetItem();
              leaf->setText( 0, QString("%1 - %2").arg( component->reference().c_str() ).arg( component->device()->get()->probe_attrs() ) );
              leaf->setData( 0, NODE_ROLE, QVariant::fromValue<void *>( component->device() ) );
              root->addChild( leaf );
            }
        }
    }

  connect( m_instList, SIGNAL(itemClicked( QTreeWidgetItem *,int )), this, SLOT(itemClicked( QTreeWidgetItem *,int )) );

  connect( m_instList, SIGNAL(itemDoubleClicked( QTreeWidgetItem *,int )), this, SLOT(itemDoubleClicked( QTreeWidgetItem *,int )) );

  layout->addWidget( m_instList );

  QDialogButtonBox *buttons = new QDialogButtonBox( this );
  buttons->addButton( tr("OK"), QDialogButtonBox::AcceptRole );
  buttons->addButton( tr("Cancel"), QDialogButtonBox::RejectRole );

  layout->addWidget( buttons );

  connect( buttons, SIGNAL(accepted()), this, SLOT(accept()));

  connect( buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

void ProbePickDialog::itemClicked(QTreeWidgetItem *item, int )
{
  m_selectedProbe = static_cast<PointerProxy<IDevice> *>( item->data( 0, NODE_ROLE ).value<void *>() );
}

void ProbePickDialog::itemDoubleClicked(QTreeWidgetItem *item, int )
{
  m_selectedProbe = static_cast<PointerProxy<IDevice> *>( item->data( 0, NODE_ROLE ).value<void *>() );
  if( m_selectedProbe->valid() )
    accept();
}

}

#include "probepickdialog.moc"
