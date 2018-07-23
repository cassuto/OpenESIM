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

#define TRACE_UNIT "IdAlloc"

#include <dsim/logtrace.h>
#include <algorithm>
#include <functional>

#include "idallocator.h"

namespace dsim
{

int IdAllocator::alloc()
{ using namespace std;

  int num = 0, delta = 0;

  for( set<int>::iterator n = m_ids.begin(); n!=m_ids.end(); n++ )
    {
      delta = *n - num;
      if( delta > 1 ) { num = (*n) - delta; break; }
      num = *n;
    }

  ++num;

  m_ids.insert( num );
  return num;
}

void IdAllocator::release( int id )
{ using namespace std;

  set<int>::iterator it = find( m_ids.begin(), m_ids.end(), id );
  if( it != m_ids.end() )
    {
      m_ids.erase( it );
    }
}

void IdAllocator::add( int id )
{ trace_assert( unused( id ) ); m_ids.insert( id ); }

bool IdAllocator::unused( int id )
{ return m_ids.end() == std::find( m_ids.begin(), m_ids.end(), id ); }

void IdAllocator::clear()
{ m_ids.clear(); }

}
