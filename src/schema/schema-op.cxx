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
#include <dsim/device-lib.h>

#include "elementgraphitem.h"
#include "schemaview.h"
#include "domdataset.h"

#include "schema-internal.h"

namespace dsim
{

#define GET_SCH(_sc, _dom) SchemaParser *_dom = (SchemaParser*)lisp_get_opaque( _sc )

int
sch_op_check_format( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out )
{
  GET_SCH( sc, sch );

  if ( synlst_check_type_ptr( in, SCHEME_VAL_SYMBOL ) )
    {
      switch( sch->type() )
      {
        case DOM_SCHEMA:
          if ( std::strcmp( lispval_symbol(in), SCHEMA_TYPE ) ) // validate the type of schema file
            return schema_throw( err_sch_invalid_format );
          break;

        case DOM_SCHEMA_SYMBOL:
          if ( std::strcmp( lispval_symbol(in), SCHEMA_SYMBOL_TYPE ) ) // validate the type of schema file
            return schema_throw( err_sch_invalid_format );
          break;
      }
    }
  else
    return schema_throw( err_sch_syntax );

  in = in->next;

  if ( synlst_check_type_ptr( in, SCHEME_VAL_NUMBER ) )
    {
      if ( lispval_number(in) > SCHEMA_VERSION ) // validate the version number of schema file
        return schema_throw( err_sch_invalid_format );
    }
  else
    return schema_throw( err_sch_syntax );

  return 0;
}

int
sch_op_add_device( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out )
{
  using namespace dsim;
  GET_SCH( sc, sch );

  if ( synlst_check_type_ptr( in, SCHEME_VAL_STRING ) )
    {
      const DeviceLibraryEntry *entry = device_lib_entry( lispval_string(in) );
      if( entry )
        {
          sch->dom()->addPickedDevice( entry );
          return 0;
        }
      else
        {
          return schema_throw( err_sch_invalid_device_symbol );
        }
    }
  else
    return schema_throw( err_sch_syntax );

  return 0;
}

int
sch_op_add_do( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out )
{
  using namespace dsim;
  GET_SCH( sc, sch );
  int rc = 0;

  if ( synlst_check_type_ptr( in, SCHEME_VAL_STRING ) )
    {
      const char *classname = lispval_string(in);
      ElementGraphItem *elem = sch->dom()->schemaView()->createElement( classname, QPoint(0, 0), /*deser*/ true );
      if( elem )
        {
          sch->dom()->beginDeserialize( in->next );

          if( (rc = elem->deserialize( sch->dom() )) )
            {
              delete elem;
            }

          sch->dom()->endDeserialize();
        }
      else
        {
          return schema_throw( err_sch_invalid_element_symbol );
        }
    }
  else
    return schema_throw( err_sch_syntax );

  return rc;
}

}
