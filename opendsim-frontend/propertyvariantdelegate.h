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

#ifndef PROPERTYVARIANTDELEGATE_H_
#define PROPERTYVARIANTDELEGATE_H_

#include <QItemDelegate>

class QSignalMapper;

namespace dsim
{

class PropertyVariantDelegate : public QItemDelegate
{
public:
  PropertyVariantDelegate( QObject* parent = 0 );
  virtual ~PropertyVariantDelegate();

  QWidget *createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;
  virtual void setEditorData( QWidget *editor, const QModelIndex &index ) const;
  virtual void setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const;
  virtual void updateEditorGeometry( QWidget *editor,  const QStyleOptionViewItem &option, const QModelIndex &index ) const;

  QSignalMapper*    m_finishedMapper;
};

}

#endif
