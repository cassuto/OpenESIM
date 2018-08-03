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
#include "lispdataset.h"

#include "transformations.h"

namespace dsim
{

void Transformations::rotation( int angle, int x0, int y0 )
{
  double xFactor = cos( angle ), yFactor = sin( angle );
  double x = 0, y = 0;
  double x1, y1;
  int i = 0;

  QVector<QPointF>::iterator it;

  for( it=m_originPoints.begin(); it!=m_originPoints.end(); ++it )
    {
      x = (*it).x(); y = (*it).y();

      x1 = (x - x0)*xFactor - (y - y0)*yFactor + x0;
      y1 = (x - x0)*yFactor + (y - y0)*xFactor + y0;

      m_transPoints[i++] = QPointF( x1, y1 );
    }
}

void Transformations::rotation( ElemDirect direction, int x0, int y0 )
{
  switch( direction )
  {
    case ELEM_RIGHT:    rotation( 180, x0, y0 );    break;
    case ELEM_TOP:      rotation( 90, x0, y0 );     break;
    case ELEM_LEFT:     rotation( 0, x0, y0 );      break;
    case ELEM_BOTTOM:   rotation( -90, x0, y0 );    break;
  }
}

void Transformations::move( const QPointF &pos )
{
  int i = 0;
  QVector<QPointF>::iterator it;

  for( it=m_originPoints.begin(); it!=m_originPoints.end(); ++it )
    {
      m_transPoints[i++] = (*it) + pos;
    }
}

void Transformations::addPoint( const QPointF &point )
{ m_originPoints.append( point ); m_transPoints.append( point ); }

void Transformations::removeOne( const QPointF &point )
{ m_originPoints.removeOne( point ); m_transPoints.removeOne( point ); }

void Transformations::removeAll()
{ m_originPoints.clear(); m_transPoints.clear(); }


int Transformations::serialize( LispDataset *dataset )
{
  int rc = 0;

  rc = dataset->beginNode(true, "trans");                   UPDATE_RC(rc);

  foreach( QPointF point, m_originPoints )
    {
      rc = dataset->beginNode(true);                        UPDATE_RC(rc);
      rc = dataset->ser( double(point.x()) );               UPDATE_RC(rc);
      rc = dataset->ser( double(point.y()) );               UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }

  rc = dataset->endNode(true);                              UPDATE_RC(rc);
  return rc;
}

int Transformations::deserialize( LispDataset *dataset )
{
  int rc = 0;

  removeAll();

  DomEntry entry;

  rc = dataset->des( entry, "trans" );                      UPDATE_RC(rc);

  for( entry.begin(); entry.valid(); ++entry )
    {
      dataset->deserializePush( entry.child() );
        {
          LispDataset::AutoPop autoPop( dataset );

          double x, y;

          rc = dataset->des( x );                           UPDATE_RC(rc);
          rc = dataset->des( y );                           UPDATE_RC(rc);

          addPoint( QPointF( x, y ) );
        } // auto pop
    }

  return rc;
}

}
