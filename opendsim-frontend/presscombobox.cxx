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

#include "presscombobox.h"

namespace dsim
{

PressComboBox::PressComboBox( QWidget *parent )
              : QComboBox( parent )
              , m_color( palette().color( QPalette::Base ) )
{
}

void PressComboBox::mousePressEvent( QMouseEvent *event )
{
  emit clicked();
}

void PressComboBox::setColor( const QColor &color )
{
  m_color = color;
  QPalette p = palette();
  p.setColor( QPalette::Base, color );
  p.setColor( QPalette::Highlight, color );
  setPalette( p );
}

void PressComboBox::setEnabled( bool enabled )
{
  QComboBox::setEnabled( enabled );
  setColor( m_color );
}

}
