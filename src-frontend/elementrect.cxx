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
#include "schemaview.h"
#include "staffpad.h"

#include "elementrect.h"

namespace dsim
{

ElementRect::ElementRect( int id, bool edit, const QPoint &pos, SchemaGraph *schgraph, ElementGraphItem* parent )
          : ElementGraphItem( id, edit, schgraph, parent )
          , m_width( 30 )
          , m_height( 30 )
{
  setPos( pos );

  setStyle( "component" );
  updateBounding();

  if( edit )
    {
      for( int i=0; i<4; i++ )
        {
          m_pads[i] = new StaffPad( i, QPointF(0, 0), schgraph, this, this );
          m_pads[i]->setVisible( false );
        }
      updatePads();
    }
}

ElementRect::~ElementRect()
{
}

void ElementRect::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementRect::setWidth( int width )
{ m_width = width; updateBounding(); }

void ElementRect::setHeight( int height )
{ m_height = height; updateBounding(); }

void ElementRect::setSize( int w, int h )
{
  if( w < gridWh ) w = gridWh;
  if( h < gridHt ) h = gridHt;

  m_width = w; m_height = h;
  updateBounding();
  updatePads();
  update();
}

int ElementRect::serialize( DomDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset ); UPDATE_RC(rc);

  rc = dataset->ser( int(m_width) );   UPDATE_RC(rc);
  rc = dataset->ser( int(m_height) );  UPDATE_RC(rc);

  return rc;
}

int ElementRect::deserialize( DomDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset ); UPDATE_RC(rc);

  int width, height;

  rc = dataset->des( width );          UPDATE_RC(rc);
  rc = dataset->des( height );         UPDATE_RC(rc);

  setSize( width, height );
  return rc;
}

void ElementRect::updateBounding()
{
  setBoundingRect( QRect(0, 0, m_width, m_height ) );
}

void ElementRect::updatePads()
{
  m_pads[0]->setPos( boundingRect().topLeft() - m_pads[0]->central() );
  m_pads[1]->setPos( boundingRect().topRight() - m_pads[1]->central() );
  m_pads[2]->setPos( boundingRect().bottomLeft() - m_pads[2]->central() );
  m_pads[3]->setPos( boundingRect().bottomRight() - m_pads[3]->central() );

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

void ElementRect::staffMoveEvent( int index, QGraphicsSceneMouseEvent *event )
{
  float dW = .0f, dH = .0f;

  QPointF cp = togrid(event->scenePos());

  switch( index )
  {
    case 0: // topLeft
      {
        dW = pos().x() - cp.x();
        dH = pos().y() - cp.y();
        setPos( cp );
        break;
      }

    case 1: // topRight
      {
        QPointF trp = pos() + boundingRect().topRight();
        dW = cp.x() - trp.x();
        dH = trp.y() - cp.y();
        setPos( pos().x(), cp.y() );
        break;
      }

    case 2: // bottomLeft
      {
        QPointF blp = pos() + boundingRect().bottomLeft();
        dW = blp.x() - cp.x();
        dH = cp.y() - blp.y();
        setPos( cp.x(), pos().y() );
        break;
      }

    case 3: // bottomRight
      {
        QPointF brp = pos() + boundingRect().bottomRight();
        dW = cp.x() - brp.x();
        dH = cp.y() - brp.y();
        break;
      }
  }

  float w = m_width, h = m_height;

  w += dW;
  h += dH;

  setSize( w, h );
}

void ElementRect::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{

  ElementGraphItem::applyStyle( painter, style(), isSelected() );

  painter->drawRect( QRect( 0, 0, m_width, m_height ) );

  updatePads();
}

}
