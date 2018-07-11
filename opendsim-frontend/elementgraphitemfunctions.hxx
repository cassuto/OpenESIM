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

#ifndef ELEMENTGRAPHITEM_H_
# include "elementgraphitem.h"
#endif

#ifndef ELEMENTGRAPHITEMFUNCTIONS_H_
#define ELEMENTGRAPHITEMFUNCTIONS_H_

#include "schemaview.h"
#include "schemasheet.h"
#include "schemagraph.h"
#include "templatestyle.h"

namespace dsim
{

template <class base>
  ElementGraphItem<base>::ElementGraphItem( int id, SchemaGraph *scene, bool editable, base *parent )
               : ElementBase( id, scene )
               , base( parent )
               , m_editable( editable )
               , m_direct( ELEM_LEFT )
  {
    setGraphicsItem( this );
    base::setData( 0, QVariant::fromValue( static_cast<void *>(static_cast<ElementBase *>(this)) ) );
  }

template <class base>
  void ElementGraphItem<base>::setStyle( const char *style )
  { m_style = style; }

template <class base>
  void ElementGraphItem<base>::setDirect( ElemDirect direct )
  { m_direct = direct; }

template <class base>
  void ElementGraphItem<base>::setSelected( bool selected )
  {
    setSelected( selected );

    foreach( ElementBase *element, elements() )
      {
        element->graphicsItem()->setSelected( selected );
      }
  }

template <class base>
  void ElementGraphItem<base>::setLayout()
  {
  }

template <class base>
  void ElementGraphItem<base>::paintBound( QPainter *painter )
  {
    QPen pen = painter->pen();
    painter->setPen( QPen( Qt::red, 0.5 ) );
    painter->drawRect( base()->boundingRect() );
    painter->setPen( pen );
  }

template <class base>
  void ElementGraphItem<base>::addToScene( QGraphicsScene *scene )
  { scene->addItem( this ); }

template <class base>
  void ElementGraphItem<base>::removeFromScene( QGraphicsScene *scene )
  { scene->removeItem( this ); }

template <class base>
  int ElementGraphItem<base>::serialize( LispDataset *dataset )
  {
    int rc = ElementBase::serialize( dataset );           UPDATE_RC(rc);
        rc = dataset->ser( int(m_direct) );               UPDATE_RC(rc);
        rc = dataset->ser( m_style );                     UPDATE_RC(rc);
    return rc;
  }

template <class base>
  int ElementGraphItem<base>::deserialize( LispDataset *dataset )
  {
    int direct;
    std::string style;

    int rc = ElementBase::deserialize( dataset );         UPDATE_RC(rc);
        rc = dataset->des( direct );                      UPDATE_RC(rc);
        rc = dataset->des( style );                       UPDATE_RC(rc);

    setDirect( (ElemDirect)direct );
    setStyle( style.c_str() );

    return rc;
  }

template <class base>
  void ElementGraphItem<base>::setVisible( bool visible )
  {
    base::setVisible( visible );

    foreach( ElementBase *element, elements() )
      {
        element->graphicsItem()->setVisible( visible );
      }
  }

template <class base>
  void ElementGraphItem<base>::move( QPointF delta )
  {
    base::setPos( base::scenePos() + delta );

    foreach( ElementBase *element, elements() )
      {
        QGraphicsItem *item= element->graphicsItem();
        if( !item->isSelected() )
          item->setPos( item->scenePos() + delta );
      }
  }

}

#endif
