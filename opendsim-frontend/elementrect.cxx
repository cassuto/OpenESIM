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
#include "lispdataset.h"
#include "schemaview.h"
#include "staffpad.h"

#include "elementrect.h"

namespace dsim
{

ElementRect::ElementRect( const QRectF &rect, int id, SchemaGraph *scene, bool edit, QGraphicsRectItem* parent )
          : ElementGraphItem<QGraphicsRectItem>( id, scene, edit, parent )
{
  if( edit )
    {
      for( int i=0; i<4; i++ )
        {
          m_pads[i] = new StaffPad( i, QPointF(0, 0), scene, this, this );
          m_pads[i]->setVisible( false );
        }
    }

  setRect( rect );
  setStyle( "component" );
  setFineturningEnabled( true );
}

ElementRect::~ElementRect()
{
}

void ElementRect::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementRect::setRect( const QRectF &rect )
{
  QRectF r( rect );
  if( r.width() < gridWh ) r.setWidth( gridWh );
  if( r.height() < gridHt ) r.setHeight( gridHt );

  QGraphicsRectItem::setRect( r );
  updatePads();
}

int ElementRect::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );                  UPDATE_RC(rc);

  rc = dataset->ser( double(rect().topLeft().x()) );                UPDATE_RC(rc);
  rc = dataset->ser( double(rect().topLeft().y()) );                UPDATE_RC(rc);
  rc = dataset->ser( double(rect().bottomRight().x()) );            UPDATE_RC(rc);
  rc = dataset->ser( double(rect().bottomRight().y()) );            UPDATE_RC(rc);

  return rc;
}

int ElementRect::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );                UPDATE_RC(rc);

  double x1, y1, x2, y2;

  rc = dataset->des( x1 );                                          UPDATE_RC(rc);
  rc = dataset->des( y1 );                                          UPDATE_RC(rc);
  rc = dataset->des( x2 );                                          UPDATE_RC(rc);
  rc = dataset->des( y2 );                                          UPDATE_RC(rc);

  QRectF rect = QGraphicsRectItem::rect();
  rect.setTopLeft( QPointF( x1, y1 ) );
  rect.setBottomRight( QPointF( x2, y2 ) );
  setRect( rect );
  return rc;
}

void ElementRect::updatePads()
{
  if( isRef() || !editable() ) return;

  m_pads[0]->setPos( QGraphicsRectItem::boundingRect().topLeft() - m_pads[0]->central() );
  m_pads[1]->setPos( QGraphicsRectItem::boundingRect().topRight() - m_pads[1]->central() );
  m_pads[2]->setPos( QGraphicsRectItem::boundingRect().bottomLeft() - m_pads[2]->central() );
  m_pads[3]->setPos( QGraphicsRectItem::boundingRect().bottomRight() - m_pads[3]->central() );

  for( int i=0; i<4; i++ )
    {
      m_pads[i]->setVisible( isSelected() );
    }
}

void ElementRect::setSelected( bool selected )
{
  ElementGraphItem::setSelected( selected );
  updatePads();
}

void ElementRect::staffMoveEvent( int index, bool fineTurning, QGraphicsSceneMouseEvent *event )
{
  QPointF cp = fineTurning ? event->scenePos() : togrid(event->scenePos());
  QRectF rect = QGraphicsRectItem::rect();

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

  setRect( rect );
}

void ElementRect::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{

  Templatestyle::apply( this, customLine(), customFill(), style(), isSelected() );

  QGraphicsRectItem::paint( painter, option, widget );
  updatePads();
}

}
