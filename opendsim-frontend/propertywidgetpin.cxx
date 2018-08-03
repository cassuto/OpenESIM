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

#include <cstring>

#include "elementpin.h"
#include "propertywidgetpin.h"

namespace dsim
{

PropertyWidgetPin::PropertyWidgetPin( ElementPin *pin, QWidget *parent )
                  : PropertyWidget( pin, parent )
{
  QGridLayout *centralLayout = new QGridLayout( this );

  QLabel *label = new QLabel( tr("Set Symbol:"), this );
  centralLayout->addWidget( label, 0, 0, 1, 1 );
  m_symbolEdit = new QLineEdit( pin->symbol(), this );
  m_symbolEdit->selectAll();
  centralLayout->addWidget( m_symbolEdit, 0, 1, 1, 1 );
  label = new QLabel( tr("Set Reference:"), this );
  centralLayout->addWidget( label, 1, 0, 1, 1 );
  m_referenceEdit = new QLineEdit( pin->reference(), this );
  m_referenceEdit->selectAll();
  centralLayout->addWidget( m_referenceEdit, 1, 1, 1, 1 );
  label = new QLabel( tr("Set Length:"), this );
  centralLayout->addWidget( label, 2, 0, 1, 1 );
  m_lengthSpin = new QSpinBox( this );
  m_lengthSpin->setRange( 1, 60 );
  m_lengthSpin->setValue( pin->length() );
  centralLayout->addWidget( m_lengthSpin, 2, 1, 1, 1 );
  m_showSymbol = new QCheckBox( tr("Show Symbol?"), this );
  m_showSymbol->setChecked( pin->showSymbol() );
  centralLayout->addWidget( m_showSymbol, 3, 0, 1, 1 );
  m_showReference = new QCheckBox( tr("Show Reference?"), this );
  m_showReference->setChecked( pin->showReference() );
  centralLayout->addWidget( m_showReference, 4, 0, 1, 1 );
  label = new QLabel( tr("Set IO type:"), this );
  centralLayout->addWidget( label, 5, 0, 1, 1 );
  m_ioType = new QComboBox( this );

  /*
   * keep the order of IOTYPE enum definition
   */
  m_ioType->addItem( "passive", int(IOTYPE_PASSIVE) );
  m_ioType->addItem( "input", int(IOTYPE_INPUT) );
  m_ioType->addItem( "output", int(IOTYPE_OUTPUT) );
  m_ioType->addItem( "io", int(IOTYPE_IO) );
  m_ioType->addItem( "power", int(IOTYPE_POWER) );
  m_ioType->addItem( "pullup", int(IOTYPE_PULLUP) );
  m_ioType->addItem( "pulldown", int(IOTYPE_PULLDOWN) );

  int idx = int(pin->ioType());
  if( idx >=0 && idx < m_ioType->count() )
    {
      m_ioType->setCurrentIndex( idx );
    }
  centralLayout->addWidget( m_ioType, 5, 1, 1, 1 );
}

const char *PropertyWidgetPin::ioType2String( io_type_t io )
{
  switch( io )
  {
    case IOTYPE_PASSIVE:    return "passive";
    case IOTYPE_INPUT:      return "input";
    case IOTYPE_OUTPUT:     return "output";
    case IOTYPE_IO:         return "io";
    case IOTYPE_POWER:      return "power";
    case IOTYPE_PULLUP:     return "pullup";
    case IOTYPE_PULLDOWN:   return "pulldown";
  }
  return "unknown";
}

io_type_t string2IoType( const char *string )
{
  if( 0==std::strcmp( string, "passive" ) )
    {
      return IOTYPE_PASSIVE;
    }
  else if( 0==std::strcmp( string, "input" ) )
    {
      return IOTYPE_INPUT;
    }
  else if( 0==std::strcmp( string, "output" ) )
    {
      return IOTYPE_OUTPUT;
    }
  else if( 0==std::strcmp( string, "io" ) )
    {
      return IOTYPE_IO;
    }
  else if( 0==std::strcmp( string, "power" ) )
    {
      return IOTYPE_POWER;
    }
  else if( 0==std::strcmp( string, "pullup" ) )
    {
      return IOTYPE_PULLUP;
    }
  else if( 0==std::strcmp( string, "pulldown" ) )
    {
      return IOTYPE_PULLDOWN;
    }
  return IOTYPE_PASSIVE;
}

void PropertyWidgetPin::accept()
{
  ElementPin *pin = opaque<ElementPin *>();

  pin->setSymbol( m_symbolEdit->text() );
  pin->setReference( m_referenceEdit->text() );
  pin->setLength( m_lengthSpin->value() );
  pin->setShowSymbol( m_showSymbol->checkState() == Qt::Checked );
  pin->setShowReference( m_showReference->checkState() == Qt::Checked );
  pin->setIOType( (io_type_t ) m_ioType->currentIndex() );
}

}

#include "propertywidgetpin.moc"
