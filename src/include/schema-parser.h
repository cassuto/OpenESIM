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

#ifndef SCHEMA_PARSER_H_
#define SCHEMA_PARSER_H_

#if !defined(__cplusplus)
#error cplusplus only
#endif

typedef struct ds_scheme_s ds_scheme_t;

#include "domdataset.h"

namespace dsim
{

class DomDataset;

class SchemaParser
{
public:
  SchemaParser( ds_scheme_t *sc );

  int init( DomType type );
  int parse( DomDataset *dom );

  inline DomDataset *dom() { return m_dom; }
  inline DomType type() { return m_type; }

private:
  DomDataset    *m_dom;
  ds_scheme_t   *m_sc;
  DomType        m_type;
};

#define SCHEMA_VERSION (1.0)
#define SCHEMA_VERSION_STR "1.0"
#define SCHEMA_TYPE "schema"
#define SCHEMA_SYMBOL_TYPE "ssymbol"
}

#endif //!defined(SCHEMA_PARSER_H_)
