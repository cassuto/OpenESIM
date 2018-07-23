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

#ifndef ELEMENTWIRE_H_
#define ELEMENTWIRE_H_

#include "elementbase.h"

namespace dsim
{

class ElementWire;
class ElementAbstractPort;

class WireSegment : public QGraphicsLineItem
{
public:
  WireSegment( const QLineF &line, ElementWire *wire, QGraphicsItem* parent = 0 );
  ~WireSegment();

  void ref();
  void release();
  void connectP1( WireSegment *wire );
  void connectP2( WireSegment *wire );
  void disconnectP1( WireSegment *wire );
  void disconnectP2( WireSegment *wire );
  void setLine( const QLineF &line );
  void setP1( const QPointF &point );
  void setP2( const QPointF &point );
  void setPlainP1( const QPointF &point );
  void setPlainP2( const QPointF &point );
  void moveWire( const QPointF &delta );
  void move( QPointF delta );
  void setElementWire( ElementWire *wire ) { m_elementWire = wire; }
  inline QPointF p1() { return line().p1(); }
  inline QPointF p2() { return line().p2(); }
  inline qreal dx() { return line().p2().x() - line().p1().x(); }
  inline qreal dy() { return line().p2().y() - line().p1().y(); }
  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

protected:
  void mousePressEvent( QGraphicsSceneMouseEvent *event );
  void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
  void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
  void hoverEnterEvent( QGraphicsSceneHoverEvent *event );
  void hoverLeaveEvent( QGraphicsSceneHoverEvent *event );
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  void setConnectedP1( const QPointF &p1 );
  void setConnectedP2( const QPointF &p2 );

private:
  int                  m_refcount;
  QList<WireSegment *> m_connectedWiresP1;
  QList<WireSegment *> m_connectedWiresP2;
  ElementWire         *m_elementWire;
  bool                 m_selected;

public:
  enum { Type = UserType + 3868 /*magic*/ };
  int type() const { return Type; }
};

class ElementPin;

class ElementWire : public ElementBase, public QGraphicsItem
{
public:
  ElementWire( int id, SchemaScene *scene, QGraphicsItem *parent = 0l );
  ~ElementWire();
  const char *classname() { return "wire"; }

  void connectStartPort( ElementAbstractPort *port );
  void connectEndPort( ElementAbstractPort *port );
  void disconnectPort( ElementAbstractPort *port );
  inline ElementAbstractPort *startPort() { return m_startPort; }
  inline ElementAbstractPort *endPort() { return m_endPort; }

  void addWire( WireSegment* wire, int index );
  WireSegment* addWire( const QLineF &line, int index );
  void addActiveWire();
  ElementAbstractPort *addJoint( const QPointF &scenePos );
  void removeNullWires();
  void removeWire( WireSegment *wire );
  void deleteAll();
  void connectWires( int dst, int src );
  void disconnectWires( int dst, int src );
  void split( int index, ElementAbstractPort* port1, ElementAbstractPort* port2 );
  void move( QPointF delta );
  void layoutWires( ElementAbstractPort *port, const QPointF &pos );
  WireSegment *wireAt( const QPointF &scenePos );
  inline QList<WireSegment *> *wireSegments() { return &m_wires; }
  inline QGraphicsItem *parentItem() const { return m_parent; }

  QRectF boundingRect() const;
  QPainterPath shape() const;
  void addToScene( QGraphicsScene *scene );
  void removeFromScene( QGraphicsScene *scene );

  int resolveSubElements();
  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

protected:
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  QGraphicsItem       *m_parent;
  QList<WireSegment *> m_wires;
  ElementAbstractPort *m_startPort;
  ElementAbstractPort *m_endPort;
  int                  m_lastindex;
  int                  m_activeLine;
  int                  m_startPortId, m_endPortId; // for deserialize() and resolveSubElements() only
};


}

#endif
