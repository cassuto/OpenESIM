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

#define TRACE_UNIT "elementwire"
#include <cmath>
#include <dsim/error.h>
#include <dsim/logtrace.h>

#include "templatestyle.h"
#include "lispdataset.h"
#include "schemaview.h"
#include "staffpad.h"
#include "elementjoint.h"
#include "elementwire.h"

namespace dsim
{

WireSegment::WireSegment( const QLineF &line, ElementWire *wire, QGraphicsItem* parent )
          : QGraphicsLineItem( line, parent )
          , m_elementWire( wire )
          , m_selected( false )
{
  setFlag( QGraphicsItemGroup::ItemIsSelectable, false );
  setAcceptHoverEvents( true );
  setLine( line );
}

WireSegment::~WireSegment()
{
}

void WireSegment::setLine( const QLineF &line )
{
  QGraphicsLineItem::setLine( line );
}

void WireSegment::setP1( const QPointF &point )
{
  emit sigSetPlainP2( point );
  setPlainP1( point );
}

void WireSegment::setP2( const QPointF &point )
{
  emit sigSetPlainP1( point );
  setPlainP2( point );
}

void WireSegment::setPlainP1( const QPointF &point )
{
  QLineF l = line();
  l.setP1( point );
  setLine( l );
}

void WireSegment::setPlainP2( const QPointF &point )
{
  QLineF l = line();
  l.setP2( point );
  setLine( l );
}

void WireSegment::moveWire( const QPointF &delta )
{
  QLineF l = line();
  qreal x1 = l.p1().x(), y1 = l.p1().y(), x2 = l.p2().x(), y2 = l.p2().y();

  if( ( dy() == 0 ) && ( dx() != 0 ) )
    {
      y1 += delta.y();
      y2 += delta.y();
    }
  else if( ( dx() == 0 ) && ( dy() != 0 ) )
    {
      x1 += delta.x();
      x2 += delta.x();
    }
  else return;

  setLine( QLineF( x1, y1, x2, y2 ) );

  emit sigSetPlainP1( line().p2() );
  emit sigSetPlainP2( line().p1() );
}

void WireSegment::move( QPointF delta )
{
  int index = m_elementWire->wireSegments()->indexOf( this );
  if( ( index == 0 ) || ( index == m_elementWire->wireSegments()->size()-1 ) ) // skip the start and end points
    return;

  moveWire( delta );
}

int WireSegment::serialize( LispDataset *dataset )
{
  int rc = 0;

  rc = dataset->ser( double(line().x1()) );                 UPDATE_RC(rc);
  rc = dataset->ser( double(line().y1()) );                 UPDATE_RC(rc);
  rc = dataset->ser( double(line().x2()) );                 UPDATE_RC(rc);
  rc = dataset->ser( double(line().y2()) );                 UPDATE_RC(rc);
  return rc;
}

int WireSegment::deserialize( LispDataset *dataset )
{
  int rc = 0;

  double x1, y1, x2, y2;

  rc = dataset->des( x1 );                                  UPDATE_RC(rc);
  rc = dataset->des( y1 );                                  UPDATE_RC(rc);
  rc = dataset->des( x2 );                                  UPDATE_RC(rc);
  rc = dataset->des( y2 );                                  UPDATE_RC(rc);

  setLine( QLineF(x1, y1, x2, y2) );
  return rc;
}

void WireSegment::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  SchemaView *schemaView = m_elementWire->view();
  if( event->button() == Qt::LeftButton )
    {
      event->accept();

      switch( m_elementWire->view()->mode() )
      {
        case MODE_SELECTION:
          grabMouse();
          return;

        case MODE_WIRE:
          {
            ElementWire *currentWire = static_cast<ElementWire *>(schemaView->m_hintElement);

            if( currentWire != m_elementWire ) // avoid connecting a wire to itself
              {
                ElementAbstractPort *port = m_elementWire->addJoint( this, event->scenePos() );
                schemaView->connectEndWire( port );
              }
            return;
          }

        default:
          return;
      }
    }

  event->ignore();
}

