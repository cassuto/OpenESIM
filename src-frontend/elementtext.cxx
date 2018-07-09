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

#include <dsim/error.h>

#include "templatestyle.h"
#include "domdataset.h"

#include "elementtext.h"

namespace dsim
{

ElementText::ElementText( int id, bool edit, ElemDirect direct, const QPoint &pos, SchemaGraph *schgraph, ElementGraphItem* parent )
          : ElementGraphItem( id, edit, schgraph, parent )
          , m_text( new QGraphicsSimpleTextItem( this ) )
{
  setPos( pos );
  setDirect( direct );

  setStyle( "component" );
  m_text->setText("");
  m_text->setRotation( 0 );

  updateTextBound();
}

ElementText::~ElementText()
{
}

void ElementText::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementText::setDirect( ElemDirect direct )
{
  ElementGraphItem::setDirect( direct );

  switch( direct )
  {
    case ELEM_RIGHT: setRotation( 180 ); break;
    case ELEM_TOP: setRotation( 90 ); break;
    case ELEM_LEFT: setRotation( 0 ); break;
    case ELEM_BOTTOM: setRotation( -90 ); break;
    default: return;
  }

  updateTextBound();
}

void ElementText::setText( QString label )
{
  m_text->setText( label );
  updateTextBound();
}

void ElementText::setColor( QColor color )
{
  m_text->setBrush( color );
}

QFont ElementText::font() const
{
  return m_text->font();
}

QString ElementText::text() const
{
  return m_text->text();
}

int ElementText::serialize( DomDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset ); UPDATE_RC(rc);

  std::string symbol = m_text->text().toStdString();

  rc = dataset->ser( symbol );          UPDATE_RC(rc);

  return rc;
}

int ElementText::deserialize( DomDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset ); UPDATE_RC(rc);

  std::string symbol;
  rc = dataset->des( symbol );          UPDATE_RC(rc);

  setText( symbol.c_str() );
  updateTextBound();

  return rc;
}

void ElementText::updateTextBound()
{
  QFontMetrics fm( m_text->font() );

  setBoundingRect( QRect(0, 0, fm.width(m_text->text())+1, fm.height()+1 ) );
}

void ElementText::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  ElementGraphItem::applyTextStyle( m_text, style(), isSelected() );

  m_text->paint( painter, option, widget );
}

}
