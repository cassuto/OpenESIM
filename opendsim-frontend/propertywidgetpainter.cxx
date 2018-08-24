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

#include "elementpainter.h"
#include <device/graph.h>
#include "propertywidgetpainter.h"

namespace dsim
{

PropertyWidgetPainter::PropertyWidgetPainter( ElementPainter *painter, QWidget *parent )
                      : PropertyWidget( painter, parent )
{
  QVBoxLayout *centralLayout = new QVBoxLayout( this );

  QLabel *label = new QLabel( tr("Set token ID:"), this );
  centralLayout->addWidget( label );
  m_tokenId = new QComboBox( this );
  m_tokenId->setEditable( true );
  m_tokenId->setEditText( painter->tokenId().c_str() );
  centralLayout->addWidget( m_tokenId );

  centralLayout->addStretch( 1 );

}

PropertyWidgetPainter::~PropertyWidgetPainter() {}

void PropertyWidgetPainter::firstPage()
{
  m_tokenId->lineEdit()->setFocus( Qt::NoFocusReason );
  m_tokenId->lineEdit()->selectAll();
}

void PropertyWidgetPainter::accept()
{
  ElementPainter *painter = opaque<ElementPainter *>();
  painter->setTokenId( m_tokenId->currentText().toLatin1().data() );
}

}

#include "propertywidgetpainter.moc"
