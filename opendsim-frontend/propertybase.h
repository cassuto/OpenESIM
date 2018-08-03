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

#ifndef PROPERTYBASE_H_
#define PROPERTYBASE_H_

#include <QStyleOption>
#include <QVariant>
#include <QString>

#include <model/model.h>
#include <device/device.h>

namespace dsim
{

class PropertyBase : public QObject
{
  Q_OBJECT
public:
  PropertyBase( const QString& name = QString(), const QString& symbol = QString(), QObject* parent = 0 );
  PropertyBase( ModelPropertyValue *model, const QString& name = QString(), const QString& symbol = QString(), QObject* parent = 0 );
  PropertyBase( DevicePropertyValue *device, const QString& name = QString(), const QString& symbol = QString(), QObject* parent = 0 );

  virtual QString name() const;
  virtual QString symbol() const;
  virtual QVariant value( int role = Qt::UserRole ) const;
  virtual void setValue( const QVariant& value );
  virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option );
  virtual QVariant editorData( QWidget *editor );
  virtual bool setEditorData( QWidget *editor, const QVariant& data );

  int row() { return parent()->children().indexOf(this); }
  void setName( const QString &name ) { m_name = name; }
  void setSymbol( const QString &symbol ) { m_symbol = symbol; }

private:
  PropertyClass m_class;
  union
  {
    ModelPropertyValue *model;
    DevicePropertyValue *device;
  } m_u;
  QString       m_name;
  QString       m_symbol;
};

}

#endif
