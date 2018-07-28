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

#ifndef ELEMENTJOINT_H_
#define ELEMENTJOINT_H_

#include "elementgraphitem.h"
#include "elementabstractport.h"

namespace dsim
{

class ElementJoint;
class ElementJointPort : public ElementAbstractPort, public ElementBase
{
public:
  ElementJointPort( int id, SchemaScene *scene );
  virtual ~ElementJointPort();
  const char *classname() const { return "jointport"; }
  void addToScene( QGraphicsScene *scene ) { UNUSED(scene); }
  void removeFromScene( QGraphicsScene *scene ) { UNUSED(scene); }

  QString reference() const;
  QPointF portScenePos() const;
  int parentId() const { return id(); }
  void disconnectedEvent();

protected:
  void setScenePos( const QPointF &pos );
  void setElementJoint( ElementJoint *joint );

private:
  QPointF m_scenePos;
  ElementJoint *m_elementJoint;

  friend class ElementJoint;
};

class ElementJoint : public ElementGraphItem<QGraphicsEllipseItem>
{
public:
  ElementJoint( const QPointF &pos, int id, SchemaScene *scene, QGraphicsItem* parent = 0 );
  const char *classname() const { return "joint"; }

  int setPorts( ElementJointPort **ports );
  void moveJoint( const QPointF &pos );
  ElementJointPort *port( int index );
  void remove();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );
  int resolveSubElements();

protected:
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  void joinWires( ElementAbstractPort* port0, ElementAbstractPort* port1 );

};

}

#endif