void WireSegment::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
  event->accept();

  int index = m_elementWire->wireSegments()->indexOf( this );

  if( index == 0 )
    m_elementWire->addWire( QLineF( p1(), p1() ), index );

  else if( index == m_elementWire->wireSegments()->size()-1 )
    m_elementWire->addWire( QLineF( p2(), p2() ), index + 1 );

  QPointF delta = togrid( event->scenePos() ) - togrid( event->lastScenePos() );
  moveWire( delta );
}

void WireSegment::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  event->accept();

  //m_elementWire->removeNullWires( this );

  if( event->button() == Qt::LeftButton )
    {
      ungrabMouse();
    }
}

void WireSegment::hoverEnterEvent( QGraphicsSceneHoverEvent *event )
{ event->accept(); m_selected = true; update(); }

void WireSegment::hoverLeaveEvent( QGraphicsSceneHoverEvent *event )
{ event->accept(); m_selected = false; update(); }

void WireSegment::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( painter, 0l, 0l, "wire", m_selected );

  setPen( painter->pen() );
  QGraphicsLineItem::paint( painter, option, widget );
}


ElementWire::ElementWire( int id, SchemaScene *scene, QGraphicsItem *parent )
            : ElementBase( id, scene )
            , m_parent( parent )
            , m_startPort( 0l )
            , m_endPort( 0l )
            , m_lastindex( 0 )
            , m_activeLine( 0 )
            , m_startPortId( -1 )
            , m_endPortId( -1 )
{
  setNonRoot( true );
}

ElementWire::~ElementWire()
{
  deleteAll();
  if( m_startPort ) m_startPort->disconnectWire( this, /*boardcast*/ true );
  if( m_endPort )   m_endPort->disconnectWire( this, /*boardcast*/ true );
}

void ElementWire::connectStartPort( ElementAbstractPort *port )
{
  port->connectWire( this );
  port->setOppositePort( 0l );
  m_startPort = port;
}

void ElementWire::connectEndPort( ElementAbstractPort *port, bool deser )
{
  port->connectWire( this ); m_endPort = port;
  if( !deser )
    {
      layoutWires( port, port->portScenePos() );
      removeNullWires();
    }
  m_startPort->setOppositePort( m_endPort );
  m_endPort->setOppositePort( m_startPort );
}

void ElementWire::disconnectPort( ElementAbstractPort *port, bool boardcast )
{
  if( port == m_startPort )
    {
      m_startPort->disconnectWire( this, boardcast );
      m_startPort->setOppositePort( 0l );
      m_startPort = 0l;
    }
  else if( port == m_endPort )
    {
      m_endPort->disconnectWire( this, boardcast );
      m_endPort->setOppositePort( 0l );
      m_endPort = 0l;
    }
  else
    {
      trace_assert(0); // port is an random pointer!!!
    }
}

void ElementWire::addWire( WireSegment* wire, int index )
{
  if( index > 0  && index < m_wires.size() ) disconnectWires( index-1, index );

  m_wires.insert( index, wire );

  ElementBase::scene()->addItem( wire );

  if( index > 0 )
    {
      connectWires( index-1, index );
      m_wires.at( index-1 )->setPlainP2( wire->p1() );
    }

  if( index < m_wires.size()-1 )
    {
      if( m_wires.size() < 2 ) return;

      connectWires( index, index+1 );
      m_wires.at( index+1 )->setPlainP1( wire->p2() );
    }
}

WireSegment* ElementWire::addWire( const QLineF &line, int index )
{
  WireSegment *wire = new WireSegment( line, this, m_parent );

  addWire( wire, index );
  return wire;
}

void ElementWire::addActiveWire()
{ if( m_activeLine < m_wires.size()-1 ) ++m_activeLine; }

