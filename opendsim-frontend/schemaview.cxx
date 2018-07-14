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

#include <QtGui>
#include <cstring>
#include <cstdio>
#include <dsim/error.h>
#include "schemasheet.h"
#include "schemagraph.h"
#include "lispdataset.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementline.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"
#include "elementellipse.h"

#include "schemaview.h"

namespace dsim
{

SchemaView::SchemaView( SchemaSheet *sheet, QWidget *parent )
          : QGraphicsView( parent )
          , m_sheet( sheet )
          , stack( 0 )
          , m_selectedElements( 0l )
{
  m_schemaGraph = 0;
  m_scalefactor = 1;

  clear();

  viewport()->setFixedSize( 1600, 1200 );
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setViewportUpdateMode( QGraphicsView::MinimalViewportUpdate );
  setRenderHint( QPainter::HighQualityAntialiasing );
  setCacheMode( CacheBackground );
  setTransformationAnchor( AnchorUnderMouse );
  setResizeAnchor( AnchorUnderMouse );
  setDragMode( QGraphicsView::RubberBandDrag );

  setAcceptDrops( true );

  m_editable[0] = true;
  m_editable[1] = false;

  /*
   * schemaviewmenus.cxx related construction
   */
  createContextMenus();

  /*
   * schemaviewfunctions.cxx related construction
   */
  m_hintElement = 0l;
  m_hintComponent = 0l;
  m_hintDirect = ELEM_LEFT;
  m_hintCount = 0;
  m_moving = false;
  m_mousePressEvent = 0l;
  m_mouseMoveEvent = 0l;
  m_resetEvent = 0l;

  setMode( MODE_SELECTION );
}

SchemaView::~SchemaView() { }

void SchemaView::clear()
{
  if( m_schemaGraph )
    {
      m_schemaGraph->remove();
      m_schemaGraph->deleteLater();
    }

  clearStack( 0 );
  clearStack( 1 );

  m_schemaGraph = new SchemaGraph( -1600, -1200, 3200, 2400, this );
  setScene( m_schemaGraph );
  centerOn( 300, 200 );
}


ElementBase *SchemaView::createElement( const char *classname, const QPointF &pos, bool editable, bool deser )
{
  int id = 0;
  ElementBase *element = 0l;

  if( !deser ) id = m_id[stack].alloc();

  if( 0==std::strcmp( classname, "component" ) )
    {
      ComponentGraphItem *component = new ComponentGraphItem( id, m_schemaGraph, editable );
      element = component;
    }
  if( 0==std::strcmp( classname, "pin" ) )
    {
      ElementPin *pinElement = new ElementPin( (ElemDirect)m_hintDirect, pos, 0l, 0l, id, m_schemaGraph, editable );
      element = pinElement;
    }
  else if( 0==std::strcmp( classname, "line" ) )
    {
      ElementLine *lineElement = new ElementLine( pos, id, m_schemaGraph, editable );
      element = lineElement;
    }
  else if( 0==std::strcmp( classname, "text" ) )
    {
      ElementText *textElement = new ElementText( (ElemDirect)m_hintDirect, pos, id, m_schemaGraph, editable );
      textElement->setText( "TEXT 1" );
      element = textElement;
    }
  else if( 0==std::strcmp( classname, "rect" ) )
    {
      ElementRect *rectElement = new ElementRect( QRectF( pos, pos ), id, m_schemaGraph, editable );
      element = rectElement;
    }
  else if( 0==std::strcmp( classname, "ellipse" ) )
    {
      ElementEllipse *ellipseElement = new ElementEllipse( QRectF( pos, pos ), id, m_schemaGraph, editable );
      element = ellipseElement;
    }

  element->addToScene( m_schemaGraph );
  m_elements[stack].append( element );
  return element;
}

void SchemaView::removeElement( ElementBase *element )
{
  if( std::strcmp( element->classname(), "component") == 0 )
    {
      ComponentGraphItem *component = static_cast<ComponentGraphItem *>(element);
      m_sheet->deleteDevice( component->device() );
    }

  foreach( ElementBase *subelm, element->elements() )
    {
      m_elements[stack].removeOne( subelm );
      m_id[stack].release( subelm->id() );
      subelm->removeFromScene( m_schemaGraph );
    }

  m_elements[stack].removeOne( element );
  m_id[stack].release( element->id() );
  element->removeFromScene( m_schemaGraph );
}

int SchemaView::serialize( LispDataset *dom )
{
  int rc = 0;

  dom->beginEntry( "elements" );
  {
    foreach( ElementBase *element, m_elements[stack] )
      {
        rc = dom->beginNode();                  UPDATE_RC(rc);

        rc = dom->ser( element->classname(), /*symbol*/true );  UPDATE_RC(rc);

        rc = element->serialize( dom );         UPDATE_RC(rc);

        rc = dom->endNode();                    UPDATE_RC(rc);
      }
  }
  dom->endEntry();

  return 0;
}

int SchemaView::deserialize( LispDataset *dom )
{
  int rc = 0;
  std::string symbol;

  clear();

  /*
   * Deserialize Elements
   */
  DomEntry nodes = dom->entry( 0, "elements" );
  {
    for( nodes.begin(); nodes.valid(); ++nodes )
      {
        dom->deserializePush( nodes.child() );
        {
          LispDataset::AutoPop autoPop( dom );

          rc = dom->des( symbol, /*symbol*/true ); UPDATE_RC(rc);

          const char *classname = symbol.c_str();
          ElementBase *elem = createElement( classname, QPoint(0, 0), m_editable[stack], /*deser*/ true );
          if( elem )
            {
              dom->deserializePush( dom->current() );
              {
                LispDataset::AutoPop autoPop( dom );

                if( (rc = elem->deserialize( dom )) )
                  {
                    removeElement( elem ); // fault
                  }
              } // auto pop
            }
          else
            rc = -DS_INVALID_ELEMENT_SYMBOL;
        } // auto pop

        if( rc ) break;
      }
  }

  /*
   * Resolve the sub elements of each one
   */
  if( 0==rc )
    {
      foreach( ElementBase* element, m_elements[stack] )
        {
          if( (rc = element->resolveSubElements()) )
            break;
        }
    }

  return rc;
}

ComponentGraphItem *SchemaView::loadSymbol( std::ifstream &instream )
{
  int rc = 0;

  clearStack( 1 );

  /*
   * Parse ssymbol file
   */
  stack = 1;
  LispDataset *dom = new LispDataset( DOM_SCHEMA_SYMBOL );

  rc = dom->init();                     if( rc ) return 0l;
       dom->addItem( this );
  rc = dom->deserialize( instream );    if( rc ) return 0l;
       dom->uninit();

  delete dom;

  /*
   * Reset all the elements in stack 1
   */
  stack = 0;
  ComponentGraphItem *component = new ComponentGraphItem( m_id[stack].alloc(), m_schemaGraph, m_editable[stack] );

  foreach( ElementBase *element, m_elements[1] )
    {
      element->resetId( m_id[stack].alloc() ); // Reallocate ID of all the new elements
      m_elements[stack].append( element );
      component->addElement( element );
    }

  clearStack( 1 );

  return component;
}

ElementBase *SchemaView::element( int id )
{
  foreach( ElementBase* element, m_elements[stack] )
    {
      if( element->id() == id )
        return element;
    }
  return 0l;
}

void SchemaView::addId( int id )
{ m_id[stack].add( id ); }

bool SchemaView::idUnused( int id )
{ return m_id[stack].unused( id ); }

void SchemaView::mousePressEvent( QMouseEvent *event )
{
  if( 0l==m_mousePressEvent || (this->*m_mousePressEvent)( event ) )
    {
      QGraphicsView::mousePressEvent( event );
    }
}

void SchemaView::mouseMoveEvent( QMouseEvent *event )
{
  if( 0l==m_mouseMoveEvent || (this->*m_mouseMoveEvent)( event ) )
    {
      QGraphicsView::mouseMoveEvent( event );
    }
}

void SchemaView::mouseReleaseEvent( QMouseEvent *event )
{
  if( 0l==m_mouseReleaseEvent || (this->*m_mouseReleaseEvent)( event ) )
    {
      QGraphicsView::mouseReleaseEvent( event );
    }
}

void SchemaView::keyPressEvent( QKeyEvent *event )
{
  if( m_keyPressEvent && !(this->*m_keyPressEvent)( event ) )
    return;
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::ScrollHandDrag );
  if( event->key() == Qt::Key_Escape )
    setMode( MODE_SELECTION );
  if( event->key() == Qt::Key_Delete )
    {
      foreach( QGraphicsItem *item, m_schemaGraph->selectedItems() )
        {
          ElementBase *element = elementbase_cast(item);

          if( element->isRef() )
            {
              element->graphicsItem()->setSelected( false );
            }
          foreach( ElementBase *subelm, element->elements() )
            {
              subelm->graphicsItem()->setSelected( false );
            }
        }

      QList<QGraphicsItem *> selected( m_schemaGraph->selectedItems() );
      foreach( QGraphicsItem *item, selected )
        {
          ElementBase *element = elementbase_cast(item);
          removeElement( element );
        }
    }
}

void SchemaView::keyReleaseEvent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::RubberBandDrag );
}

void SchemaView::wheelEvent( QWheelEvent *event )
{
  scaleView( pow( 2.0, event->delta() / 700.0) );
}

void SchemaView::scaleView( qreal scaleFactor )
{
  qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 8, 8)).width();

  if( factor < 2 || factor > 100 ) return;

  m_scalefactor *=  factor;
  scale( scaleFactor, scaleFactor );
}

void SchemaView::resizeEvent( QResizeEvent *event )
{
  int width = event->size().width();
  int height = event->size().height();

  m_schemaGraph->setSceneRect( -width/2+2, -height/2+2, width-4, height-4 );

  QGraphicsView::resizeEvent(event);
}

void SchemaView::clearStack( int i )
{ m_elements[i].clear(); m_id[i].clear(); }


} // namespace dsim
