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

#include "elementtext.h"
#include <device/graph.h>
#include "propertywidgettext.h"

namespace dsim
{

PropertyWidgetText::PropertyWidgetText( ElementText *element, QWidget *parent )
                      : PropertyWidget( element, parent )
{
  QVBoxLayout *centralLayout = new QVBoxLayout( this );

  QLabel *label = new QLabel( tr("Set Text:"), this );
  label->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Fixed );
  m_text = new QComboBox( this );
  m_text->setEditable( true );
  m_text->setEditText( element->text() );

  centralLayout->addWidget( label );
  centralLayout->addWidget( m_text );

  QGroupBox *groups = new QGroupBox( tr("Rotation"), this );
  groups->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Maximum );
  QGridLayout *layout = new QGridLayout( groups );
  m_roatationButtons = new QButtonGroup( groups );
  m_roatationButtons->setExclusive( true );
  QRadioButton *radio[4];
  radio[0] = new QRadioButton( tr("Left"), groups );
  layout->addWidget( radio[0], 0, 0, 1, 1 );
  m_roatationButtons->addButton( radio[0] );
  radio[1] = new QRadioButton( tr("Top"), groups );
  layout->addWidget( radio[1], 0, 1, 1, 1 );
  m_roatationButtons->addButton( radio[1] );
  radio[2] = new QRadioButton( tr("Right"), groups );
  layout->addWidget( radio[2], 1, 0, 1, 1 );
  m_roatationButtons->addButton( radio[2] );
  radio[3] = new QRadioButton( tr("Bottom"), groups );
  layout->addWidget( radio[3], 1, 1, 1, 1 );
  m_roatationButtons->addButton( radio[3] );

  int index = (int)(element->direct());
  if( index >= 0 && index < 4 )
    radio[index]->setChecked( true );

  centralLayout->addWidget( groups );
  centralLayout->addStretch( 1 );
}

PropertyWidgetText::~PropertyWidgetText() {}

void PropertyWidgetText::firstPage()
{
  m_text->lineEdit()->setFocus( Qt::NoFocusReason );
  m_text->lineEdit()->selectAll();
}

void PropertyWidgetText::accept()
{
  ElementText *textElement = opaque<ElementText *>();
  textElement->setText( m_text->currentText() );

  QAbstractButton *checked = m_roatationButtons->checkedButton();
  if( checked )
    {
      if( 0==checked->text().compare( "Left" ) )
        {
          textElement->setDirect( ELEM_LEFT );
        }
      else if( 0==checked->text().compare( "Top" ) )
        {
          textElement->setDirect( ELEM_TOP );
        }
      else if( 0==checked->text().compare( "Right" ) )
        {
          textElement->setDirect( ELEM_RIGHT );
        }
      else if( 0==checked->text().compare( "Bottom" ) )
        {
          textElement->setDirect( ELEM_BOTTOM );
        }
    }
}

}

#include "propertywidgettext.moc"
