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

#include <dsim/device-lib.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/rbtree.h>
#include <dsim/string.h>
#include <cstring>

#include "componentlibrarydialog.h"

namespace dsim
{

enum { NODE_ROLE = Qt::UserRole + 100 };

ComponentLibraryDialog::ComponentLibraryDialog( QWidget *parent )
                      : QDialog( parent )
                      , m_devices( device_lib_get_categories() )
                      , m_device_tree( device_lib_get_tree() )
                      , m_wholeWord( false )
{
  createDataModels();
  createWidgets();
  createActions();

  this->setWindowTitle( tr("Pick Device") );
  this->update();

  loadCategories();
}

ComponentLibraryDialog::~ComponentLibraryDialog()
{
}

void ComponentLibraryDialog::createDataModels()
{
  category = new QStandardItemModel( this );
  subCategory = new QStandardItemModel( this );
  searchResults = new QStandardItemModel( this );
}

void ComponentLibraryDialog::createWidgets()
{
  QHBoxLayout *centralLayout = new QHBoxLayout( this );
  centralLayout->setMargin( 0 );
  centralLayout->setSpacing( 0 );

  QWidget *queryWidget = new QWidget( this );
  QVBoxLayout *queryLayout = new QVBoxLayout( queryWidget );
  QLabel *lable[3];
  lable[0] = new QLabel( tr("Keyword:"), queryWidget );
  lable[1] = new QLabel( tr("Category"), queryWidget );
  lable[2] = new QLabel( tr("Sub Category"), queryWidget );
  QCheckBox *wholewordCheck = new QCheckBox( queryWidget );
  wholewordCheck->setText( tr("Match whole word?") );
  wholewordCheck->setCheckState( Qt::Unchecked );
  keywordEdit = new QLineEdit( queryWidget );
  categoryList = new QListView( queryWidget );
  categoryList->setModel( category );
  subCategoryList = new QListView( queryWidget );
  subCategoryList->setModel( subCategory );

  queryLayout->addWidget( lable[0] );
  queryLayout->addWidget( keywordEdit );
  queryLayout->addWidget( wholewordCheck );
  queryLayout->addWidget( lable[1] );
  queryLayout->addWidget( categoryList );
  queryLayout->addWidget( lable[2] );
  queryLayout->addWidget( subCategoryList );
  QSizePolicy p = queryWidget->sizePolicy();
  p.setHorizontalStretch( 4 );
  queryWidget->setSizePolicy( p );
  centralLayout->addWidget( queryWidget );

  QWidget *resultsWidget = new QWidget( this );
  QVBoxLayout *resultsLayout = new QVBoxLayout( resultsWidget );
  resultsLabel = new QLabel( tr("Results:"), resultsWidget );
  searchResultList = new QTreeView( resultsWidget );
  searchResultList->setEditTriggers( QTreeView::NoEditTriggers );
  searchResultList->setModel( searchResults );
  resultsLayout->addWidget( resultsLabel );
  resultsLayout->addWidget( searchResultList );
  p = resultsWidget->sizePolicy();
  p.setHorizontalStretch( 10 );
  resultsWidget->setSizePolicy( p );
  centralLayout->addWidget( resultsWidget );

  connect( keywordEdit, SIGNAL(textEdited(const QString &)), this, SLOT(onKeywordChanged(const QString &)) );

  connect( wholewordCheck, SIGNAL(stateChanged(int)), this, SLOT(onWholewordChanged(int)) );

  connect( categoryList, SIGNAL(clicked(const QModelIndex &)),this, SLOT(onCategoryClicked(const QModelIndex &)) );

  connect( subCategoryList, SIGNAL(clicked(const QModelIndex &)),this, SLOT(onSubCategoryClicked(const QModelIndex &)) );

  connect( searchResultList,SIGNAL(doubleClicked ( const QModelIndex &)),this,SLOT(onSearchListDoubleClick(const QModelIndex &)));
}

void ComponentLibraryDialog::createActions()
{
}

QSize ComponentLibraryDialog::sizeHint () const
{
  return QSize( 879, 472 );
}

void ComponentLibraryDialog::loadCategories()
{
  categoryList->setUpdatesEnabled( false );

  category_node_t *lastNode = 0l;

  category->clear();

  foreach_hashmap( category_node_t, node, m_devices )
    {
      const char *category_name = node->name;
      QStandardItem* item = new QStandardItem( QString(category_name) );
      item->setData( QVariant::fromValue((void *)node), NODE_ROLE );
      category->appendRow( item );
      lastNode = node;
    }

  if( lastNode ) loadSubCategory( lastNode );

  categoryList->setUpdatesEnabled( true );
}

void ComponentLibraryDialog::loadSubCategory( const category_node_t *node )
{
  subCategoryList->setUpdatesEnabled( false );

  subCategory->clear();

  subCategory->appendRow( new QStandardItem( QString("(All sub-categories)") ) );

  foreach_hashmap( sub_category_node_t, sub_node, &node->sub_categories )
    {
      const char *subname = sub_node->name;
      QStandardItem* item = new QStandardItem( QString(subname) );
      item->setData( QVariant::fromValue((void *)sub_node), NODE_ROLE );
      subCategory->appendRow( item );
    }

  subCategoryList->setUpdatesEnabled( true );

  loadAllEntries();
}

void ComponentLibraryDialog::clearSearchResults()
{
  searchResults->clear();
  searchResults->setHorizontalHeaderLabels( QStringList() << tr("Device symbol") << tr("Sub-category") << tr("Manufacturer") << tr("Description") );
}

void ComponentLibraryDialog::insertEntryItem( const DeviceLibraryEntry *entry )
{
  QStandardItem* item = new QStandardItem( QString(entry->symbol_name) );
  item->setData( QVariant::fromValue((void *)entry), NODE_ROLE );
  searchResults->appendRow( item );
  int row = searchResults->indexFromItem(item).row();
  searchResults->setItem(row, 1, new QStandardItem( QString(entry->sub_category) ) );
  searchResults->setItem(row, 2, new QStandardItem( QString(entry->manufacturer) ) );
  searchResults->setItem(row, 3, new QStandardItem( QString(entry->description) ) );
}

void ComponentLibraryDialog::loadEntries( const sub_category_node_t *sub_node, bool clear )
{
  searchResultList->setUpdatesEnabled( false );

  if( clear ) clearSearchResults();

  foreach_list( device_entry_node_t, entry_node, &sub_node->entries )
    {
      const DeviceLibraryEntry *entry = entry_node->entry;
      insertEntryItem( entry );
    }

  searchResultList->setUpdatesEnabled( true );
}

void ComponentLibraryDialog::loadAllEntries()
{
  clearSearchResults();

  int count = subCategory->rowCount();
  for( int i=1; i < count; i++ )
    {
      loadEntries( (sub_category_node_t *)(subCategory->item( i, 0 )->data(NODE_ROLE).value<void*>()), false );
    }
}

void ComponentLibraryDialog::onKeywordChanged( const QString &value )
{
  QByteArray bytes = value.toLocal8Bit();
  const char *dst_symbol = bytes.data();
  int dst_len = std::strlen( dst_symbol );

  if( dst_len == 0 ) return;

  searchResultList->setUpdatesEnabled( false );

  clearSearchResults();

  if( m_wholeWord )
    {
      foreach_rbtree( device_entry_search_node_t, node, m_device_tree )
        {
          if( 0 == ds_strcasecmp( node->symbol_name, dst_symbol ) )
            {
              insertEntryItem( node->entry );
            }
        }
    }
  else
    {
      foreach_rbtree( device_entry_search_node_t, node, m_device_tree )
        {
          if( 0 == ds_strncasecmp( node->symbol_name, dst_symbol, dst_len ) )
            {
              insertEntryItem( node->entry );
            }
        }
    }

  searchResultList->setUpdatesEnabled( true );
}

void ComponentLibraryDialog::onWholewordChanged( int state )
{
  m_wholeWord = ( state == Qt::Checked ? true : false );
  onKeywordChanged( keywordEdit->text() );
}

void ComponentLibraryDialog::onCategoryClicked( const QModelIndex &index )
{
  QStandardItem *currentItem = category->itemFromIndex(index);
  if( currentItem )
    loadSubCategory( (category_node_t *)(index.data(NODE_ROLE).value<void*>()) );
}

void ComponentLibraryDialog::onSubCategoryClicked( const QModelIndex &index )
{
  QStandardItem *currentItem = subCategory->itemFromIndex(index);
  if( currentItem->index().row() == 0 )
    {
      loadAllEntries();
      return;
    }
  if( currentItem )
    {
      loadEntries( (sub_category_node_t *)(index.data(NODE_ROLE).value<void*>()) );
    }
}

void ComponentLibraryDialog::onSearchListDoubleClick( const QModelIndex &index )
{
  DeviceLibraryEntry *entry = ( DeviceLibraryEntry * )(index.data(NODE_ROLE).value<void*>());

  if ( entry ) emit insertDevice( entry );
}

}

#include "componentlibrarydialog.moc"
