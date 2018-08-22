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

#include "propertyvariantdelegate.h"

#include "propertybase.h"

#include <QAbstractItemView>
#include <QtCore/QSignalMapper>

namespace dsim
{

PropertyVariantDelegate::PropertyVariantDelegate( QObject* parent ) : QItemDelegate(parent)
{
  m_finishedMapper = new QSignalMapper(this);
  connect(m_finishedMapper, SIGNAL(mapped(QWidget*)), this, SIGNAL(commitData(QWidget*)));
  connect(m_finishedMapper, SIGNAL(mapped(QWidget*)), this, SIGNAL(closeEditor(QWidget*)));
}

PropertyVariantDelegate::~PropertyVariantDelegate(){}

QWidget *PropertyVariantDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem& option , const QModelIndex & index ) const
{
  PropertyBase* p = static_cast<PropertyBase*>(index.internalPointer());
  switch(p->value().type())
  {
    case QVariant::Color:
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::String:
    case QVariant::UserType:
      {
        QWidget *editor = p->createEditor(parent, option);
        if( editor )
          {
            if( editor->metaObject()->indexOfSignal("editFinished()") != -1)
              {
                connect(editor, SIGNAL(editFinished()), m_finishedMapper, SLOT(map()));
                m_finishedMapper->setMapping(editor, editor);
              }
            return editor;
          }
      }
      /* fall through */
    default:
      break;
  }
  return QItemDelegate::createEditor(parent, option, index); // no editor could be created
}

void PropertyVariantDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{        
  m_finishedMapper->blockSignals(true);
  QVariant data = index.model()->data(index, Qt::EditRole);

  switch(data.type())
  {
    case QVariant::Color:
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::String:
    case QVariant::UserType:
      if( static_cast<PropertyBase*>(index.internalPointer())->setEditorData(editor, data) ) // if editor couldn't be recognized use default
        break;
    /* fall through*/
    default:
      QItemDelegate::setEditorData(editor, index);
  }
  m_finishedMapper->blockSignals(false);
}

void PropertyVariantDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{    
  QVariant data = index.model()->data(index, Qt::EditRole);
  switch(data.type())
  {
    case QVariant::Color:
    case QVariant::Int:
    case QVariant::Double:
    case QVariant::String:
    case QVariant::UserType: 
      {
        QVariant data = static_cast<PropertyBase*>(index.internalPointer())->editorData(editor);
        if (data.isValid())
          {
            model->setData(index, data , Qt::EditRole);
            break;
          }
      }
      /* fall through */
    default:
      break;
  }
  QItemDelegate::setModelData(editor, model, index);
}

void PropertyVariantDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index ) const
{
  return QItemDelegate::updateEditorGeometry(editor, option, index);
}


}