ElementAbstractPort *ElementWire::addJoint( WireSegment *dst, const QPointF &scenePos )
{
  int index;
  int myindex = m_wires.indexOf( dst );
  QPointF point1 = togrid( scenePos );

  WireSegment* wire;

  if( ( ( (dst->dy() == 0) && ( std::fabs( point1.x()-dst->p2().x() ) < gridWh ) ) // point near the p2 corner
   ||( (dst->dx() == 0) && ( std::fabs( point1.y()-dst->p2().y() ) < gridHt ) ) )
   && ( myindex != m_wires.size()-1 ) )
    {
     if ( myindex == m_wires.size()-1 )
       {
         return 0l; // ignore
       }

     point1 = dst->p2();
     index = myindex+1;
     wire = m_wires.at( index );
    }
  else if( ( ( (dst->dy() == 0) && ( std::fabs( point1.x()-dst->p1().x() ) < gridWh ) ) // point near the p1 corner
        ||( (dst->dx() == 0) && ( std::fabs( point1.y()-dst->p1().y() ) < gridHt ) ) )
        &&( myindex != 0 ) )
    {
      if ( myindex == 0 )
       {
         return 0l; // ignore
       }
      point1 = dst->p1();
      wire = dst;
      index = myindex;
    }
  else                                // split this line in two
    {
     if ( dst->dy() == 0 )    point1.setY( dst->p1().y() );
     else                     point1.setX( dst->p1().x() );

     index = myindex+1;

     wire = new WireSegment( QLineF( point1, dst->p2() ), this, m_parent );
     addWire( wire, index );
    }

  ElementJoint *joint = static_cast<ElementJoint *>(view()->createElement( "joint", point1 ));
  ElementJointPort *ports[3];
  for( int i=0; i<3; i++ )
    {
      ports[i] = static_cast<ElementJointPort *>(view()->createElement( "jointport", point1 ));
    }
  joint->setPorts( ports );
  joint->moveJoint( point1 );

  split( index, joint->port(0), joint->port(2) );
  return joint->port(1);
}

void ElementWire::removeNullWires( WireSegment *exp )
{
  if( m_wires.length() < 2 ) return;

  foreach( WireSegment* wire, m_wires )
    {
      int index = m_wires.indexOf( wire );
      if( index < m_wires.length()-1 )
        {
          WireSegment* wire2 = m_wires.at( index+1 );

          if( wire->dx() == wire2->dx() || wire->dy() == wire2->dy() )
            {
              wire2->setPlainP1( wire->p1() );
              trace_assert( wire != exp );
              removeWire( wire );
            }
        }
    }
  if( m_wires.length() < 2 )
    {
      m_lastindex = 0;
      m_activeLine = 0;
    }
}

/**
 * note! ElementWire will delete the wire if it became unused
 */
void ElementWire::removeWire( WireSegment *wire )
{
  int index = m_wires.indexOf( wire );

  disconnectWires( index, index-1 );
  disconnectWires( index, index+1 );
  connectWires( index-1, index+1 );

  m_wires.removeOne( wire );
  delete wire;
}

void ElementWire::deleteAll()
{
  while( !m_wires.isEmpty() )
    {
      WireSegment* wire = m_wires.takeLast();
      ElementBase::scene()->removeItem( wire );
      delete wire;
    }
}

void ElementWire::connectWires( int dst, int src )
{
  if( dst < 0 || src < 0 || src > m_wires.length()-1 )
    return;

  WireSegment* wire1 = m_wires.at( dst );
  WireSegment* wire2 = m_wires.at( src );

  QObject::connect( wire1, SIGNAL( sigSetPlainP1( const QPointF& ) ), wire2, SLOT( setPlainP1( const QPointF& ) ) );
  QObject::connect( wire2, SIGNAL( sigSetPlainP2( const QPointF& ) ), wire1, SLOT( setPlainP2( const QPointF& ) ) );
}

void ElementWire::disconnectWires( int dst, int src )
{
  if( dst < 0 || src < 0 || src > m_wires.length()-1 )
      return;

  WireSegment* wire1 = m_wires.at( dst );
  WireSegment* wire2 = m_wires.at( src );

  QObject::disconnect( wire1, SIGNAL( sigSetPlainP1( const QPointF& ) ), wire2, SLOT( setPlainP1( const QPointF& ) ) );
  QObject::disconnect( wire2, SIGNAL( sigSetPlainP2( const QPointF& ) ), wire1, SLOT( setPlainP2( const QPointF& ) ) );
}

