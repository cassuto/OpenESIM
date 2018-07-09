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
#include "domdataset.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"
#include "schemaview.h"

namespace dsim
{

SchemaView::SchemaView( SchemaSheet *sheet, QWidget *parent )
          : QGraphicsView( parent )
          , m_hintElement( 0l )
          , m_hintComponent( 0l )
          , m_hintSize( 0 )
          , m_hintDirect( ELEM_LEFT )
          , m_sheet( sheet )
          , m_mode( MODE_SELECTION )
          , stack( 0 )
{
  m_schemaGraph = 0;
  m_scalefactor = 1;

  clear();

  viewport()->setFixedSize( 1600, 1200 );
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setViewportUpdateMode( QGraphicsView::MinimalViewportUpdate );
  setCacheMode( CacheBackground );
  setRenderHint( QPainter::Antialiasing );
  setTransformationAnchor( AnchorUnderMouse );
  setResizeAnchor( AnchorUnderMouse );
  setDragMode( QGraphicsView::RubberBandDrag );

  setAcceptDrops( true );

  m_editable[0] = true;
  m_editable[1] = false;
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

void SchemaView::setMode( DrawMode mode )
{
  m_mode = mode;

  switch( mode )
  {
    case MODE_SELECTION:
      this->setCursor( QCursor( Qt::ArrowCursor ));
      break;
    case MODE_PIN:
    case MODE_LINE:
    case MODE_TEXT:
    case MODE_RECT:
    case MODE_ROUND:
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      this->setCursor( QCursor( Qt::CrossCursor ));
      break;
  }

  switch( mode )
  {
    case MODE_SELECTION:
      if ( m_hintElement )
        {
          removeElement( m_hintElement );
          m_hintElement = 0;
        }
      if ( m_hintComponent )
        {
          removeElement( m_hintComponent );
          m_hintComponent = 0;
        }
      m_hintSize = 0;
      break;

    case MODE_PIN:
      {
        ElementGraphItem *symbol = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)symbol)->setText( "PIN 1" );

        m_hintElement = createElement( "pin", QPoint( 0, 0 ) );
        ((ElementPin *)m_hintElement)->setSub( (ElementText *)symbol );

        m_hintElement->setVisible( false );
        break;
      }

    case MODE_LINE:
      break;
    case MODE_TEXT:
      {
        m_hintElement = createElement( "text", QPoint( 0, 0 ) );
        m_hintElement->setVisible( false );
        break;
      }
    case MODE_RECT:
      {
        m_hintElement = createElement( "rect", QPoint( 0, 0 ) );
        m_hintElement->setVisible( false );
        m_hintSize = 1;
        break;
      }
    case MODE_ROUND:
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      break;
  }
}

ElementGraphItem *SchemaView::createElement( const char *classname, const QPoint &pos, bool editable, bool deser )
{
  int id = 0;
  ElementGraphItem *element = 0l;

  if( !deser ) id = m_id[stack].alloc();

  if( 0==std::strcmp( classname, "pin" ) )
    {
      ElementPin *pinElement = new ElementPin( id, editable, (ElemDirect)m_hintDirect, pos, 0l, m_schemaGraph );
      element = pinElement;
    }
  else if( 0==std::strcmp( classname, "text" ) )
    {
      ElementText *textElement = new ElementText( id, editable, (ElemDirect)m_hintDirect, pos, m_schemaGraph );
      textElement->setText( "TEXT 1" );
      element = textElement;
    }
  else if( 0==std::strcmp( classname, "rect" ) )
    {
      ElementRect *rectElement = new ElementRect( id, editable, pos, m_schemaGraph );
      element = rectElement;
    }

  m_schemaGraph->addItem( element );
  m_elements[stack].append( element );
  return element;
}

void SchemaView::removeElement( ElementGraphItem *element )
{
  if( std::strcmp( element->classname(), "component") == 0 )
    {
      ComponentGraphItem *component = static_cast<ComponentGraphItem *>(element);
      m_sheet->deleteDevice( component->device() );
    }

  foreach( ElementGraphItem *subelm, element->elements() )
    {
      m_elements[stack].removeOne( subelm );
      m_id[stack].release( subelm->id() );
      m_schemaGraph->removeItem( subelm );
    }

  m_elements[stack].removeOne( element );
  m_id[stack].release( element->id() );
  m_schemaGraph->removeItem( element );
}

