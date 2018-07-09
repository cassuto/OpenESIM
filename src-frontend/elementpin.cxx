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

#include "domdataset.h"
#include "componentgraphitem.h"
#include "elementtext.h"

#include "elementpin.h"

namespace dsim
{

ElementPin::ElementPin( int id, bool edit, ElemDirect direct, const QPoint &pos, ElementText *text, SchemaGraph *schgraph, ElementGraphItem* parent )
          : ElementGraphItem( id, edit, schgraph, parent )
          , m_symbolLabel( text )
{
  setBoundingRect( QRect(-4, -4, 12, 8) );

  setPos( pos );
  setDirect( direct );
  setLength( 32 );

  if( !edit ) setCursor( Qt::CrossCursor );

  setSub( text );
}

ElementPin::~ElementPin()
{
}

void ElementPin::setDirect( ElemDirect direct )
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

  setLayout();
}

void ElementPin::setLength( int length )
{
  m_length = length;

  setBoundingRect( QRect(-4, -4, m_length + 4, 8) );
}

void ElementPin::setSub( ElementText *symbol )
{
  if( !m_symbolLabel && symbol )
    {
      addElement( symbol );
      m_symbolLabel = symbol;

      symbol->setStyle( "pin" );
      setLayout();
    }
}

void ElementPin::setLayout()
{
  if( !m_symbolLabel ) return;

  QFontMetrics fm( m_symbolLabel->font() );

  int xlabelpos = pos().x();
  int ylabelpos = pos().y();

  switch( m_direct )
  {
    case ELEM_RIGHT:
      xlabelpos -= fm.width(m_symbolLabel->text())+m_length+1;
      ylabelpos -= 5;
      m_symbolLabel->setDirect( ELEM_LEFT );
      break;

    case ELEM_TOP:
      xlabelpos += 5;
      ylabelpos += m_length+1;
      m_symbolLabel->setDirect( m_direct );
      break;

    case ELEM_LEFT:
      xlabelpos += m_length+1;
      ylabelpos -= 5;
      m_symbolLabel->setDirect( ELEM_LEFT );
      break;

    case ELEM_BOTTOM:
      xlabelpos -= 5;
      ylabelpos -= m_length+1;
      m_symbolLabel->setDirect( m_direct );
      break;
  }
  m_symbolLabel->setPos( QPointF(xlabelpos, ylabelpos) );
}

int ElementPin::serialize( DomDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset ); UPDATE_RC(rc);

  rc = dataset->ser( int(m_length) );   UPDATE_RC(rc);

  return rc;
}

int ElementPin::deserialize( DomDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset ); UPDATE_RC(rc);

  int length;
  rc = dataset->des( length );          UPDATE_RC(rc);
  setLength( length );

  return rc;
}

int ElementPin::resolveSubElements()
{
  int rc = ElementGraphItem::resolveSubElements(); UPDATE_RC(rc);

  m_symbolLabel = static_cast<ElementText *>(m_elements[0]);
  setLayout();
  return 0;
}

void ElementPin::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Q_UNUSED(option); Q_UNUSED(widget);

  QPen pen(Qt::black, 1, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin);

  if( isSelected() ) pen.setColor( Qt::darkGray);

  painter->setPen( pen );

  if( m_length < 1 ) m_length = 1;
  painter->drawLine( 0, 0, m_length-1, 0);

  pen.setColor( Qt::red );
  pen.setWidthF( 0.7 );
  painter->setPen( pen );
  painter->drawEllipse( -4, -4, 8, 8 );
}

}