void ElementWire::split( int index, ElementAbstractPort* port1, ElementAbstractPort* port2 )
{
  if( !m_endPort ) return;

  disconnectWires( index-1, index );

  ElementWire *new_wire = static_cast<ElementWire *>(view()->createElement( "wire", QPoint( 0, 0 ) ));
  new_wire->connectStartPort( port2 );

  int newindex = 0;
  int size = m_wires.size();
  for( int i = index; i < size; ++i)
    {
      WireSegment* ws = m_wires.takeAt( index );
      new_wire->wireSegments()->insert( newindex, ws );

      ws->setParentItem( new_wire->parentItem() );
      ws->setElementWire( new_wire );

      if( newindex > 1 ) new_wire->addActiveWire();
      ++newindex;
    }

  if( index > 1 )  m_activeLine = index-2;
  else             m_activeLine = 0;

  m_endPort->disconnectWire( this, /*boardcast*/ false );
  new_wire->connectEndPort( m_endPort );  // Close new_wire first please
  connectEndPort( port1 );                // Close this
}

void ElementWire::move( QPointF delta )
{
  foreach( WireSegment *wire, m_wires )
    {
      wire->move( delta );
    }
}

void ElementWire::layoutWires( ElementAbstractPort *port, const QPointF &pos )
{
  int length = m_wires.length();
  WireSegment* wire;
  WireSegment* prewire = 0l;

  if( port == m_startPort )
    {
      wire = m_wires.first();
      wire->setPlainP1( pos.toPoint() );
      m_lastindex = 0;

      if( length > 1 )
        {
          prewire = m_wires.at(1);
          m_activeLine = 1;
        }
      else m_activeLine = 0;
    }
  else
    {
      wire = m_wires.last();
      wire->setPlainP2( togrid( pos ).toPoint() );
      m_lastindex = length-1;

      if( length > 1 )
        {
          prewire = m_wires.at( m_lastindex-1 );
          if( port != 0l ) m_activeLine = m_lastindex-1;
        }
    }

  if( wire->dx() == 0 && wire->dy() == 0 && length > 1 )
    {
      removeWire( wire );

      if( m_activeLine > 0 )  m_activeLine -= 1;
    }
  else if( wire->dx() != 0 && wire->dy() != 0 )
    {
      QPointF point;

      if( m_lastindex == m_activeLine )          // Add new corner
        {
          point = wire->p2();

          if( abs(wire->dx()) > abs(wire->dy()) ) point.setY( wire->p1().y() );
          else                                    point.setX( wire->p1().x() );

          WireSegment* newire = addWire( QLineF(point.x(), point.y(), wire->p2().x(), wire->p2().y()), m_lastindex + 1 );

          if( wire->isSelected() ) newire->setSelected( true );

          wire->setP2( point );
        }
      else if( m_lastindex < m_activeLine )      // Update the first corner
        {
          point = wire->p2();

          if( prewire->dx() == 0 ) point.setY( wire->p1().y() );
          else                     point.setX( wire->p1().x() );

          wire->setP2( point );

          if( wire->dx() == prewire->dx() || wire->dy() == prewire->dy() ) // Lines aligned or null line
            {
              if( wire->isSelected() || prewire->isSelected())
                {
                  prewire->setPlainP1( wire->p1() );
                  removeWire( wire  );
                  if( m_activeLine > 0 )  m_activeLine -= 1;
                }
            }
        }
      else                                    // Update the last corner
        {
          point = wire->p1();

          if( prewire->dx() == 0 ) point.setY( wire->p2().y() );
          else                     point.setX( wire->p2().x() );

          wire->setP1( point );

          if( wire->dx() == prewire->dx() || wire->dy() == prewire->dy() ) // Lines aligned or null line
          {
              if( wire->isSelected() || prewire->isSelected())
              {
                  prewire->setPlainP2( wire->p2() );
                  removeWire( wire  );
                  if( m_activeLine > 0 )  m_activeLine -= 1;
              }
          }
        }
    }
}

WireSegment *ElementWire::wireAt( const QPointF &scenePos )
{
  QList<QGraphicsItem *> list = ElementBase::scene()->items( scenePos );
  foreach( QGraphicsItem *current, list )
    {
      WireSegment *wire = qgraphicsitem_cast<WireSegment *>(current);
      if( wire ) return wire;
    }
  return 0l;
}

