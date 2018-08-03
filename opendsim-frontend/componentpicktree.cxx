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

#include <device/libentry.h>

#include "componentpicktree.h"

namespace dsim
{

ComponentPickTree::ComponentPickTree( QWidget* parent )
                 : QTreeWidget( parent )
{
  setDragEnabled(true);
  setDragDropMode( QAbstractItemView::DragOnly );
  setIndentation( 12 );
  setRootIsDecorated( true );
  headerItem()->setHidden( true );
  setIconSize( QSize( 16, 16));

  setContextMenuPolicy( Qt::CustomContextMenu );

  connect( this, SIGNAL(customContextMenuRequested(const QPoint&)),
           this, SLOT  (onContextMenu(const QPoint&)));

  connect( this, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
           this, SLOT  (onItemClicked(QTreeWidgetItem*, int)) );
}
ComponentPickTree::~ComponentPickTree()
{
}

void ComponentPickTree::addCategory( const QString &category )
{
  QTreeWidgetItem *treeitem = new QTreeWidgetItem();
  treeitem->setFlags( QFlag(32) );
  QFont font = treeitem->font(0);
  font.setWeight(75);
  treeitem->setIcon( 0, QIcon(":bitmaps/modecomponent.png") );
  treeitem->setFont( 0, font );
  treeitem->setText( 0, category );
  treeitem->setTextColor( 0, QColor( 110, 95, 50 )/*QColor(255, 230, 200)*/ );
  treeitem->setBackground( 0, QBrush(QColor(240, 235, 245)) );
  treeitem->setChildIndicatorPolicy( QTreeWidgetItem::ShowIndicator );

  addTopLevelItem( treeitem );

  treeitem->setExpanded( true );

  m_categories[category] = treeitem;
}

void ComponentPickTree::addComponent( const QString &category, const DeviceLibraryEntry *entry )
{
  QTreeWidgetItem* treeentry = m_categories[category];

  if( !treeentry ) return;

  QTreeWidgetItemIterator it( treeentry ); // check if there is no existing item before
  while( *it )
    {
      if( (*it)->data( 0, Qt::UserRole + 100 ).value<void *>() == entry )
        return;
      it++;
    }

  QTreeWidgetItem* treeitem =  new QTreeWidgetItem(0);
  treeitem->setText( 0, entry->symbol_name );
  treeitem->setFlags( QFlag(32) );
  treeitem->setIcon( 0, QIcon(":bitmaps/modecomponent.png") );
  treeitem->setData( 0, Qt::UserRole + 100, QVariant::fromValue( (void *)entry ) );

  treeentry->addChild( treeitem );
}

const DeviceLibraryEntry *ComponentPickTree::deivceEntry( QTreeWidgetItem *item )
{
  return static_cast<const DeviceLibraryEntry *>(item->data( 0, Qt::UserRole + 100 ).value<void *>());
}

QTreeWidgetItem* ComponentPickTree::treeEntry( const QString &category )
{
  return m_categories[category];
}

void ComponentPickTree::mouseReleaseEvent( QMouseEvent* event )
{
  event->ignore();
}

void ComponentPickTree::onItemClicked( QTreeWidgetItem* item, int column)
{
  Q_UNUSED( column );

  if( !item ) return;
  const DeviceLibraryEntry *entry = (const DeviceLibraryEntry *)(item->data( 0, Qt::UserRole + 100 ).value<void *>());
  if( 0l == entry ) return;

  QMimeData *mimeData = new QMimeData;

  mimeData->setText( entry->symbol_name );

  QDrag *drag = new QDrag(this);
  drag->setMimeData(mimeData);

  drag->exec( Qt::CopyAction | Qt::MoveAction, Qt::CopyAction );
}

void ComponentPickTree::onContextMenu( const QPoint& point )
{
  QMenu menu;

  QAction* managePluginAction = menu.addAction( tr("Remove Component") );
  connect( managePluginAction, SIGNAL(triggered()), this, SLOT(onRemoveComponent()) );

  menu.exec( mapToGlobal(point) );
}

void ComponentPickTree::onRemoveComponent()
{

}

} // namespace dsim

#include "componentpicktree.moc"
