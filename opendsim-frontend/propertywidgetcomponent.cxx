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

#include "componentgraphitem.h"
#include <device/graph.h>
#include "unitsconversion.h"
#include "propertywidgetcomponent.h"

namespace dsim
{

PropertyWidgetComponent::PropertyWidgetComponent( ComponentGraphItem *component, QWidget *parent )
                      : PropertyWidget( component, parent )
{
  component->syncParameters();

  QVBoxLayout *centralLayout = new QVBoxLayout( this );

  QLabel *label = new QLabel( component->symbol().c_str(), this );
  label->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  centralLayout->addWidget( label );

  label = new QLabel( tr("Set Reference:"), this );
  centralLayout->addWidget( label );
  m_reference = new QComboBox( this );
  m_reference->setEditable( true );
  m_reference->setEditText( component->reference().c_str() );
  centralLayout->addWidget( m_reference );

  label = new QLabel( tr("Display Value:"), this );
  centralLayout->addWidget( label );
  m_value = new QComboBox( this );
  m_value->setEditable( true );
  m_value->setEditText( component->value().c_str() );
  centralLayout->addWidget( m_value );

  connect( m_value, SIGNAL(editTextChanged(const QString &)), this, SLOT(valueTextChanged(const QString &)) );

  label = new QLabel( tr("Valid Value:"), this );
  centralLayout->addWidget( label );
  m_validValue = new QLabel( this );
  m_validValue->setFrameStyle( QFrame::Panel | QFrame::Sunken );
  centralLayout->addWidget( m_validValue );

  centralLayout->addStretch( 1 );

  valueTextChanged( component->value().c_str() );
}

PropertyWidgetComponent::~PropertyWidgetComponent() {}

void PropertyWidgetComponent::firstPage()
{
  m_value->lineEdit()->setFocus( Qt::NoFocusReason );
  m_value->lineEdit()->selectAll();
}

void PropertyWidgetComponent::accept()
{
  ComponentGraphItem *component = opaque<ComponentGraphItem *>();
  component->setReference( m_reference->currentText() );
  component->setValue( m_value->currentText() );
}

void PropertyWidgetComponent::valueTextChanged( const QString &text )
{ m_validValue->setText( UnitsConversion::unified( text ) ); }

}

#include "propertywidgetcomponent.moc"
