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

#include <elementgraphitem.h>
#include <QtGui>

#include "schemasheet.h"
#include "schemagraph.h"
#include "domdataset.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementtext.h"
#include "elementpin.h"
#include "schemaview.h"

namespace dsim
{

SchemaView::SchemaView( SchemaSheet *sheet, DomDataset *dom, QWidget *parent )
          : QGraphicsView( parent )
          , m_hintElement( 0l )
          , m_hintComponent( 0l )
          , m_hintDirect( ELEM_LEFT )
          , m_sheet( sheet )
          , m_dom( dom )
          , m_mode( MODE_SELECTION )
{
  m_schemaGraph = 0;
  m_scalefactor = 1;

  dom->setSchemaView( this );

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
}
SchemaView::~SchemaView() { }

void SchemaView::clear()
{
  if( m_schemaGraph )
    {
      m_schemaGraph->remove();
      m_schemaGraph->deleteLater();
    }
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
          m_schemaGraph->removeItem( m_hintElement );
          m_elements.removeOne( m_hintElement );
          m_hintElement = 0;
        }
      if ( m_hintComponent )
        {
          m_sheet->deleteDevice( m_hintComponent->device() );
          m_schemaGraph->removeItem( m_hintComponent );
          m_elements.removeOne( m_hintElement );
          m_hintComponent = 0;
        }
      break;

    case MODE_PIN:
      {
        ElementGraphItem *symbol = createElement( "text", QPoint( 0, 0 ) );
        ((ElementText *)symbol)->setText( "PIN 1" );

        m_hintElement = createElement( "pin", QPoint( 0, 0 ) );
        ((ElementPin *)m_hintElement)->setSymbol( (ElementText *)symbol );

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
    case MODE_ROUND:
    case MODE_SCRIPT:
    case MODE_COMPONENT:
      break;
  }
}

ElementGraphItem *SchemaView::createElement( const char *classname, const QPoint &pos, bool deser )
{
  int id = 0;
  ElementGraphItem *element = 0l;

  if( 0==strcmp( classname, "pin" ) )
    {
      if( !deser ) id = m_sheet->allocId();
      ElementPin *pinElement = new ElementPin( id, (ElemDirect)m_hintDirect, pos, 0l, true, m_schemaGraph );
      element = pinElement;
    }
  else if( 0==strcmp( classname, "text" ) )
    {
      if( !deser ) id = m_sheet->allocId();
      ElementText *textElement = new ElementText( id, (ElemDirect)m_hintDirect, pos, m_schemaGraph );
      textElement->setText( "TEXT 1" );
      element = textElement;
    }

  m_schemaGraph->addItem( element );
  m_elements.append( element );

  if( deser )
    {
      element->addDOM( dom() );
    }
  return element;
}

int SchemaView::serialize( DomType type, std::ofstream & outstream )
{
  return dom()->serialize( type, outstream );
}

int SchemaView::deserialize( DomType type, std::ifstream &instream )
{
  sheet()->clearId();

  int rc = dom()->deserialize( type, instream );

  if( !rc )
    {
      foreach( ElementGraphItem* element, m_elements )
        {
          if( (rc = element->resolveSubElements()) )
            return rc;
        }
    }
  return rc;
}

ElementGraphItem *SchemaView::element( int id )
{
  foreach( QGraphicsItem* item, m_schemaGraph->items() )
    {
      ElementGraphItem *element = qgraphicsitem_cast<ElementGraphItem *>(item);

      if( element->id() == id )
        return element;
    }
  return 0;
}

void SchemaView::dragEnterEvent( QDragEnterEvent *event )
{
  event->accept();

  QString text = event->mimeData()->text();
  std::string symbol = text.toStdString();

  IDevice *hintDevice = m_sheet->createDevice( symbol.c_str(), "", 1 );
  if( hintDevice )
    {
      m_hintComponent = new ComponentGraphItem( m_sheet->allocId(), hintDevice, m_schemaGraph );
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
          m_hintElement->setLayout();
          m_hintElement->setVisible( true );
          m_hintElement->addDOM( m_dom );
          m_hintElement = 0l;
        }

      setMode( MODE_SELECTION );
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
      m_hintElement->setVisible( true );
      m_hintElement->setPos( togrid( mapToScene( event->pos() ) ) );
      m_hintElement->setLayout();
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
          m_hintElement->setDirect( (ElemDirect)m_hintDirect );
          if( m_hintDirect < ELEM_BOTTOM )
            m_hintDirect++;
          else
            m_hintDirect = 0;
        }
    }
}

void SchemaView::keyReleaseEvent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::RubberBandDrag );
}

} // namespace dsim
