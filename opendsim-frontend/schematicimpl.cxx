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
#include <device/device.h>
#include <device/graph.h>
#include "schemaview.h"
#include "componentgraphitem.h"
#include "elementpainter.h"
#include "schematicimpl.h"

namespace dsim
{

SchematicImpl::SchematicImpl( SchemaView *schemaView, ComponentGraphItem *component )
              : m_schemaView( schemaView )
              , m_component( component )
{}

void SchematicImpl::reset() {}

void SchematicImpl::registerRender( IDevice *device )
{ m_schemaView->sheet()->registerRender( SchemaSheet::RenderData( this, m_component->deviceGraph(), device ) ); }

void SchematicImpl::changeValue( const char *value )
{
  m_component->setValue( value );
}

IDeviceGraph *SchematicImpl::getDeviceGraph( const char *tokenId )
{
  foreach( ElementBase *element, m_schemaView->elements() ) // extract ports
    {
      if( 0==std::strcmp( element->classname(), "painter" ) )
        {
          ElementPainter *painter = static_cast<ElementPainter *>(element);
          if( 0 == painter->tokenId().compare( tokenId ) )
            {
              return painter->deviceGraph();
            }
        }
    }
  return 0l;
}

}
