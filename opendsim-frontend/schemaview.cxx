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

#define TRACE_UNIT "schemaview"
#include <QtGui>
#include <cstring>
#include <dsim/logtrace.h>
#include <frontend/error.h>
#include "schemasheet.h"
#include "schemascene.h"
#include "lispdataset.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementwire.h"
#include "elementline.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"
#include "elementellipse.h"
#include "elementjoint.h"
#include "elementorigin.h"
#include "schemaview.h"

namespace dsim
{

SchemaView::SchemaView( SchemaSheet *sheet, QWidget *parent )
          : QGraphicsView( parent )
          , m_paintGrid( true )
          , m_paintFrameAxes( false )
          , m_sheet( sheet )
          , stack( 0 )
          , m_selectedElements( 0l )
{
  m_schemaGraph = 0;
  m_scalefactor = 1.0;

  clear();

  sheet->setSchemaView( this );

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
  m_currentDirection = 0;
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

SchemaView::~SchemaView() { releaseAllElements(); }

void SchemaView::clear()
{
  if( m_schemaGraph )
    {
      releaseAllElements();
      m_schemaGraph->deleteLater();

      m_hintElement = 0l;
      m_hintComponent = 0l;
    }

  clearStack( 0 );
  clearStack( 1 );

  m_schemaGraph = new SchemaScene( -1600, -1200, 3200, 2400, this );
  m_schemaGraph->setPaintGrid( m_paintGrid );
  m_schemaGraph->setPaintFrameAxes( m_paintFrameAxes );
  m_schemaGraph->update();

  setScene( m_schemaGraph );
  gotoCenter();
}

void SchemaView::gotoCenter()
{ centerOn( mapFromScene( -(this->width()/2), -(this->height()/2 )) ); }

void SchemaView::setPaintGrid( bool paint )
{ m_paintGrid = paint; m_schemaGraph->setPaintGrid( paint ); viewport()->update(); }

void SchemaView::setPaintFrameAxes( bool paint )
{ m_paintFrameAxes = paint; m_schemaGraph->setPaintFrameAxes( paint ); viewport()->update(); }


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
  else if( 0==std::strcmp( classname, "pin" ) )
    {
      ElementPin *pinElement = new ElementPin( (ElemDirect)m_hintDirect, pos, 0l, 0l, id, m_schemaGraph, editable );
      element = pinElement;
    }
  else if( 0==std::strcmp( classname, "wire" ) )
    {
      ElementWire *wireElement = new ElementWire( id, m_schemaGraph );
      element = wireElement;
    }
  else if( 0==std::strcmp( classname, "joint" ) )
    {
      ElementJoint *jointElement = new ElementJoint( pos, id, m_schemaGraph );
      element = jointElement;
    }
  else if( 0==std::strcmp( classname, "jointport" ) )
    {
      ElementJointPort *jointportElement = new ElementJointPort( id, m_schemaGraph );
      element = jointportElement;
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
  else if( 0==std::strcmp( classname, "origin" ) )
    {
      ElementOrigin *originElement = new ElementOrigin( pos, id, m_schemaGraph, editable );
      element = originElement;
    }
  else
    {
      return 0l;
    }
  element->addToScene( m_schemaGraph );
  m_elements[stack].append( element );
  return element;
}

void SchemaView::releaseElement( ElementBase *element )
{
  element->releaseSubElements(); // recursively calls releaseElement()

#define removeElement(_element) \
  { \
    _element->removeFromScene( m_schemaGraph );                 \
    m_elements[stack].removeOne( _element );                    \
    m_id[stack].release( _element->id() );                      \
  }

  if( element->refcount() == 0 ) // target is not referenced by any elements But the SchemaScene wants to delete it
    {
      removeElement( element )
      delete element;
    }
  else if( element->refcount()-1 == 0  ) // target is to be no longer referenced
    {
      removeElement( element )
      element->release(); // delete
    }
  else                          // just decrease the counter
    {
      element->release();
    }
}

void SchemaView::releaseAllElements()
{
  trace_assert( stack == 0 );

  /*
   * As releaseSubElements() may delete some elements and affect the element list, we have to maintain a list storing
   * current root targets to release here. It only makes sense when we are processing root elements.
   */
  QList<ElementBase *>releases;
  foreach( ElementBase *element, m_elements[stack] )
    {
      if( element->isRoot() )
        {
          releases.append( element ); // pick out root elements
        }
    }

  foreach( ElementBase *element, releases )
    {
      trace_assert( m_elements[stack].contains( element ) );
      if( element->isRoot() )
        {
          releaseElement( element );
        }
    }

  trace_assert( m_elements[stack].count() == 0 ); // ensure that we have released all
#ifndef NDEBUG
  if( m_elements[stack].count() )
    {
      foreach( ElementBase *element, m_elements[stack] )
        {
          trace_debug(("memmory leaked %p %s\n", element, element->classname() ));
        }
    }
#endif
}

int SchemaView::serialize( LispDataset *dom )
{
  int rc = 0;

  if( dom->type() == DOM_SCHEMA_SYMBOL )
    {
      ElementOrigin *origin = 0l;
      foreach( ElementBase *element, elements() )
        {
          if( 0==std::strcmp( element->classname(), "origin" ) )
            {
              origin = static_cast<ElementOrigin *>(element);
              break;
            }
        }
      dom->beginNode( false, "origin" );
      {
        if( origin )
          {
            rc = dom->ser( double(origin->originPos().x()) ); UPDATE_RC(rc);
            rc = dom->ser( double(origin->originPos().y()) ); UPDATE_RC(rc);
          }
        else
          {
            rc = dom->ser( double(0) );         UPDATE_RC(rc);
            rc = dom->ser( double(0) );         UPDATE_RC(rc);
          }
      }
      dom->endNode();
    }

  dom->beginEntry( "elements" );
  {
    foreach( ElementBase *element, m_elements[0] )
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

  if( stack == 0 ) clear();

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
                    releaseElement( elem ); // fault
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

int SchemaView::loadSymbol( ComponentGraphItem *component, const char *filename )
{ using namespace std;
  int rc = 0;

  string fn( "library/" );
  fn += filename;
  ifstream instream( fn.c_str() );
  if( !instream.is_open() ) return -DS_OPEN_FILE;

  clearStack( 1 );
  stack = 1;

  /*
   * Parse ssymbol file
   */
  LispDataset *dom = new LispDataset( DOM_SCHEMA_SYMBOL );

  rc = dom->init();                     if( rc ) { stack = 0; return rc; }
       dom->addItem( this );
  rc = dom->deserialize( instream );    if( rc ) { stack = 0; return rc; }
       dom->uninit();

  delete dom;

  stack = 0;

  ElementOrigin *originElement = 0l;
  foreach( ElementBase *element, m_elements[1] )
    {
      if( 0==std::strcmp( element->classname(), "origin" ) )
        { originElement = static_cast<ElementOrigin *>(element); break; }
    }

  QPointF origin;
  if( originElement )
    { origin = originElement->originPos(); }
  else
    { origin.setX( gridWh/2 ); origin.setY( gridHt/2 ); } // there is no origin information, using the default origin that is aligned to the grid.

  QPointF oldOrigin = component->pos(); component->setPos( origin );

  foreach( ElementBase *element, m_elements[1] )
    {
      element->resetId( m_id[stack].alloc() ); // Reallocate ID of all the new elements
      m_elements[stack].append( element );
      component->addComponentElement( element );
    }
  component->setPos( oldOrigin );

  clearStack( 1 );

  return rc;
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
      QList<ElementBase *> targetList;

      foreach( QGraphicsItem *item, m_schemaGraph->selectedItems() )
        {
          ElementBase *element = elementbase_cast(item);

          if( element && element->isRoot() ) targetList.append( element ); // pick out the standalone element
        }

      foreach( ElementBase *element, targetList )
        {
          releaseElement( element );
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

#include "schemaview.moc"
