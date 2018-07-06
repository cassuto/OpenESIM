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

#define TRACE_UNIT "dompool"

#include <dsim/logtrace.h>
#include <dsim/memory.h>
#include <dsim/node-template.h>
#include "domdataset.h"

#include "dompool.h"

namespace dsim
{

DomPool::DomPool()
{
  list_init( &m_domList );
}

int DomPool::addDOM( DomDataset *dom )
{
  int rc = -DS_FAULT;

  if( !list_contains_voidptr( &m_domList, dom ) )
    {
      if( !(rc = list_insert_voidptr( &m_domList, dom )) )
        dom->ref();
    }
  return rc;
}

void DomPool::deleteDOMs()
{
  foreach_list( list_voidptr_node_t, node, &m_domList )
    {
      DomDataset *dom = (DomDataset *)(node->val);
      int count = dom->release();

      trace_assert( count == 0 ); // ensure no one has referenced this DOM
    }
  list_clear( &m_domList, ds_heap_free );
}

}
