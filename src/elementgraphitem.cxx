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
#include "elementgraphitem.h"
#include "schemaview.h"
#include "schemasheet.h"

#include "schemagraph.h"

namespace dsim
{

ElementGraphItem::ElementGraphItem( int id, SchemaGraph *schgraph, QGraphicsItem *parent )
             : QGraphicsItem( parent )
             , m_id( id )
             , m_schgraph( schgraph )
             , m_edit( false )
             , m_moving( false )
             , m_direct( ELEM_LEFT )
             , m_elementIds( 0l )
{
}

void ElementGraphItem::setStyle( const char *style )
{ m_style = style; }

void ElementGraphItem::setDirect( ElemDirect direct )
{ m_direct = direct; }

void ElementGraphItem::setEdit( bool edit )
{ m_edit = edit; }

void ElementGraphItem::setLayout()
{
}

void ElementGraphItem::paintBound( QPainter *painter )
{
  QPen pen = painter->pen();
  painter->setPen( QPen( Qt::blue, 0.5 ) );
  painter->drawRect( boundingRect() );
  painter->setPen( pen );
}

int ElementGraphItem::serialize( DomDataset *dataset )
{
  int rc = dataset->ser( m_id );              UPDATE_RC(rc);
      rc = dataset->ser( double(pos().x()) ); UPDATE_RC(rc);
      rc = dataset->ser( double(pos().y()) ); UPDATE_RC(rc);
      rc = dataset->ser( int(m_direct) );     UPDATE_RC(rc);
      rc = dataset->ser( m_style );           UPDATE_RC(rc);
      rc = dataset->ser( int(m_elements.count()) ); UPDATE_RC(rc);

  for( int i=0; i < m_elements.count(); i++ )
    {
      rc = dataset->ser( int(m_elements.at(i)->id()) ); UPDATE_RC(rc);
    }
  return rc;
}

int ElementGraphItem::deserialize( DomDataset *dataset )
{
  double x, y;

  int rc = dataset->des( m_id );    UPDATE_RC(rc);
      rc = dataset->des( x );       UPDATE_RC(rc);
      rc = dataset->des( y );       UPDATE_RC(rc);
  setPos( QPointF(x, y) );

  if( !view()->sheet()->idUnused( m_id ) )
    {
      m_id = 0;
      return -DS_INVALID_ELEMENT_ID;
    }
  view()->sheet()->addId( m_id );

  int direct;
  rc = dataset->des( direct );      UPDATE_RC(rc);
  setDirect( (ElemDirect)direct );

  std::string style;
  rc = dataset->des( style );       UPDATE_RC(rc);
  setStyle( style.c_str() );

  int count, id;
  rc = dataset->des( count );       UPDATE_RC(rc);

  if( count )
    {
      m_elementIds = new QList<int>;

      for( int i=0; i < count; i++ )
        {
          rc = dataset->des( id ); UPDATE_RC(rc);

          m_elementIds->append( id );
        }
    }
  else
    m_elementIds = 0l;
  return rc;
}

int ElementGraphItem::resolveSubElements()
{
  if( !m_elementIds ) return 0;

  foreach( int id, *m_elementIds )
    {
      ElementGraphItem *element = view()->element( id );
      if( element )
        {
          m_elements.append( element );
        }
      else
        {
          return -DS_SOLVE_SUBELEMENTS;
        }
    }

  delete m_elementIds;
  return 0;
}

void ElementGraphItem::addDOM( DomDataset *dataset )
{
  dataset->addItem( this );

  foreach( ElementGraphItem *item, m_elements )
    {
      dataset->addItem( item );
    }
}

void ElementGraphItem::setVisible( bool visible )
{
  QGraphicsItem::setVisible( visible );

  foreach( ElementGraphItem *item, m_elements )
    {
      item->setVisible( visible );
    }
}

void ElementGraphItem::addElement( ElementGraphItem *item )
{
  m_elements.append( item );
}

void ElementGraphItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  if( m_edit )
    {
      if( event->button() == Qt::LeftButton )
        {
          event->accept();

          if( !this->isSelected() )     // Deselect the other elements
            {
              QList<QGraphicsItem*> itemlist = m_schgraph->selectedItems();

              foreach( QGraphicsItem* item, itemlist ) item->setSelected( false );

              this->setSelected( true );
            }

          grabMouse();
        }
    }
}

void ElementGraphItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
  if( m_edit )
    {
      event->accept();

      QList<QGraphicsItem*> itemlist = m_schgraph->selectedItems();
      if ( !itemlist.isEmpty() )
        {
          if( !m_moving )
            {
              m_moving = true;
            }

          QPointF delta = togrid(event->scenePos()) - togrid(event->lastScenePos());

          foreach( QGraphicsItem* item , itemlist )
            {
              ElementGraphItem* element =  qgraphicsitem_cast<ElementGraphItem* >( item );
              if( element ) element->move( delta );
            }
        }
    }
}

void ElementGraphItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  if( m_edit )
    {
      event->accept();
      ungrabMouse();
      m_moving = false;
    }
}

void ElementGraphItem::move( QPointF delta )
{
  setPos( scenePos() + delta );

  foreach( ElementGraphItem *element, m_elements )
    {
      element->setPos( element->scenePos() + delta );
    }
}

}
