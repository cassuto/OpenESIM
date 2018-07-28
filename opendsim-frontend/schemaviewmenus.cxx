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
#include "propertiescustomdialog.h"

#include "schemaview.h"

namespace dsim
{

DECLARE_ELEMENT_CAST(ElementLine, "line");
DECLARE_ELEMENT_CAST(ElementText, "text");
DECLARE_ELEMENT_CAST(ElementPin, "pin");
DECLARE_ELEMENT_CAST(ComponentGraphItem, "component");
DECLARE_ELEMENT_CAST(ElementRect, "rect");
DECLARE_ELEMENT_CAST(ElementEllipse, "ellipse");

void SchemaView::createContextMenus()
{
  m_actionEditProperties = new QAction( tr("Edit properties..."), this );
  m_actionRotateLeft = new QAction( tr("Rotate left"), this );
  m_actionRotateLeft->setIcon( QIcon(":/bitmaps/rotateleft.png") );
  m_actionRotateRight = new QAction( tr("Rotate right"), this );
  m_actionRotateRight->setIcon( QIcon(":/bitmaps/rotateright.png") );

  connect( m_actionEditProperties, SIGNAL(triggered(bool)), this, SLOT(onEditProperties(bool)) );

  connect( m_actionRotateLeft, SIGNAL(triggered(bool)), this, SLOT(onRotateLeft(bool)) );

  connect( m_actionRotateRight, SIGNAL(triggered(bool)), this, SLOT(onRotateRight(bool)) );
}

void SchemaView::contextMenuEvent( QContextMenuEvent* event )
{
  QTransform transform;
  m_selectedElements = elementbase_cast( m_schemaGraph->itemAt( mapToScene( event->pos() ), transform ) );

  if( m_selectedElements )
    {
      QMenu *contextMenu = new QMenu( this );

      if( 0==std::strcmp( m_selectedElements->classname(), "line" ) ||
          0==std::strcmp( m_selectedElements->classname(), "rect" ) ||
          0==std::strcmp( m_selectedElements->classname(), "ellipse" ) ||
          0==std::strcmp( m_selectedElements->classname(), "text" ) )
        {
          contextMenu->addAction( m_actionEditProperties );
        }
      if( 0==std::strcmp( m_selectedElements->classname(), "pin" ) )
        {
          QAction *editPinPropertiesAct = new QAction( tr("Edit properties of pin..."), this );
          connect( editPinPropertiesAct, SIGNAL(triggered(bool)), this, SLOT(onEditPinProperties(bool)) );

          contextMenu->addAction( editPinPropertiesAct );
        }

      if( 0==std::strcmp( m_selectedElements->classname(), "text" ) ||
          0==std::strcmp( m_selectedElements->classname(), "pin" ) ||
          0==std::strcmp( m_selectedElements->classname(), "component" ) )
        {
          contextMenu->addAction( m_actionRotateLeft );
          contextMenu->addAction( m_actionRotateRight );
        }

      if( 0==std::strcmp( m_selectedElements->classname(), "wire" ) )
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
  static void configLineFillText( base *element )
  {
    PropertiesCustomDialog customDialog( element->customLine(), element->customFill(), element->customText() );

    customDialog.loadTemplate( element->style() );

    if( customDialog.exec() == QDialog::Accepted )
      {
        element->setStyle( customDialog.styleName().c_str() );
        element->setCustomLine( customDialog.lineCustom() );
        element->setCustomFill( customDialog.fillCustom() );
        element->setCustomText( customDialog.textCustom() );
        element->update();
      }
  }

template <typename base>
  static void configText( base *element )
  {
    PropertiesCustomDialog customDialog( 0l, 0l, element->customText() );

    customDialog.loadTemplate( element->style() );

    if( customDialog.exec() == QDialog::Accepted )
      {
        element->setStyle( customDialog.styleName().c_str() );
        element->setCustomText( customDialog.textCustom() );
        element->update();
      }
  }

// ------------------------------------------------------------------ //
// General actions for property edition
// ------------------------------------------------------------------ //

void SchemaView::onEditProperties( bool )
{
  if( ElementLine *line = element_cast<ElementLine *>(m_selectedElements) )
    {
      configLineFillText( line );
    }
  else if( ElementRect *rect = element_cast<ElementRect *>(m_selectedElements) )
    {
      configLineFillText( rect );
    }
  else if( ElementEllipse *ellipse = element_cast<ElementEllipse *>(m_selectedElements) )
    {
      configLineFillText( ellipse );
    }
  else if( ElementText *text = element_cast<ElementText *>(m_selectedElements) )
    {
      configText( text );
    }
}

void SchemaView::onRemoveSelected( bool )
{
  if( m_selectedElements ) releaseElement( m_selectedElements );
}

void SchemaView::onEditPinProperties( bool )
{
  if( m_selectedElements )
    {
      ElementPin *pin = element_cast<ElementPin *>( m_selectedElements );
      if( pin )
        {
          pin->execPropertiesDialog();
        }
    }
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

void SchemaView::onRotateLeft( bool )
{
  if( m_selectedElements )
    {
      if( ElementText *text = element_cast<ElementText *>(m_selectedElements) )
        {
          rotateLeft( text );
        }
      else if( ElementPin *pin = element_cast<ElementPin *>(m_selectedElements) )
        {
          rotateLeft( pin );
        }
      else if ( ComponentGraphItem *component = element_cast<ComponentGraphItem *>(m_selectedElements) )
        {
          rotateLeft( component );
        }
    }
}

void SchemaView::onRotateRight( bool )
{
  if( m_selectedElements )
    {
      if( ElementText *text = element_cast<ElementText *>(m_selectedElements) )
        {
          rotateRight( text );
        }
      else if( ElementPin *pin = element_cast<ElementPin *>(m_selectedElements) )
        {
          rotateRight( pin );
        }
      else if ( ComponentGraphItem *component = element_cast<ComponentGraphItem *>(m_selectedElements) )
        {
          rotateRight( component );
        }
    }
}

}
