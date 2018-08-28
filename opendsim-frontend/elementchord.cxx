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

#include "templatestyle.h"
#include "lispdataset.h"
#include "schemaview.h"
#include "staffpad.h"

#include "elementchord.h"

namespace dsim
{

ElementChord::ElementChord( const QRectF &rect, int id, SchemaScene *scene, bool edit, QGraphicsItem* parent )
          : ElementGraphItem<QGraphicsItem>( id, scene, edit, parent )
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
  setStyle( "component" );
  setFineturningEnabled( true );
}

ElementChord::~ElementChord()
{
}

void ElementChord::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementChord::setRectParent( const QRectF &rect )
{
  QRectF r( rect );

  setPos( r.topLeft() );
  setBoundingRect( mapRectFromParent( r ) );
  updatePads();
}

QRectF ElementChord::rectParent() const
{ return mapRectToParent( m_bounding ); }

void ElementChord::normalizeRect()
{
  QRectF rect( rectParent().normalized() );

  if( rect.width() < gridWh/4 ) rect.setWidth( gridWh/4 ); // limit the minimum size
  if( rect.height() < gridHt/4 ) rect.setHeight( gridHt/4 );
  setRectParent( rect );
}

int ElementChord::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );                  UPDATE_RC(rc);

  double x1, y1, x2, y2;
  rectParent().normalized().getCoords( &x1, &y1, &x2, &y2 );
  rc = dataset->ser( x1 );                                          UPDATE_RC(rc);
  rc = dataset->ser( y1 );                                          UPDATE_RC(rc);
  rc = dataset->ser( x2 );                                          UPDATE_RC(rc);
  rc = dataset->ser( y2 );                                          UPDATE_RC(rc);
  rc = dataset->ser( m_a );                                         UPDATE_RC(rc);
  rc = dataset->ser( m_alen );                                      UPDATE_RC(rc);

  return rc;
}

int ElementChord::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );                UPDATE_RC(rc);

  double x1, y1, x2, y2;

  rc = dataset->des( x1 );                                          UPDATE_RC(rc);
  rc = dataset->des( y1 );                                          UPDATE_RC(rc);
  rc = dataset->des( x2 );                                          UPDATE_RC(rc);
  rc = dataset->des( y2 );                                          UPDATE_RC(rc);
  rc = dataset->des( m_a );                                         UPDATE_RC(rc);
  rc = dataset->des( m_alen );                                      UPDATE_RC(rc);

  QRectF rect;
  rect.setCoords( x1, y1, x2, y2 );
  setRectParent( rect.normalized() );
  return rc;
}

void ElementChord::updatePads()
{
  if( !isRoot() || !editable() ) return;

  m_pads[0]->setPos( boundingRect().topLeft() - m_pads[0]->central() );
  m_pads[1]->setPos( boundingRect().topRight() - m_pads[1]->central() );
  m_pads[2]->setPos( boundingRect().bottomLeft() - m_pads[2]->central() );
  m_pads[3]->setPos( boundingRect().bottomRight() - m_pads[3]->central() );

  for( int i=0; i<4; i++ )
    {
      m_pads[i]->setVisible( isSelected() );
    }
}

void ElementChord::setSelected( bool selected )
{
  ElementGraphItem::setSelected( selected );
  updatePads();
}

void ElementChord::staffMoveEvent( int index, bool fineturning, QGraphicsSceneMouseEvent *event )
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

void ElementChord::staffMouseReleaseEvent( int, QGraphicsSceneMouseEvent * )
{ normalizeRect(); }

void ElementChord::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( painter, customLine(), customFill(), style(), isSelected() );

  painter->drawChord( boundingRect(), m_a * 16, m_alen * 16 );
  updatePads();
  UNUSED( option ); UNUSED( widget );
}

}
