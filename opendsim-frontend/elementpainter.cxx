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

#include <cmath>
#include <frontend/error.h>
#include <device/graph.h>

#include "templatestyle.h"
#include "lispdataset.h"
#include "schemaview.h"
#include "componentgraphimpl.h"
#include "staffpad.h"

#include "elementpainter.h"

namespace dsim
{

ElementPainter::ElementPainter( const QRectF &rect, int id, SchemaScene *scene, bool edit, QGraphicsRectItem* parent )
          : ElementGraphItem<QGraphicsRectItem>( id, scene, edit, parent )
          , m_pixBuffer( 0l )
          , m_deviceGraph( new ComponentGraphImpl )
{
  if( edit )
    {
      for( int i=0; i<4; i++ )
        {
          m_pads[i] = new StaffPad( i, QPointF(0, 0), scene, this, this );
          m_pads[i]->setVisible( false );
        }
    }

  setRectParent( rect );
  setStyle( "painter" );
  setFineturningEnabled( true );
  m_deviceGraph->init( this );
}

ElementPainter::~ElementPainter()
{
}

void ElementPainter::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementPainter::setRectParent( const QRectF &rect )
{
  QRectF r( rect );

  setPos( r.topLeft() );
  QRectF mappedRect = mapRectFromParent( r );
  QGraphicsRectItem::setRect( mappedRect );
  m_deviceGraph->setSize( mappedRect.width()-1, mappedRect.height()-1 );
  updatePads();
}

QRectF ElementPainter::rectParent() const
{ return mapRectToParent( rect() ); }

void ElementPainter::normalizeRect()
{
  QRectF rect( rectParent().normalized() );

  if( rect.width() < gridWh/4 ) rect.setWidth( gridWh/4 ); // limit the minimum size
  if( rect.height() < gridHt/4 ) rect.setHeight( gridHt/4 );
  setRectParent( rect );
}

int ElementPainter::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );                  UPDATE_RC(rc);

  double x1, y1, x2, y2;
  rectParent().normalized().getCoords( &x1, &y1, &x2, &y2 );
  rc = dataset->ser( x1 );                                          UPDATE_RC(rc);
  rc = dataset->ser( y1 );                                          UPDATE_RC(rc);
  rc = dataset->ser( x2 );                                          UPDATE_RC(rc);
  rc = dataset->ser( y2 );                                          UPDATE_RC(rc);
  rc = dataset->ser( m_tokenId );                                   UPDATE_RC(rc);
  return rc;
}

int ElementPainter::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );                UPDATE_RC(rc);

  double x1, y1, x2, y2;

  rc = dataset->des( x1 );                                          UPDATE_RC(rc);
  rc = dataset->des( y1 );                                          UPDATE_RC(rc);
  rc = dataset->des( x2 );                                          UPDATE_RC(rc);
  rc = dataset->des( y2 );                                          UPDATE_RC(rc);
  rc = dataset->des( m_tokenId );                                   UPDATE_RC(rc);

  QRectF rect;
  rect.setCoords( x1, y1, x2, y2 );
  setRectParent( rect.normalized() );

  return rc;
}

void ElementPainter::updatePads()
{
  if( !isRoot() || !editable() ) return;

  m_pads[0]->setPos( QGraphicsRectItem::boundingRect().topLeft() - m_pads[0]->central() );
  m_pads[1]->setPos( QGraphicsRectItem::boundingRect().topRight() - m_pads[1]->central() );
  m_pads[2]->setPos( QGraphicsRectItem::boundingRect().bottomLeft() - m_pads[2]->central() );
  m_pads[3]->setPos( QGraphicsRectItem::boundingRect().bottomRight() - m_pads[3]->central() );

  for( int i=0; i<4; i++ )
    {
      m_pads[i]->setVisible( isSelected() );
    }
}

void ElementPainter::setSelected( bool selected )
{
  ElementGraphItem::setSelected( selected );
  updatePads();
}

void ElementPainter::staffMoveEvent( int index, bool fineturning, QGraphicsSceneMouseEvent *event )
{
  QPointF cp = fineturning ? event->scenePos() : togrid(event->scenePos());
  QRectF rect = rectParent();

  switch( index )
  {
    case 0: // topLeft
      {
        rect.setTopLeft( cp );
        break;
      }

    case 1: // topRight
      {
        rect.setTopRight( cp );
        break;
      }

    case 2: // bottomLeft
      {
        rect.setBottomLeft( cp );
        break;
      }

    case 3: // bottomRight
      {
        rect.setBottomRight( cp );
        break;
      }
  }

  setRectParent( rect );
}

void ElementPainter::staffMouseReleaseEvent( int, QGraphicsSceneMouseEvent * )
{ normalizeRect(); }

void ElementPainter::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( this, customLine(), customFill(), style(), isSelected() );

  QGraphicsRectItem::paint( painter, option, widget );
  if( m_pixBuffer )
    painter->drawImage( 1, 1, *m_pixBuffer );
  updatePads();
}

}
