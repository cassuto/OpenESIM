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

#include <frontend/error.h>
#include "schemasheet.h"
#include "schemaview.h"
#include "lispdataset.h"

#include "schemawidget.h"

namespace dsim
{

SchemaWidget::SchemaWidget( QWidget *parent, SchemaSheet *sheet, LispDataset *dom )
            : QWidget( parent )
            , m_sheet( sheet )
            , m_dom( dom )
{
  m_view = new SchemaView( m_sheet, this );

  if( m_dom->type() == DOM_SCHEMA_SYMBOL )
    {
      m_view->setPaintFrameAxes( true );
    }

  m_verticalLayout = new QVBoxLayout( this );
  m_verticalLayout->setObjectName(tr("verticalLayout"));
  m_verticalLayout->setContentsMargins(0, 0, 0, 0);
  m_verticalLayout->setSpacing(0);

  m_verticalLayout->addWidget( m_view );
}

SchemaWidget::~SchemaWidget()
{}

void SchemaWidget::clear()
{ m_view->clear(); }

void SchemaWidget::setMode( DrawMode mode )
{ m_view->setMode( mode ); }

} // namespace dsim

#include "schemawidget.moc"
