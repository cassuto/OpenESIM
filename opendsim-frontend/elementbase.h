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

#ifndef ELEMENTBASE_H_
#define ELEMENTBASE_H_

#include <QList>
#include <device/graph.h>
#include "schemagraph.h"
#include "domitem.h"

class QGraphicsItem;

namespace dsim
{

class SchemaView;

class ElementBase: public DomItem
{
public:
  ElementBase( int id, SchemaGraph *scene );
  virtual ~ElementBase() {}
  virtual const char *classname() { return "none"; }

  bool          ref();
  bool          isRef();
  virtual int   addElement( ElementBase *element );
  virtual int   resolveSubElements();
  virtual void  addToScene( QGraphicsScene *scene )=0;
  virtual void  removeFromScene( QGraphicsScene *scene )=0;
  virtual void  setLayout() {}
  const QList<ElementBase*> &elements() const;

  virtual int serialize( LispDataset *dataset );
  virtual int deserialize( LispDataset *dataset );

public:
  inline int    id() const { return m_id; }
  inline void   resetId( int id ) { m_id = id; }
  inline SchemaView *view() const { return m_schemaGraph->view(); }
  inline SchemaGraph *scene() const { return m_schemaGraph; }
  inline QGraphicsItem *graphicsItem() const { return m_graphicsItem; }

protected:
  void setGraphicsItem( QGraphicsItem *item ) { m_graphicsItem = item; }

private:
  int                       m_id;
  QList<ElementBase*>       m_elements;
  QList<int>                m_elementIds;
  bool                      m_ref;
  SchemaGraph              *m_schemaGraph;
  QGraphicsItem            *m_graphicsItem;
};

}

#endif
