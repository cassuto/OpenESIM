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

#ifndef COMPONENTLIBRARYDIALOG_H_
#define COMPONENTLIBRARYDIALOG_H_

#include <QtWidgets>

#include <dsim/hashmap.h>
#include <dsim/rbtree.h>
#include <dsim/device-lib.h>

namespace dsim
{

class ComponentLibraryDialog : public QDialog
{
  Q_OBJECT

public:
  ComponentLibraryDialog( QWidget *parent );
  ~ComponentLibraryDialog();

private:
  void createDataModels();
  void createWidgets();
  void createActions();

  void loadCategories();
  void loadSubCategory( const category_node_t *node );
  void loadEntries( const sub_category_node_t *sub_node, bool clear = true );
  void loadAllEntries();
  void insertEntryItem( const DeviceLibraryEntry *entry );
  void clearSearchResults();

private slots:
  void onKeywordChanged( const QString &value );
  void onWholewordChanged( int state );
  void onCategoryClicked( const QModelIndex &index );
  void onSubCategoryClicked( const QModelIndex &index );
  void onSearchListDoubleClick( const QModelIndex &index );

signals:
  void insertDevice( const DeviceLibraryEntry *entry );

protected:
  QSize sizeHint () const;

private:
  const hashmap_t *m_devices;
  rb_tree_t *m_device_tree;

  QLineEdit *keywordEdit;
  QListView *categoryList;
  QListView *subCategoryList;
  QLabel    *resultsLabel;
  QTreeView *searchResultList;
  QStandardItemModel *category;
  QStandardItemModel *subCategory;
  QStandardItemModel *searchResults;

  bool          m_wholeWord;
};

}

#endif