QList<qreal> ElementWire::pointList()
{
  QList<qreal> list;
  if ( m_wires.isEmpty() ) return list;

  QString data;

  list.append( m_wires.at(0)->p1().x() );
  list.append( m_wires.at(0)->p1().y() );
  int count = m_wires.size();
  for( int i=0; i<count; i++ )
    {
      list.append( m_wires.at(i)->p2().x() );
      list.append( m_wires.at(i)->p2().y() );
    }
  return list;
}

QRectF ElementWire::boundingRect() const
{
  return shape().controlPointRect();
}
QPainterPath ElementWire::shape() const
{
  QPainterPath pathsum;

  foreach( QGraphicsLineItem *wire, m_wires )
    {
      pathsum += wire->shape();
    }

  return pathsum;
}

int ElementWire::resolveSubElements()
{
  if( m_startPortId >= 0 )
    {
      if( m_endPortId < 0 ) return -DS_INVALID_ELEMENT_ID;
      
      ElementBase *startPortBase = view()->element( m_startPortId );
      ElementBase *endPortBase = view()->element( m_endPortId );
      
      if( 0l==startPortBase || 0l==endPortBase )
        return -DS_INVALID_ELEMENT_ID;

      ElementAbstractPort *startPort = elementabstractport_cast( startPortBase );
      ElementAbstractPort *endPort = elementabstractport_cast( endPortBase );
      if( startPort && endPort )
        {
          connectStartPort( startPort );
          connectEndPort( endPort, /*deser*/ true );
        }
      else
        return -DS_INVALID_ELEMENT_ID;
    }
  return 0;
}

int ElementWire::serialize( LispDataset *dataset )
{
  int rc = ElementBase::serialize( dataset );               UPDATE_RC(rc);

  if( m_startPort )
    { rc = dataset->ser( m_startPort->parentId() );         UPDATE_RC(rc); }
  else
    { rc = dataset->ser( -1 );                              UPDATE_RC(rc); }
  if( m_endPort )
    { rc = dataset->ser( m_endPort->parentId() );           UPDATE_RC(rc); }
  else
    { rc = dataset->ser( -1 );                              UPDATE_RC(rc); }

  rc = dataset->beginNode(true, "pts");                     UPDATE_RC(rc);

  rc = dataset->beginNode(true);                            UPDATE_RC(rc);
  rc = dataset->ser( double( m_wires.at(0)->p1().x() ) );   UPDATE_RC(rc);
  rc = dataset->ser( double( m_wires.at(0)->p1().y() ) );   UPDATE_RC(rc);
  rc = dataset->endNode(true);                              UPDATE_RC(rc);

  int count = m_wires.size();
  for(int i=0; i<count; i++)
    {
      rc = dataset->beginNode(true);                          UPDATE_RC(rc);
      rc = dataset->ser( double( m_wires.at(i)->p2().x() ) ); UPDATE_RC(rc);
      rc = dataset->ser( double( m_wires.at(i)->p2().y() ) ); UPDATE_RC(rc);
      rc = dataset->endNode(true);                            UPDATE_RC(rc);
    }

  rc = dataset->endNode(true);                              UPDATE_RC(rc);
  return rc;
}

int ElementWire::deserialize( LispDataset *dataset )
{
  int rc = ElementBase::deserialize( dataset );         UPDATE_RC(rc);

  rc = dataset->des( m_startPortId );                   UPDATE_RC(rc);
  rc = dataset->des( m_endPortId );                     UPDATE_RC(rc);

  DomEntry entry;
  double x, y;
  bool x0 = true;
  QPointF p1;
  QPointF p2;

  rc = dataset->des( entry, "pts" );                    UPDATE_RC(rc);

  for( entry.begin(); entry.valid(); ++entry )
    {
      dataset->deserializePush( entry.child() );
        {
          LispDataset::AutoPop autoPop( dataset );

          rc = dataset->des( x );                       UPDATE_RC(rc);
          rc = dataset->des( y );                       UPDATE_RC(rc);

          if( UNLIKELY(x0) )
            {
              p1 = QPointF(x, y);
              x0 = false;
            }
          else
            {
              p2 = QPointF(x, y);
              addWire( QLineF(p1, p2), m_wires.length() );
              p1 = p2;
            }
        } // auto pop
    }

  removeNullWires();
  return rc;
}

}

#include "elementwire.moc"
