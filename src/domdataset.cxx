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

#define TRACE_UNIT "domdataset"

#include <dsim/logtrace.h>
#include <device/libentry.h>
#include <dsim/device-lib.h>
#include <dsim/scheme-intr.h>
#include <dsim/error.h>
#include <dsim/misc.h>
#include <cstdlib>
#include <sstream>

#include "schema-parser.h"
#include "dompool.h"
#include "domitem.h"
#include "domdataset.h"

namespace dsim
{

DomDataset::DomDataset( DomPool *dompool_ )
          : refcount( 0 )
          , m_outstream( 0l )
          , m_instream( 0l )
          , m_dompool( dompool_ )
          , m_synlist( 0l )
          , m_schemaView( 0l )
{
  m_dompool->addDOM( this );
}

DomDataset::~DomDataset()
{
  trace_assert( refcount == 0 );
}

int DomDataset::release()
{
  trace_assert( refcount > 0); refcount--;
  if( refcount == 0 ) delete this;
  return refcount;
}

void DomDataset::addPickedDevice( const DeviceLibraryEntry *entry )
{
  m_pickedDevices.push_front( entry );
}

void DomDataset::addItem( DomItem *item )
{
  m_items.push_front( item );
}

int DomDataset::ser( const std::string &val )
{
  *m_outstream << "\"" << val << "\" ";
  return m_outstream->good() ? 0 : -DS_WRITE_FILE;
}

int DomDataset::serialize( DomType type, std::ofstream & outstream )
{ using namespace std;

  int rc = 0;

  m_outstream = &outstream;

  switch( type )
  {
    case DOM_SCHEMA:
      outstream << "(check-format " SCHEMA_TYPE " " SCHEMA_VERSION_STR ")\n";
      break;

    case DOM_SCHEMA_SYMBOL:
      outstream << "(check-format " SCHEMA_SYMBOL_TYPE " " SCHEMA_VERSION_STR ")\n";
      break;
  }

  if( outstream.bad() ) return -DS_WRITE_FILE;

  /*
   * Serialize picked device list
   */
  for( list<const DeviceLibraryEntry *>::iterator it = m_pickedDevices.begin(); it!=m_pickedDevices.end() ; it++ )
    {
      const DeviceLibraryEntry *entry = *it;
      outstream << "(add-picked-device \"" << entry->symbol_name << "\")\n";

      if( outstream.bad() ) return -DS_WRITE_FILE;
    }

  /*
   * Serialize DomItems
   */
  for( list<DomItem *>::iterator it = m_items.begin(); it!=m_items.end() ; it++ )
    {
      DomItem *item = *it;

      outstream << "(add-do \"" << item->classname() << "\" ";

      if( outstream.bad() ) return -DS_WRITE_FILE;

      if( (rc = item->serialize( this )) )
        return rc;

      outstream << ")\n";
    }
  return rc;
}

extern "C"
  int stream_read( ds_scheme_t *sc, char *buff, int maxsize )
    {
      std::ifstream *stream = (std::ifstream *)lisp_get_stream_opaque(sc);
      stream->read( buff, maxsize );
      if( stream->bad() ) return -1;
      return stream->gcount() > 0 ? stream->gcount() : -1;
    }

int DomDataset::deserialize( DomType type, std::ifstream & instream )
{
  using namespace std;

  int rc = -DS_NO_MEMORY;

  ds_scheme_t *scheme = lisp_create( stream_read, &instream );
  if( scheme )
    {
      clear();

      SchemaParser schema_parser( scheme );
      if( !(rc = schema_parser.init( type )) )
        rc = schema_parser.parse( this );

      lisp_release( scheme );
    }
  return rc;
}

#define DES_TEMPLATE(_val, _schemetype, _schemeval, _ctype) \
do { \
  if( synlist() ) \
    { \
      if ( synlst_check_type_ptr( synlist(), _schemetype ) ) \
        { \
          _val = (_ctype)( _schemeval( synlist() ) ); \
          m_synlist = m_synlist->next; \
        } \
      else \
        return -DS_TYPE_MISMATCH; \
    } \
  else \
    return -DS_FAULT; \
} while(0)

int DomDataset::des( int &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_NUMBER, lispval_number, int); return 0;
}

int DomDataset::des( float &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_NUMBER, lispval_number, float); return 0;
}

int DomDataset::des( double &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_NUMBER, lispval_number, double); return 0;
}

int DomDataset::des( std::string &val )
{
  const char *buff;

  DES_TEMPLATE(buff, SCHEME_VAL_STRING, lispval_string, const char *);

  val.assign( buff );
  return 0;
}

const std::list<const DeviceLibraryEntry *> &DomDataset::pickedDevices() const
{
  return m_pickedDevices;
}

void DomDataset::clear()
{
  m_pickedDevices.clear();
  m_items.clear();
}

void DomDataset::beginDeserialize( ds_scheme_synlist_t *synlist )
{
  m_synlist = synlist;
}

void DomDataset::endDeserialize()
{
  m_synlist = 0l;
}

}
