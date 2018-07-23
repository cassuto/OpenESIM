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

#define TRACE_UNIT "elementabsport"
#include <dsim/logtrace.h>
#include <cstring>
#include "elementwire.h"
#include "schemaview.h"
#include "elementabstractport.h"

namespace dsim
{

ElementAbstractPort::ElementAbstractPort()
                   : m_connectedWire( 0l )
{
}

ElementAbstractPort::~ElementAbstractPort()
{
  if( connectedWire() ) connectedWire()->disconnectPort( this );
}

void ElementAbstractPort::connectWire( ElementWire *wire )
{
  trace_assert( 0l==m_connectedWire );
  if( wire )
    {
      m_connectedWire = wire;
    }
}

void ElementAbstractPort::disconnectWire( ElementWire *wire )
{
  trace_assert( m_connectedWire );
  m_connectedWire = 0l;

  UNUSED(wire);
}

}

#include "elementpin.h"
#include "elementjoint.h"

namespace dsim
{

ElementAbstractPort *elementabstractport_cast( ElementBase *base )
{
  if( 0==std::strcmp( base->classname(), "pin" ) )
    {
      return static_cast<ElementAbstractPort *>(static_cast<ElementPin *>(base));
    }
  else if( 0==std::strcmp( base->classname(), "jointport" ) )
    {
      return static_cast<ElementAbstractPort *>(static_cast<ElementJointPort *>(base));
    }
  /* add new one here if someone has inherited ElementAbstractPort... */
  return 0l;
}

}
