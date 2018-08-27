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
#include "schemascene.h"
#include "domitem.h"

class QGraphicsItem;

namespace dsim
{

#ifdef NDEBUG
# undef CHECK_ELEMENTBASE_MAGIC
#else
# define CHECK_ELEMENTBASE_MAGIC 1
# define ELEMENTBASE_MAGIC (0x23892749)
#endif

class SchemaView;

class ElementBase: public DomItem
{
public:
  ElementBase( int id, SchemaScene *scene );
  virtual ~ElementBase();
  virtual const char *classname() const = 0;

  bool          ref();
  bool          isRoot();
  inline void   setNonRoot( bool nonRoot ) { m_root = !nonRoot; }
  inline int    refcount() { return m_refcount; }
  void          release();
  virtual void  move( QPointF delta );
  virtual int   addElement( ElementBase *element );
  virtual int   resolveSubElements();
  virtual void  releaseSubElements();
  virtual void  addToScene( QGraphicsScene *scene )=0;
  virtual void  removeFromScene( QGraphicsScene *scene )=0;
  virtual void  setLayout() {}
  QList<ElementBase*> & elements() { return m_elements; }

  virtual QRectF boundingRect() const;

  virtual int serialize( LispDataset *dataset );
  virtual int deserialize( LispDataset *dataset );

public:
  inline int    id() const { return m_id; }
  virtual void   resetId( int id ) { m_id = id; }
  inline SchemaView *view() const { return m_schemaGraph->view(); }
  inline SchemaScene *scene() const { return m_schemaGraph; }
  inline QGraphicsItem *graphicsItem() const { return m_graphicsItem; }

protected:
  void setGraphicsItem( QGraphicsItem *item ) { m_graphicsItem = item; }

private:
  int                       m_refcount;
  int                       m_id;
  QList<ElementBase*>       m_elements;
  QList<int>                m_elementIds;
  SchemaScene              *m_schemaGraph;
  QGraphicsItem            *m_graphicsItem;
  bool                      m_root;
#ifdef CHECK_ELEMENTBASE_MAGIC
  int                       m_magic;
#endif
};

}

#include "elementbasecast.hxx"

#endif
