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

#include <QtWidgets>

#include "presscolorbox.h"

namespace dsim
{

PressColorBox::PressColorBox( QWidget *parent )
              : QComboBox( parent )
              , m_color( palette().color( QPalette::Base ) )
{
  updateDisp();

  addItem( QString() );
  setCurrentIndex( 0 );
}

void PressColorBox::mousePressEvent( QMouseEvent *event )
{
  event->accept();
  emit clicked();

  QColorDialog cd( m_color );

  if( cd.exec() == QDialog::Accepted )
    {
      setColor( cd.selectedColor() );
    }
}

void PressColorBox::setColor( const QColor &color )
{
  m_color = color;

  QPixmap pixmap( 16, 16 );
  pixmap.fill( color );

  setItemIcon( currentIndex(), pixmap );
  updateDisp();

  emit changed( color );
}

void PressColorBox::updateDisp()
{
  setItemText( currentIndex(), QString("[%1,%2,%3]").arg( m_color.red() ).arg( m_color.green() ).arg( m_color.blue() ) );
}

void PressColorBox::setEnabled( bool enabled )
{
  QComboBox::setEnabled( enabled );
  setColor( m_color );
}

}

#include "presscolorbox.moc"
