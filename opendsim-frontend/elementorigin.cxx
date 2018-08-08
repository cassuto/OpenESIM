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

#include <frontend/error.h>

#include "templatestyle.h"
#include "lispdataset.h"

#include "elementorigin.h"

namespace dsim
{

ElementOrigin::ElementOrigin( const QPointF &pos, int id, SchemaScene *scene, bool edit, QGraphicsRectItem* parent )
          : ElementGraphItem<QGraphicsItem>( id, scene, edit, parent )
{
  setStyle( "component" );
  setPos( pos );
}

ElementOrigin::~ElementOrigin()
{
}

int ElementOrigin::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );                  UPDATE_RC(rc);

  rc = dataset->ser( double(pos().x()) );                           UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );                           UPDATE_RC(rc);

  return rc;
}

int ElementOrigin::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );                UPDATE_RC(rc);

  double x, y;

  rc = dataset->des( x );                                           UPDATE_RC(rc);
  rc = dataset->des( y );                                           UPDATE_RC(rc);
  setPos( QPointF( x, y ) );
  return rc;
}

QRectF ElementOrigin::boundingRect() const { return QRectF( -(gridWh/2), -(gridHt/2), gridWh, gridHt ); }

QPointF ElementOrigin::originPos() const { return pos(); }

void ElementOrigin::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
{
  if( isRoot() )
    {
      Templatestyle::apply( painter, customLine(), customFill(), style(), isSelected() );

      painter->drawEllipse( -(gridWh/2), -(gridHt/2), gridWh, gridHt );
      painter->drawLine( 0, -(gridHt/2), 0, gridHt/2 );
      painter->drawLine( -(gridWh/2), 0, gridWh/2, 0 );
    }
}

}
