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
#include <cstdio>
#include "templatestyle.h"
#include "lispdataset.h"
#include "schemaview.h"
#include "staffpad.h"

#include "elementline.h"

namespace dsim
{

ElementLine::ElementLine( const QPointF &p0, int id, SchemaScene *scene, bool edit, QGraphicsItem* parent )
          : ElementGraphItem<QGraphicsItem>( id, scene, edit, parent )
{
  setStyle( "component" );

  ElementGraphItem<QGraphicsItem>::setPos( p0 );
  m_lastpoint = p0 - pos();
  setFineturningEnabled( true );
}

ElementLine::~ElementLine()
{
}

void ElementLine::setStyle( const char *style )
{
  ElementGraphItem::setStyle( style );
  update();
}

void ElementLine::addPoint( const QPointF &point )
{
  QGraphicsLineItem *line = 0l;

  QPointF rp = point - pos(); // relevant position

  if( !m_lines.empty() ) // find the collinear and reuse
    {
      line = m_lines.last();

      float aX = line->line().p1().x(), aY = line->line().p1().y();
      float bX = line->line().p2().x(), bY = line->line().p2().y();

      if( (rp.x()-aX)*(bY-aY)-(rp.y()-aY)*(bX-aX) == 0 )
        {
          QLineF l = line->line();
          l.setP2( rp );
          line->setLine( l );
          m_lastpoint = rp;
          return;
        }
    }
  else // create start point pad
    {
      StaffPad *pad = new StaffPad( m_pads.count(), m_lastpoint, ElementBase::scene(), this, this );
      pad->setVisible( false );
      m_pads.append( pad );
    }

  line = new QGraphicsLineItem( this );
  line->setLine( QLineF( m_lastpoint, rp ) );
  line->setFlag( QGraphicsItem::ItemStacksBehindParent, true );
  line->setFlag( QGraphicsItem::ItemIsSelectable, false );

  StaffPad *pad = new StaffPad( m_pads.count(), rp, ElementBase::scene(), this, this );
  pad->setVisible( false );

  m_lastpoint = rp;
  m_lines.append( line );
  m_pads.append( pad );
}

void ElementLine::setLastPoint( const QPointF &point )
{
  if( !m_lines.empty() )
    {
      QGraphicsLineItem *line = m_lines.last();
      QPointF rp = point - pos(); // relevant position

      prepareGeometryChange();

      QLineF l = line->line();
      l.setP2( rp );
      line->setLine( l );
      m_lastpoint = rp;
    }
}

void ElementLine::removeLastPoint()
{
  if( !m_lines.empty() )
      {
        QGraphicsLineItem *line = m_lines.last();

        m_lastpoint = line->line().p1();
        ElementBase::scene()->removeItem( line );
        m_lines.removeOne( line );
        delete line;

        StaffPad *pad = m_pads.last();
        delete pad;
        m_pads.removeOne( pad );
      }
}

int ElementLine::pointCount() // the origin is not included
{ return m_lines.count(); }

int ElementLine::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );          UPDATE_RC(rc);

  QPointF cp = pos();

  bool x0 = true;

  rc = dataset->ser( double(pos().x()) );                   UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );                   UPDATE_RC(rc);

  rc = dataset->beginNode(true, "points");                  UPDATE_RC(rc);

  foreach( QGraphicsLineItem *line, m_lines )
    {
      if( x0 ) // serialize the start point
        {
          QPointF ap = cp + line->line().p1(); // absolute position

          rc = dataset->beginNode(true);                    UPDATE_RC(rc);
          rc = dataset->ser( double(ap.x()) );              UPDATE_RC(rc);
          rc = dataset->ser( double(ap.y()) );              UPDATE_RC(rc);
          rc = dataset->endNode(true);                      UPDATE_RC(rc);
          x0 = false;
        }
      QPointF ap = cp + line->line().p2(); // absolute position

      rc = dataset->beginNode(true);                        UPDATE_RC(rc);
      rc = dataset->ser( double(ap.x()) );                  UPDATE_RC(rc);
      rc = dataset->ser( double(ap.y()) );                  UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }

  rc = dataset->endNode(true);                              UPDATE_RC(rc);
  return rc;
}

int ElementLine::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );        UPDATE_RC(rc);

  double x, y;

  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  setPos( QPointF( x, y ) );

  bool x0 = true;
  DomEntry entry;

  rc = dataset->des( entry, "points" );                     UPDATE_RC(rc);

  for( entry.begin(); entry.valid(); ++entry )
    {
      dataset->deserializePush( entry.child() );
        {
          LispDataset::AutoPop autoPop( dataset );

          double x1, y1;

          rc = dataset->des( x1 );                          UPDATE_RC(rc);
          rc = dataset->des( y1 );                          UPDATE_RC(rc);

          if( UNLIKELY(x0) ) // set origin ( start point )
            {
              m_lastpoint = QPointF( x1, y1 ) - pos();
              x0 = false;
            }
          else
            {
              addPoint( QPointF( x1, y1 ) );
            }
        } // auto pop
    }

  updatePads();

  return rc;
}

void ElementLine::updatePads()
{
  if( !isRoot() || !editable() || m_lines.empty() ) return;

  m_pads[0]->setPos( m_lines[0]->line().p1() - m_pads[0]->central() );
  m_pads[0]->setVisible( isSelected() );

  int i = 1;
  foreach( QGraphicsLineItem *line, m_lines )
    {
      m_pads[i]->setPos( line->line().p2() - m_pads[i]->central() );
      m_pads[i]->setVisible( isSelected() );
      i++;
    }
}

void ElementLine::setSelected( bool selected )
{
  ElementGraphItem::setSelected( selected );
  updatePads();
}

QRectF ElementLine::boundingRect() const
{
  return shape().controlPointRect();
}
QPainterPath ElementLine::shape() const
{
  QPainterPath pathsum;

  foreach( QGraphicsLineItem *line, m_lines )
    {
      pathsum += line->shape();
    }

  return pathsum;
}

void ElementLine::staffMoveEvent( int index, bool fineturning, QGraphicsSceneMouseEvent *event )
{
  QPointF cp = fineturning ? event->scenePos() : togrid(event->scenePos());   // Absolute position
  QPointF rp = cp - pos(); // Relevant position
  QLineF line;
  QGraphicsLineItem *l = 0l;

  prepareGeometryChange();

  if( index==0 ) // start point
    {
      l = m_lines[0];
      line = l->line();
      line.setP1( rp );
      l->setLine( line );
    }
  else if( index==m_lines.count() ) // end point
    {
      l = m_lines.last();
      line = l->line();
      line.setP2( rp );
      l->setLine( line );
    }
  else
    {
      l = m_lines[index-1];
      line = l->line();
      line.setP2( rp );
      l->setLine( line );

      l = m_lines[index];
      line = l->line();
      line.setP1( rp );
      l->setLine( line );
    }
}

void ElementLine::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( painter, customLine(), customFill(), style(), isSelected() );

  foreach( QGraphicsLineItem *line, m_lines )
    {
      line->setPen( painter->pen() );
      line->paint( painter, option, widget );
    }
  updatePads();
}

}
