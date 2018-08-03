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

#define TRACE_UNIT "propmod"
#include <dsim/logtrace.h>

#include <QMessageBox>
#include "propertybase.h"
#include "propertymodel.h"

namespace dsim
{

PropertyModel::PropertyModel(QObject* parent /*= 0*/) : QAbstractTableModel(parent)
{    
  m_rootItem = new PropertyBase( "root", QString(), this );
}

PropertyModel::~PropertyModel() {}

QModelIndex PropertyModel::index( int row, int column, const QModelIndex & parent /*= QModelIndex()*/ ) const
{
  PropertyBase *parentItem = m_rootItem;
  if( parent.isValid() )
    {
      parentItem = static_cast<PropertyBase*>(parent.internalPointer());
    }
  if( row >= parentItem->children().size() || row < 0 )
    {
      return QModelIndex();
    }
  return createIndex( row, column, parentItem->children().at(row) );
}

QModelIndex PropertyModel::parent( const QModelIndex & index ) const
{
  if( !index.isValid() ) return QModelIndex();

  PropertyBase *childItem  = static_cast<PropertyBase*>(index.internalPointer());
  PropertyBase *parentItem = qobject_cast<PropertyBase*>(childItem->parent());

  if( !parentItem || parentItem == m_rootItem ) return QModelIndex();

  return createIndex( parentItem->row(), 0, parentItem );
}

int PropertyModel::rowCount( const QModelIndex & parent /*= QModelIndex()*/ ) const
{
  PropertyBase *parentItem = m_rootItem;
  if( parent.isValid() )
    parentItem = static_cast<PropertyBase*>(parent.internalPointer());
  return parentItem->children().size();
}

int PropertyModel::columnCount( const QModelIndex & /*parent = QModelIndex()*/ ) const
{ return 3; }

QVariant PropertyModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole*/ ) const
{
  if( !index.isValid() ) return QVariant();

  PropertyBase *item = static_cast<PropertyBase*>(index.internalPointer());
  switch( role )
  {
    case Qt::SizeHintRole: return  QSize( 25, 25 );
    case Qt::ToolTipRole:        
    case Qt::DecorationRole:
    case Qt::DisplayRole:
    case Qt::EditRole:
      if( index.column() == 0 ) return item->name();
      if( index.column() == 1 ) return item->symbol();
      if( index.column() == 2 ) return item->value( role );
      break;
  };
  return QVariant();
}

bool PropertyModel::setData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/ )
{
  if( index.isValid() && role == Qt::EditRole )
    {
      PropertyBase *item = static_cast<PropertyBase*>(index.internalPointer());
      if( value != item->value() )
        {
          if( QMessageBox::warning( 0l, tr("Save property"),
                                    QString( tr("Confirm to save the value of property:\n%1=%2 ?") ).arg( item->symbol() ).arg( value.toString() ),
                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes ) == QMessageBox::Yes )
            {
              item->setValue( value );
              emit dataChanged( index, index );
            }
        }
      return true;
    }
  return false;
}

Qt::ItemFlags PropertyModel::flags( const QModelIndex & index ) const
{
  if( !index.isValid() ) return Qt::ItemIsEnabled;

  if( index.column() == 0 || index.column() == 1 )
    {
      return Qt::ItemIsEnabled
              | Qt::ItemIsSelectable;
    }
  return Qt::ItemIsEnabled
           | Qt::ItemIsSelectable
           | Qt::ItemIsEditable;
}

QVariant PropertyModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
  if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
      switch( section )
        {
           case 0: return tr("Property");
           case 1: return tr("Symbol");
           case 2: return tr("Value");
        }
    }
  return QVariant();
}

QModelIndex PropertyModel::buddy( const QModelIndex & index ) const
{
  if( index.isValid() && index.column() == 0 )
    return createIndex( index.row(), 1, index.internalPointer() );
  return index;
}

void PropertyModel::addItem( ModelPropertyValue *model, const QString& name /*= QString()*/, const QString &symbol /*= QString()*/ )
{
  new PropertyBase( model, name, symbol, m_rootItem);

  int rows = rowCount();
  beginInsertRows( QModelIndex(), rows, rows + 1 );
  endInsertRows();
}

void PropertyModel::addItem( DevicePropertyValue *device, const QString& name /*= QString()*/, const QString &symbol /*= QString()*/ )
{
  new PropertyBase( device, name, symbol, m_rootItem);

  int rows = rowCount();
  beginInsertRows( QModelIndex(), rows, rows + 1 );
  endInsertRows();
}

void PropertyModel::clear()
{
  beginRemoveRows( QModelIndex(), 0, rowCount() );
  delete m_rootItem;
  m_rootItem = new PropertyBase( "root", QString(), this );
  endRemoveRows();
}

}
