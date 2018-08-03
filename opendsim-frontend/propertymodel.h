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

#ifndef PROPERTYMODEL_H_
#define PROPERTYMODEL_H_

#include <QAbstractTableModel>
#include <QMap>

#include "propertybase.h"

namespace dsim
{

class PropertyBase;

class PropertyModel : public QAbstractTableModel
{
public:
  PropertyModel( QObject* parent = 0 );
  virtual ~PropertyModel();

  QModelIndex index( int row, int column, const QModelIndex & parent = QModelIndex() ) const;
  QModelIndex parent( const QModelIndex & index ) const;
  int rowCount( const QModelIndex & parent = QModelIndex() ) const;
  int columnCount( const QModelIndex & parent = QModelIndex() ) const;
  QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const;
  bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
  Qt::ItemFlags flags( const QModelIndex & index ) const;
  QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
  QModelIndex buddy( const QModelIndex & index ) const;

  void addItem( ModelPropertyValue *model, const QString& name = QString(), const QString &symbol = QString() );
  void addItem( DevicePropertyValue *device, const QString& name = QString(), const QString &symbol = QString() );
  void clear();

private:
  PropertyBase*                                  m_rootItem;
};

}

#endif
