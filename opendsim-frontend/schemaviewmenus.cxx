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

#include <cstring>

#include "schemascene.h"
#include "elementgraphitem.h"
#include "componentgraphitem.h"
#include "elementline.h"
#include "elementtext.h"
#include "elementpin.h"
#include "elementrect.h"
#include "elementellipse.h"
#include "elementarc.h"
#include "elementpainter.h"
#include "propertydialog.h"
#include "propertywidgetcustom.h"
#include "propertywidgettext.h"
#include "propertywidgetpin.h"
#include "propertywidgetcomponent.h"
#include "propertywidgetconfig.h"
#include "propertywidgetpainter.h"
#include "mainwindow.h"
#include "schemaview.h"

namespace dsim
{

DECLARE_ELEMENT_CAST(ElementLine, "line");
DECLARE_ELEMENT_CAST(ElementText, "text");
DECLARE_ELEMENT_CAST(ElementPin, "pin");
DECLARE_ELEMENT_CAST(ComponentGraphItem, "component");
DECLARE_ELEMENT_CAST(ElementRect, "rect");
DECLARE_ELEMENT_CAST(ElementEllipse, "ellipse");
DECLARE_ELEMENT_CAST(ElementArc, "arc");
DECLARE_ELEMENT_CAST(ElementPainter, "painter");

void SchemaView::createContextMenus()
{
  m_actionEditProperties = new QAction( tr("Edit properties..."), this );
  m_actionRotateLeft = new QAction( tr("Rotate left"), this );
  m_actionRotateLeft->setIcon( QIcon(":/bitmaps/rotateleft.png") );
  m_actionRotateRight = new QAction( tr("Rotate right"), this );
  m_actionRotateRight->setIcon( QIcon(":/bitmaps/rotateright.png") );
  m_actionXmirror = new QAction( tr("X-axis mirror"), this );
  m_actionXmirror->setIcon( QIcon(":/bitmaps/xmirror.png") );
  m_actionYmirror = new QAction( tr("Y-axis mirror"), this );
  m_actionYmirror->setIcon( QIcon(":/bitmaps/ymirror.png") );

  connect( m_actionEditProperties, SIGNAL(triggered(bool)), this, SLOT(onEditProperties(bool)) );

  connect( m_actionRotateLeft, SIGNAL(triggered(bool)), this, SLOT(onRotateLeft(bool)) );

  connect( m_actionRotateRight, SIGNAL(triggered(bool)), this, SLOT(onRotateRight(bool)) );

  connect( m_actionXmirror, SIGNAL(triggered(bool)), this, SLOT(onXmirror(bool)) );

  connect( m_actionYmirror, SIGNAL(triggered(bool)), this, SLOT(onYmirror(bool)) );
}

void SchemaView::contextMenuEvent( QContextMenuEvent* event )
{
  QTransform transform;
  m_selectedElements = elementbase_cast( m_schemaGraph->itemAt( mapToScene( event->pos() ), transform ) );

  if( m_selectedElements )
    {
      QMenu *contextMenu = new QMenu( this );

      bool isWire       = ( 0==std::strcmp( m_selectedElements->classname(), "wire" ) );
      bool isLine       = ( 0==std::strcmp( m_selectedElements->classname(), "line" ) );
      bool isRect       = ( 0==std::strcmp( m_selectedElements->classname(), "rect" ) );
      bool isEllipse    = ( 0==std::strcmp( m_selectedElements->classname(), "ellipse" ) );
      bool isText       = ( 0==std::strcmp( m_selectedElements->classname(), "text" ) );
      bool isPin        = ( 0==std::strcmp( m_selectedElements->classname(), "pin" ) );
      bool isComponent  = ( 0==std::strcmp( m_selectedElements->classname(), "component") );

      if( isLine || isRect || isEllipse || isText || isPin || isComponent )
        {
          contextMenu->addAction( m_actionEditProperties );
        }
      if( isText || isPin || isComponent )
        {
          contextMenu->addAction( m_actionRotateLeft );
          contextMenu->addAction( m_actionRotateRight );
          contextMenu->addAction( m_actionXmirror );
          contextMenu->addAction( m_actionYmirror );
        }

      if( isWire )
        {
          QAction *removeWireAct = new QAction( tr("Remove this wire"), this );
          connect( removeWireAct, SIGNAL(triggered(bool)), this, SLOT(onRemoveSelected(bool)) );

          contextMenu->addAction( removeWireAct );
        }
      contextMenu->exec( event->globalPos() );
      contextMenu->deleteLater();
    }

  m_selectedElements = 0l;
}

template <typename base>
  static void configLineFillText( PropertyDialog &settings, base *element )
  {
    PropertyWidgetCustom *customWidget = new PropertyWidgetCustom( element->customLine(), element->customFill(), element->customText() );
    settings.addPropertyWidget( customWidget );

    customWidget->loadTemplate( element->style() );

    if( settings.exec() == QDialog::Accepted )
      {
        element->setStyle( customWidget->styleName().c_str() );
        element->setCustomLine( customWidget->lineCustom() );
        element->setCustomFill( customWidget->fillCustom() );
        element->setCustomText( customWidget->textCustom() );
        element->update();
      }
    customWidget->deleteLater();
  }

template <typename base>
  static void configText( PropertyDialog &settings, base *element )
  {
    PropertyWidgetCustom *customWidget = new PropertyWidgetCustom( 0l, 0l, element->customText() );
    settings.addPropertyWidget( customWidget );

    customWidget->loadTemplate( element->style() );

    if( settings.exec() == QDialog::Accepted )
      {
        element->setStyle( customWidget->styleName().c_str() );
        element->setCustomText( customWidget->textCustom() );
        element->update();
      }
    customWidget->deleteLater();
  }

// ------------------------------------------------------------------ //
// General actions for property edition
// ------------------------------------------------------------------ //

void SchemaView::onEditProperties( bool )
{
  if( m_selectedElements )
    {
      PropertyDialog settings( MainWindow::instance() );

      if( ElementLine *line = element_cast<ElementLine *>(m_selectedElements) )
        {
          configLineFillText( settings, line );
        }
      else if( ElementRect *rect = element_cast<ElementRect *>(m_selectedElements) )
        {
          configLineFillText( settings, rect );
        }
      else if( ElementEllipse *ellipse = element_cast<ElementEllipse *>(m_selectedElements) )
        {
          configLineFillText( settings, ellipse );
        }
      else if( ElementArc *arc = element_cast<ElementArc *>(m_selectedElements) )
        {
          configLineFillText( settings, arc );
        }
      else if( ElementPainter *painter = element_cast<ElementPainter *>(m_selectedElements) )
        {
          settings.addPropertyWidget( new PropertyWidgetPainter( painter, &settings ) );
          configLineFillText( settings, painter );
        }
      else if( ElementText *text = element_cast<ElementText *>(m_selectedElements) )
        {
          settings.addPropertyWidget( new PropertyWidgetText( text, &settings ) );
          configText( settings, text );
        }
      else if( ElementPin *pin = element_cast<ElementPin *>( m_selectedElements ) )
        {
          settings.addPropertyWidget( new PropertyWidgetPin( pin, &settings ) );
          settings.exec();
        }
      else if( ComponentGraphItem *component = element_cast<ComponentGraphItem *>( m_selectedElements ) )
        {
          settings.addPropertyWidget( new PropertyWidgetComponent( component, &settings ) );
          settings.addPropertyWidget( new PropertyWidgetConfig( component->properties(), &settings ) );
          settings.exec();
        }
    }
}

void SchemaView::onRemoveSelected( bool )
{
  if( m_selectedElements ) releaseElement( m_selectedElements );
}

template <typename base>
  static void rotateLeft( base *element )
    {
      int direction = int( element->direct() );
      if( direction > ELEM_LEFT )
        --direction;
      else
        direction = ELEM_BOTTOM;
      element->setDirect( (ElemDirect)direction );
    }

template <typename base>
  static void rotateRight( base *element )
    {
      int direction = int( element->direct() );
      if( direction < ELEM_BOTTOM )
        ++direction;
      else
        direction = ELEM_LEFT;
      element->setDirect( (ElemDirect)direction );
    }

template <typename base>
  static void Xmirror( base *element )
    {
      int direction = int( element->direct() );
      if( direction == ELEM_LEFT || direction == ELEM_RIGHT )
        {
          element->setDirect( (ElemDirect)( (direction + 2) % 4 ) );
        }
    }

template <typename base>
  static void Ymirror( base *element )
    {
      int direction = int( element->direct() );
      if( direction == ELEM_TOP || direction == ELEM_BOTTOM )
        {
          element->setDirect( (ElemDirect)( (direction + 2) % 4 ) );
        }
    }

#define TEMPLATE_TRANSFORMATION(_comp, _op) \
  {                                                                                         \
    if( ElementText *text = element_cast<ElementText *>(_comp) )                            \
      {                                                                                     \
        _op( text );                                                                        \
      }                                                                                     \
    else if( ElementPin *pin = element_cast<ElementPin *>(_comp) )                          \
      {                                                                                     \
        _op( pin );                                                                         \
      }                                                                                     \
    else if ( ComponentGraphItem *component = element_cast<ComponentGraphItem *>(_comp) )   \
      {                                                                                     \
        _op( component );                                                                   \
      }                                                                                     \
  }

void SchemaView::onRotateLeft( bool )
{
  if( m_selectedElements ) TEMPLATE_TRANSFORMATION( m_selectedElements, rotateLeft )
}

void SchemaView::onRotateRight( bool )
{
  if( m_selectedElements ) TEMPLATE_TRANSFORMATION( m_selectedElements, rotateRight )
}

void SchemaView::onXmirror( bool )
{
  if( m_selectedElements ) TEMPLATE_TRANSFORMATION( m_selectedElements, Xmirror )
}

void SchemaView::onYmirror( bool )
{
  if( m_selectedElements ) TEMPLATE_TRANSFORMATION( m_selectedElements, Ymirror )
}

}
