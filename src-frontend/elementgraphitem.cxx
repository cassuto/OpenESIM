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
#include "templatestyle.h"
#include <cstdio>
#include "schemagraph.h"

namespace dsim
{

ElementGraphItem::ElementGraphItem( int id, bool editable, SchemaGraph *schgraph, QGraphicsItem *parent )
             : QGraphicsItem( parent )
             , m_id( id )
             , m_schgraph( schgraph )
             , m_editable( editable )
             , m_moving( false )
             , m_direct( ELEM_LEFT )
             , m_ref( false )
{
  setFlag( QGraphicsItem::ItemStacksBehindParent, true );
  setFlag( QGraphicsItem::ItemIsSelectable, true );
}

void ElementGraphItem::setStyle( const char *style )
{ m_style = style; }

void ElementGraphItem::setDirect( ElemDirect direct )
{ m_direct = direct; }

void ElementGraphItem::setSelected( bool selected )
{
  QGraphicsItem::setSelected( selected );

  foreach( ElementGraphItem *item, m_elements )
    {
      item->setSelected( selected );
    }
}

void ElementGraphItem::setLayout()
{
}

void ElementGraphItem::setBoundingRect( const QRect &area )
{ prepareGeometryChange(); m_area = area; }

void ElementGraphItem::paintBound( QPainter *painter )
{
  QPen pen = painter->pen();
  painter->setPen( QPen( Qt::red, 0.5 ) );
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
      rc = dataset->ser( m_elements );        UPDATE_RC(rc);
  return rc;
}

int ElementGraphItem::deserialize( DomDataset *dataset )
{
  double x, y;
  int direct;
  std::string style;

  int rc = dataset->des( m_id );    UPDATE_RC(rc);
      rc = dataset->des( x );       UPDATE_RC(rc);
      rc = dataset->des( y );       UPDATE_RC(rc);
      rc = dataset->des( direct );  UPDATE_RC(rc);
      rc = dataset->des( style );   UPDATE_RC(rc);
      rc = dataset->des( m_elementIds ); UPDATE_RC(rc);

  setPos( QPointF(x, y) );

  if( !view()->idUnused( m_id ) )
    {
      m_id = 0;
      return -DS_INVALID_ELEMENT_ID;
    }
  view()->addId( m_id );

  setDirect( (ElemDirect)direct );
  setStyle( style.c_str() );

  return rc;
}

int ElementGraphItem::resolveSubElements()
{
  if( m_elementIds.empty() ) return 0;

  foreach( int id, m_elementIds )
    {
      ElementGraphItem *element = view()->element( id );
      if( element )
        {
          if( element->ref() )
            {
              m_elements.append( element );
            }
          else
            {
              return -DS_REREFERENCE;
            }
        }
      else
        {
          return -DS_SOLVE_SUBELEMENTS;
        }
    }

  m_elementIds.clear();
  setLayout();
  return 0;
}

bool ElementGraphItem::ref()
{ if( m_ref) return false; m_ref = true; return true; }
bool ElementGraphItem::isRef()
{ return m_ref; }

void ElementGraphItem::setVisible( bool visible )
{
  QGraphicsItem::setVisible( visible );

  foreach( ElementGraphItem *item, m_elements )
    {
      item->setVisible( visible );
    }
}

const QList<ElementGraphItem*> &ElementGraphItem::elements() const
{
  return m_elements;
}

void ElementGraphItem::applyStyle( QPainter *painter, const char *style, bool selected )
{
  if( !TemplateStyle::instance()->isStyle( style ) )
    return;

  StyleItem s = TemplateStyle::instance()->fillStyle( style, selected );

  QBrush brush = painter->brush();
  if( s.usebkcolor )
    {
      brush.setStyle( Qt::NoBrush );
    }
  else
    {
      brush.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
    }

  s = TemplateStyle::instance()->lineStyle( style, selected );

  QPen pen = painter->pen();
  pen.setStyle( TemplateStyle::toQtPenStyle( s.line ) );
  if( s.line != LINE_NONE)
    {
      pen.setWidthF( s.width );
      pen.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
    }

  painter->setBrush( brush );
  painter->setPen( pen );
}


void ElementGraphItem::applyTextStyle( QGraphicsSimpleTextItem *text, const char *style, bool selected )
{
  if( !TemplateStyle::instance()->isStyle( style ) )
    return;

  StyleItem s = TemplateStyle::instance()->textStyle( style, selected );

  QFont font = text->font();
  font.setWeight( s.weight );
  font.setBold( s.bold );
  font.setItalic( s.italic );
  text->setFont( font );

  QBrush brush = text->brush();
  brush.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
  text->setBrush( brush );
}

int ElementGraphItem::addElement( ElementGraphItem *item )
{
  if( item->ref() )
    {
      m_elements.append( item );
      return 0;
    }
  else
    {
      return -DS_REREFERENCE;
    }
}

void ElementGraphItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  if( m_editable )
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
  if( m_editable )
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
  if( m_editable )
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
      if( !element->isSelected() )
        element->setPos( element->scenePos() + delta );
    }
}

}
