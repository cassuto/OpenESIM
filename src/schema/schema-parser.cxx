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

#include "Schema-internal.h"

namespace dsim
{

SchemaParser::SchemaParser( ds_scheme_t *sc )
            : m_dom( 0l )
            , m_sc( sc )
{
  lisp_set_opaque( sc, this );
}

int SchemaParser::init( DomType type )
{
  int rc = 0;

  m_type = type;

  rc = lisp_add_symbol( m_sc, "check-format", sch_op_check_format );      UPDATE_RC(rc);
  rc = lisp_add_symbol( m_sc, "add-do", sch_op_add_do );                  UPDATE_RC(rc);

  if( type == DOM_SCHEMA )
    {
      rc = lisp_add_symbol( m_sc, "add-picked-device", sch_op_add_device );   UPDATE_RC(rc);
    }

  return 0;
}

int SchemaParser::parse( DomDataset *dom )
{
  m_dom = dom;
  return lisp_eval( m_sc );
}

}
