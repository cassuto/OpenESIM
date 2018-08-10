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

namespace dsim
{

InstrumentPickDialog::InstrumentPickDialog( InstrumentManagement *rack, QWidget *parent /*= 0l*/ )
                   : QDialog( parent )
                   , m_rack( rack )
{
  setWindowTitle( tr("Instrument Rack View") );

  QVBoxLayout *layout = new QVBoxLayout( this );

  QLabel *label = new QLabel( tr("Select a instrument from following list:"), this );
  layout->addWidget( label );

  m_instList = new QTreeWidget( this );
  m_instList->headerItem()->setHidden( true );
  m_instList->setIconSize( QSize( 16, 16));
  m_root = new QTreeWidgetItem();
  m_root->setText( 0, tr("Built-in Instruments") );
  m_instList->addTopLevelItem( m_root );
  m_root->setExpanded( true );

  foreach_rbtree( inst_entry_search_node_t, node, rack->instrumentTree() )
    {
      QTreeWidgetItem *leaf = new QTreeWidgetItem();
      leaf->setText( 0, node->classname );
      m_root->addChild( leaf );
    }

  connect( m_instList, SIGNAL(itemClicked( QTreeWidgetItem *,int )), this, SLOT(itemClicked( QTreeWidgetItem *,int )) );

  connect( m_instList, SIGNAL(itemDoubleClicked( QTreeWidgetItem *,int )), this, SLOT(itemDoubleClicked( QTreeWidgetItem *,int )) );

  layout->addWidget( m_instList );

  QDialogButtonBox *buttons = new QDialogButtonBox( this );
  buttons->addButton( tr("OK"), QDialogButtonBox::AcceptRole );
  buttons->addButton( tr("Cancel"), QDialogButtonBox::AcceptRole );

  layout->addWidget( buttons );

  connect( buttons, SIGNAL(accepted()), this, SLOT(accept()));

  connect( buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

void InstrumentPickDialog::itemClicked(QTreeWidgetItem *item, int )
{
  if( item != m_root )
    m_selectedClassname = item->text(0);
  else
    m_selectedClassname.clear();
}

void InstrumentPickDialog::itemDoubleClicked(QTreeWidgetItem *item, int )
{
  if( item != m_root )
    {
      m_selectedClassname = item->text(0);
      accept();
    }
  else
    m_selectedClassname.clear();
}

}

#include "instrumentpickdialog.moc"