int SchemaView::serialize( DomDataset *dom )
{
  int rc = 0;

  dom->beginEntry( "elements" );
  {
    foreach( ElementGraphItem *element, m_elements[stack] )
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

int SchemaView::deserialize( DomDataset *dom )
{
  int rc = 0;
  std::string symbol;

  clear();

  /*
   * Deserialize Elements
   */
  DomEntry nodes = dom->entry( 0, "elements" );
  {
    for( nodes.begin(); nodes.valid(); nodes.next() )
      {
        dom->deserializePush( nodes.current() );
        {
          DomDataset::AutoPop autoPop( dom );

          rc = dom->des( symbol, /*symbol*/true ); UPDATE_RC(rc);

          const char *classname = symbol.c_str();
          ElementGraphItem *elem = createElement( classname, QPoint(0, 0), m_editable[stack], /*deser*/ true );
          if( elem )
            {
              dom->deserializePush( dom->current() );
              {
                DomDataset::AutoPop autoPop( dom );

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
      foreach( ElementGraphItem* element, m_elements[stack] )
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
  DomDataset *dom = new DomDataset( DOM_SCHEMA_SYMBOL );

  rc = dom->init();                     if( rc ) return 0l;
       dom->addItem( this );
  rc = dom->deserialize( instream );    if( rc ) return 0l;
       dom->uninit();

  delete dom;

  /*
   * Reset all the elements in stack 1
   */
  stack = 0;
  ComponentGraphItem *component = new ComponentGraphItem( m_id[stack].alloc(), m_editable[stack], /*device*/0l, this->m_schemaGraph );

  foreach( ElementGraphItem *element, m_elements[1] )
    {
      element->resetId( m_id[stack].alloc() ); // Reallocate ID of all the new elements
      m_elements[stack].append( element );
      component->addElement( element );
    }

  clearStack( 1 );

  return component;
}

ElementGraphItem *SchemaView::element( int id )
{
  foreach( ElementGraphItem* element, m_elements[stack] )
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

void SchemaView::dragEnterEvent( QDragEnterEvent *event )
{
  event->accept();

  QString text = event->mimeData()->text();
  std::string symbol = text.toStdString();

  IDevice *hintDevice = m_sheet->createDevice( symbol.c_str(), "", 1 );
  if( hintDevice )
    {
      m_hintComponent = new ComponentGraphItem( m_id[stack].alloc(), true, hintDevice, m_schemaGraph );
      m_hintComponent->setPos( mapToScene( event->pos() ) );
      m_hintComponent->setLayout();
      m_schemaGraph->addItem( m_hintComponent );

      setMode( MODE_COMPONENT );
    }
}

void SchemaView::dragMoveEvent( QDragMoveEvent *event )
{
  event->accept();

  if( m_hintComponent ) m_hintComponent->setPos( togrid( mapToScene( event->pos() ) ) );
}

void SchemaView::dragLeaveEvent( QDragLeaveEvent *event )
{
  event->accept();

  setMode( MODE_SELECTION );
}

void SchemaView::dropEvent( QGraphicsSceneDragDropEvent *event )
{
  event->accept();

  m_hintComponent = 0l;
}

void SchemaView::mousePressEvent( QMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      if( m_hintElement )
        {
          if( 1==m_hintSize )
            {
              m_hintOrigPos = m_hintElement->pos();
              m_hintSize = 2;

              event->accept();

              QPointF np = mapFromScene( m_hintElement->pos() ) + m_hintElement->boundingRect().bottomRight();
              QPoint curpos = mapToGlobal( QPoint(int(np.x()+1), int(np.y()+1)) );
              QCursor::setPos( curpos );
              return;
            }
          else
            {
              if( 2 == m_hintSize )
                if( !hintSize( togrid( mapToScene( event->pos() ) ) ) )
                  { setMode( MODE_SELECTION ); return; }

              m_hintElement->setLayout();
              m_hintElement->setVisible( true );
              m_hintElement = 0l;

              setMode( MODE_SELECTION );
            }
        }
    }
  else if( event->button() == Qt::RightButton )
    {
      setMode( MODE_SELECTION );
    }

  QGraphicsView::mousePressEvent( event );
}

void SchemaView::mouseMoveEvent( QMouseEvent *event )
{
  if( m_hintElement )
    {
      if( 2 == m_hintSize )
        {
          if( !hintSize( togrid( mapToScene( event->pos() ) ) ) ) return;
        }
      else
        {
          m_hintElement->setPos( togrid( mapToScene( event->pos() ) ) );
        }
      m_hintElement->setLayout();
      m_hintElement->setVisible( true );
    }

  QGraphicsView::mouseMoveEvent( event );
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

bool SchemaView::hintSize( const QPointF &cpos )
{
  float width = cpos.x() - m_hintOrigPos.x() - 1;
  float height = cpos.y() - m_hintOrigPos.y() - 1;

  if( width < 1.0 || height < 1.0 )
    {
      setMode( MODE_SELECTION );
      return false;
    }

  if( 0==std::strcmp( m_hintElement->classname(), "rect" ) )
    {
      ElementRect *rectElement = static_cast<ElementRect *>(m_hintElement);
      rectElement->setSize( width, height );
      return true;
    }
  return false;
}

void SchemaView::clearStack( int i )
{ m_elements[i].clear(); m_id[i].clear(); }

void SchemaView::resizeEvent( QResizeEvent *event )
{
  int width = event->size().width();
  int height = event->size().height();

  m_schemaGraph->setSceneRect( -width/2+2, -height/2+2, width-4, height-4 );

  QGraphicsView::resizeEvent(event);
}

void SchemaView::keyPressEvent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::ScrollHandDrag );
  if( event->key() == Qt::Key_Space )
    {
      if( m_hintElement )
        {
          if( m_hintDirect < ELEM_BOTTOM )
            ++m_hintDirect;
          else
            m_hintDirect = 0;
          m_hintElement->setDirect( (ElemDirect)m_hintDirect );
        }
    }
  if( event->key() == Qt::Key_Delete )
    {
      foreach( QGraphicsItem *item, m_schemaGraph->selectedItems() )
        {
          ElementGraphItem *element = qgraphicsitem_cast<ElementGraphItem *>(item);

          if( element->isRef() )
            {
              element->setSelected( false );
            }
          foreach( ElementGraphItem *subelm, element->elements() )
            {
              subelm->setSelected( false );
            }
        }

      QList<QGraphicsItem *> selected( m_schemaGraph->selectedItems() );
      foreach( QGraphicsItem *item, selected )
        {
          ElementGraphItem *element = qgraphicsitem_cast<ElementGraphItem *>(item);
          removeElement( element );
        }
    }
}

void SchemaView::keyReleaseEvent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::RubberBandDrag );
}

} // namespace dsim
