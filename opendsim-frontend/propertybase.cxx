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

#include <dsim/string.h>
#include "presscolorbox.h"
#include "propertybase.h"

#include <QSpinBox>
#include <QLineEdit>

namespace dsim
{

PropertyBase::PropertyBase( const QString& name /*= QString()*/, const QString& symbol /*= QString()*/, QObject* parent /*= 0*/ ) : QObject( parent )
         , m_class( PROPERTY_MODEL )
         , m_name( name )
         , m_symbol( symbol )
{ m_u.model = 0l; setObjectName( name ); }

PropertyBase::PropertyBase( ModelPropertyValue *model, const QString& name /*= QString()*/, const QString& symbol /*= QString()*/, QObject* parent /*= 0*/ ) : QObject( parent )
         , m_class( PROPERTY_MODEL )
         , m_name( name )
         , m_symbol( symbol )
{ m_u.model = model; setObjectName( name ); }

PropertyBase::PropertyBase( DevicePropertyValue *device, const QString& name /*= QString()*/, const QString& symbol /*= QString()*/, QObject* parent /*= 0*/ ) : QObject( parent )
         , m_class( PROPERTY_DEVICE )
         , m_name( name )
         , m_symbol( symbol )
{ m_u.device = device; setObjectName( name ); }


QString PropertyBase::name() const
{ return m_name; }

QString PropertyBase::symbol() const
{ return m_symbol; }

QVariant PropertyBase::value( int /*role = Qt::UserRole*/ ) const
{
  switch( m_class )
  {
    case PROPERTY_MODEL:
      {
        switch( m_u.model->type )
          {
            case MDEL_VAR_DOUBLE:
              {
                return QVariant( m_u.model->u.vd );
              }
            case MDEL_VAR_FLOAT:
              {
                return QVariant( m_u.model->u.vf );
              }
            case MDEL_VAR_INTEGER:
              {
                return QVariant( m_u.model->u.vi );
              }
            case MDEL_VAR_BOOL:
              {
                return QVariant( m_u.model->u.vb );
              }
            case MDEL_VAR_STRPTR:
              {
                if( m_u.model->u.vsp )
                  { return QVariant( m_u.model->u.vsp ); }
                else
                  { return QVariant( "null" ); }
              }
            case MDEL_VAR_VOIDPTR:
              {
                break;
              }
            case MDEL_VAR_CONST_VOIDPTR:
              {
                break;
              }
            default:
              return QVariant();
          }
        break;
      }

    case PROPERTY_DEVICE:
      {
        switch( m_u.device->type )
          {
            case DEV_VAR_DOUBLE:
              {
                return QVariant( m_u.device->u.vd );
              }
            case DEV_VAR_FLOAT:
              {
                return QVariant( m_u.device->u.vf );
              }
            case DEV_VAR_INTEGER:
              {
                return QVariant( m_u.device->u.vi );
              }
            case DEV_VAR_BOOL:
              {
                return QVariant( m_u.device->u.vb );
              }
            case DEV_VAR_STRPTR:
              {
                if( m_u.device->u.vsp )
                  { return QVariant( m_u.device->u.vsp ); }
                else
                  { return QVariant( "null" ); }
              }
            default:
              return QVariant();
          }
        break;
      }
  }

  return QVariant();
}

void PropertyBase::setValue( const QVariant &value )
{
  bool ok = true;

  switch( m_class )
    {
      case PROPERTY_MODEL:
        {
          switch( m_u.model->type )
            {
              case MDEL_VAR_DOUBLE:
                {
                  m_u.model->u.vd = value.toDouble( &ok ); break;
                }
              case MDEL_VAR_FLOAT:
                {
                  m_u.model->u.vf = value.toFloat( &ok ); break;
                }
              case MDEL_VAR_INTEGER:
                {
                  m_u.model->u.vi = value.toInt( &ok ); break;
                }
              case MDEL_VAR_BOOL:
                {
                  m_u.model->u.vb = value.toInt( &ok ); break;
                }
              case MDEL_VAR_STRPTR:
                {
                  m_u.model->u.vsp = ds_strdup( value.toString().toStdString().c_str() );
                }
              case MDEL_VAR_VOIDPTR:
                {
                  break;
                }
              case MDEL_VAR_CONST_VOIDPTR:
                {
                  break;
                }
              default:
                break;
            }
          break;
        }

      case PROPERTY_DEVICE:
        {
          switch( m_u.device->type )
            {
              case DEV_VAR_DOUBLE:
                {
                  m_u.device->u.vd = value.toDouble( &ok ); break;
                }
              case DEV_VAR_FLOAT:
                {
                  m_u.device->u.vf = value.toFloat( &ok ); break;
                }
              case DEV_VAR_INTEGER:
                {
                  m_u.device->u.vi = value.toInt( &ok ); break;
                }
              case DEV_VAR_BOOL:
                {
                  m_u.device->u.vb = value.toInt( &ok ); break;
                }
              case DEV_VAR_STRPTR:
                {
                  m_u.device->u.vsp = ds_strdup( value.toString().toStdString().c_str() );
                }
              default:
                break;
            }
          break;
        }
    }
  UNUSED(ok);
}

QWidget* PropertyBase::createEditor( QWidget *parent, const QStyleOptionViewItem& /*option*/ )
{
  QWidget* editor = 0;
  switch(value().type())
  {
    case QVariant::Color:
      editor = new PressColorBox(parent);
      break;
    case QVariant::Int:
      editor = new QSpinBox(parent);
              editor->setProperty("minimum", -1e9);
              editor->setProperty("maximum", 1e9);
      break;
    case QVariant::Double:    
      editor = new QDoubleSpinBox(parent);
              editor->setProperty("minimum", -1e9);
              editor->setProperty("maximum", 1e9);
              editor->setProperty("decimals", 3);
      break;
    case QVariant::String:
      editor = new QLineEdit(parent);
      break;
    default:
      return editor;
  }
  return editor;
}

bool PropertyBase::setEditorData( QWidget *editor, const QVariant &data )
{
  switch(value().type())
  {
    case QVariant::Color:
      static_cast<PressColorBox*>(editor)->setColor(data.value<QColor>());
      return true;;
    case QVariant::Int:
      editor->blockSignals(true);
      static_cast<QSpinBox*>(editor)->setValue(data.toInt());
      editor->blockSignals(false);
      return true;
    case QVariant::Double:    
      editor->blockSignals(true);
      static_cast<QDoubleSpinBox*>(editor)->setValue(data.toDouble());
      editor->blockSignals(false);
      return true;
    case QVariant::String:
      editor->blockSignals(true);
      static_cast<QLineEdit*>(editor)->setText(data.toString());
      editor->blockSignals(false);
      return true;
    default: 
      return false;
  }
  return false;
}

QVariant PropertyBase::editorData( QWidget *editor )
{
  switch( value().type() )
  {
    case QVariant::Color:
      return QVariant::fromValue(static_cast<PressColorBox*>(editor)->color());
    case QVariant::Int:
      return QVariant( static_cast<QSpinBox*>(editor)->value() );
    case QVariant::Double:    
      return QVariant( static_cast<QDoubleSpinBox*>(editor)->value() );
    case QVariant::String:
      return QVariant( static_cast<QLineEdit *>(editor)->text() );
    default:
      return QVariant();
  }
}

}

#include "propertybase.moc"
