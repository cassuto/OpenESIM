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

#include <dsim/misc.h>
#include "elementabstractport.h"
#include "componentgraphitem.h"
#include "netlistexplorerdialog.h"

namespace dsim
{

enum { NODE_ROLE = Qt::UserRole + 100 };

NetlistExplorerDialog::NetlistExplorerDialog( const QList<SchemaSheet::SchemaNode *> *nodes, QWidget *parent )
                      : QDialog( parent )
{
  setWindowTitle( tr("Physical Netlist") );
  createWidgets();
  loadNodes( nodes );
}

void NetlistExplorerDialog::createWidgets()
{
  QHBoxLayout *layout = new QHBoxLayout( this );
  nodesList = new QTreeWidget( this );
  nodesList->setEditTriggers( QTreeView::NoEditTriggers );
  nodesList->setHeaderHidden( true );
  QSizePolicy p = nodesList->sizePolicy();
  p.setHorizontalStretch( 4 );
  nodesList->setSizePolicy( p );
  layout->addWidget( nodesList );

  nodesRoot = new QTreeWidgetItem;
  nodesRoot->setIcon( 0, QIcon(":bitmaps/circuitnoderoot.png") );
  nodesRoot->setText( 0, tr("Circuit Root") );
  nodesRoot->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );
  nodesList->addTopLevelItem( nodesRoot );
  nodesRoot->setExpanded( true );

  portsList = new QTreeWidget( this );
  portsList->setEditTriggers( QTreeView::NoEditTriggers );
  portsList->setHeaderLabels( QStringList() << "port" << "component" << "symbol" );
  p = portsList->sizePolicy();
  p.setHorizontalStretch( 10 );
  portsList->setSizePolicy( p );
  layout->addWidget( portsList );

  connect( nodesList, SIGNAL(itemClicked(QTreeWidgetItem *, int)),this, SLOT(onNodesListClicked(QTreeWidgetItem *, int)) );

  connect( portsList, SIGNAL(itemClicked(QTreeWidgetItem *, int)),this, SLOT(onPortsListClicked(QTreeWidgetItem *, int)) );
}

void NetlistExplorerDialog::loadNodes( const QList<SchemaSheet::SchemaNode *> *nodeList )
{
  nodesList->setUpdatesEnabled( false );

  int index = 0;
  foreach( SchemaSheet::SchemaNode *node, *nodeList )
    {
      QTreeWidgetItem* treeitem =  new QTreeWidgetItem;
      treeitem->setText( 0, QString("#%1").arg( index, 6, 10, QChar('0') ) );
      treeitem->setIcon( 0, QIcon(":bitmaps/circuitnode.png") );
      treeitem->setData( 0, NODE_ROLE, QVariant::fromValue( (void *)node ) );
      nodesRoot->addChild( treeitem );
      ++index;
    }

  nodesList->setUpdatesEnabled( true );
}

void NetlistExplorerDialog::onNodesListClicked( QTreeWidgetItem *item, int column )
{
  SchemaSheet::SchemaNode *node = (SchemaSheet::SchemaNode *)(item->data( 0, NODE_ROLE).value<void*>());
  if( node )
    {
      portsList->clear();

      foreach( ElementAbstractPort *port, *(node->ports()) )
        {
          QTreeWidgetItem* treeitem =  new QTreeWidgetItem;
          treeitem->setText( 0, port->reference() );
          treeitem->setIcon( 0, QIcon(":bitmaps/circuitport.png") );
          treeitem->setData( 0, NODE_ROLE, QVariant::fromValue( (void *)node ) );
          treeitem->setText( 1, port->component()->reference().c_str() );
          treeitem->setText( 2, port->component()->symbol().c_str() );
          portsList->addTopLevelItem( treeitem );
        }
    }
  UNUSED(column);
}

void NetlistExplorerDialog::onPortsListClicked( QTreeWidgetItem *item, int column )
{
  UNUSED( item ); UNUSED( column );
}

}

#include "netlistexplorerdialog.moc"
