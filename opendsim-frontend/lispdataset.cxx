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

#define TRACE_UNIT "dataset"

#include <dsim/logtrace.h>
#include <device/libentry.h>
#include <dsim/device-lib.h>
#include <dsim/scheme-intr.h>
#include <dsim/error.h>
#include <dsim/misc.h>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <cstdio>

#include "elementgraphitem.h" // for ElementBase::id()

#include "domitem.h"
#include "lispdataset.h"

namespace dsim
{

LispDataset::LispDataset( DomType type )
          : m_type( type )
          , m_outstream( 0l )
          , m_instream( 0l )
          , m_scheme( 0l )
          , m_synlist( 0l )
          , m_synlist_root( 0l )
          , m_node_deepth( 0l )
{
}

LispDataset::~LispDataset()
{ uninit(); }

extern "C"
  int stream_read( ds_scheme_t *sc, char *buff, int maxsize )
    {
      std::ifstream *stream = (std::ifstream *)lisp_get_stream_opaque(sc);
      stream->read( buff, maxsize );
      if( stream->bad() ) return -1;
      return stream->gcount() > 0 ? stream->gcount() : -1;
    }

int LispDataset::init()
{
  uninit();
  m_scheme = lisp_create( stream_read, 0l );
  return m_scheme ? 0 : -DS_NO_MEMORY;
}

/**
 * Find an entry in pair.
 * eg. node = ( (sym1 (node1 ... ) (node2 ... ) ) (sym2 ... ) )
 *     entry(node, "sym1") returns the entry contains {(node1 ... ), (node2 ... )}.
 * @param node      Where to search the entry, must be a pair.
 * @param symbol    The symbol name of target entry.
 * @return Matched DOM entry.
 */
DomEntry LispDataset::entry( ds_scheme_synlist_t *node, const char *symbol )
{
  if( 0l == node ) // from root
    {
      node = m_synlist_root;

      while( node )
        {
          DomEntry domn = entry( node, symbol );
          if( domn.valid() ) return domn;
          node = node->next;
        }
      return DomEntry(0l);
    }
  else
    {
      if( node->synnode.type != SCHEME_VAL_PAIR )
        return DomEntry(0l);

      node = node->child;
      while( node )
        {
          if( node->synnode.type == SCHEME_VAL_PAIR )
            {
              ds_scheme_synlist_t *sym = node->child;
              if( sym )
                {
                  if( sym->synnode.type == SCHEME_VAL_SYMBOL )
                    {
                      if( 0==std::strcmp( lispval_string(sym), symbol ) )
                        {
                          m_synlist = node; // move current position
                          return DomEntry( sym->next );
                        }
                    }
                }
            }
          node = node->next;
        }
    }
  return DomEntry(0l);
}

DomEntry::DomEntry()
         : m_synlist(0l)
         , m_syn_current(0l)
{}

DomEntry::DomEntry( ds_scheme_synlist_t *synlist )
         : m_synlist( synlist )
         , m_syn_current( m_synlist )
{}

DomEntry::DomEntry( const DomEntry &node )
         : m_synlist( node.m_synlist )
         , m_syn_current( node.m_syn_current )
{}

void DomEntry::begin()
{ m_syn_current = m_synlist; }

bool DomEntry::valid()
{ return m_syn_current != 0l; }

DomEntry &DomEntry::next()
{ m_syn_current = m_syn_current->next; return *this; }

ds_scheme_synlist_t *DomEntry::child()
{ return m_syn_current->child; }

ds_scheme_synlist_t *DomEntry::leaf()
{ return m_syn_current; }

void LispDataset::addItem( DomItem *item )
{ m_items.push_front( item ); }

void LispDataset::removeItem( DomItem *item )
{
  using namespace std;
  list<DomItem *>::iterator it = find( m_items.begin(), m_items.end(), item );
  trace_assert( it != m_items.end() );

  m_items.erase( it );
}

int LispDataset::serialize( std::ofstream & outstream )
{ using namespace std;

  int rc = 0;

  m_outstream = &outstream;

  /*
   * Serialize the format and version infomation
   */
  rc = beginNode( false, "format" );                    UPDATE_RC(rc);
  {
    switch( type() )
    {
      case DOM_SCHEMA:
        rc = ser( SCHEMA_TYPE, /*symbol*/true );        UPDATE_RC(rc);
        rc = ser( SCHEMA_VERSION );                     UPDATE_RC(rc);
        break;

      case DOM_SCHEMA_SYMBOL:
        rc = ser( SCHEMA_SYMBOL_TYPE, /*symbol*/true ); UPDATE_RC(rc);
        rc = ser( SCHEMA_SYMBOL_VERSION );              UPDATE_RC(rc);
        break;

      default:
        return -DS_UNIMPLEMENTED;
    }
  }
  rc = endNode();                                       UPDATE_RC(rc);

  /*
   * Serialize registered DOM items
   */
  for( std::list<DomItem *>::iterator it = m_items.begin(); it!=m_items.end(); it++ )
    {
      if( (rc = (*it)->serialize( this ) ) )
        return rc;
    }

  return rc;
}

int LispDataset::deserialize( std::ifstream & instream )
{
  int rc = 0;
  m_instream = &instream;

  lisp_set_stream_opaque( m_scheme, m_instream );

  if( (rc = lisp_lex_all( m_scheme, &m_synlist_root )) )
    return rc;

  DomEntry ent = entry( 0, "format" );
  if( ent.valid() )
    {
      this->deserializePush( ent.leaf() );
      {
        LispDataset::AutoPop autoPop( this );

        /*
         * Validate the format and version
         */
        std::string format;

        rc = des( format, /*symbol*/true );             UPDATE_RC(rc);
        switch( type() )
          {
            case DOM_SCHEMA:
              if ( format.compare( SCHEMA_TYPE )  ) // validate the type of schema file
                return -DS_INVALID_FILE_TYPE;
              break;

            case DOM_SCHEMA_SYMBOL:
              if ( format.compare( SCHEMA_SYMBOL_TYPE ) ) // validate the type of schema file
                return -DS_INVALID_FILE_TYPE;
              break;

            default:
              return -DS_UNIMPLEMENTED;
          }

        float version;

        rc = des( version );                            UPDATE_RC(rc);

        if ( version > SCHEMA_VERSION ) // validate the version number of schema file
          return -DS_INVALID_FILE_TYPE;
      } // auto pop
    }
  else
    return -DS_INVALID_FILE_TYPE;

  /*
   * Deserialize registered DOM items
   */
  for( std::list<DomItem *>::iterator it = m_items.begin(); it!=m_items.end(); it++ )
    {
      if( (rc = (*it)->deserialize( this ) ) )
        return rc;
    }

  return 0;
}

/**
 * Serialize the ID of elements as a pair
 * @param ids Reference to the List that stores the pointers of elements
 * @return status code
 */
int LispDataset::ser( const QList<ElementBase *> &ids )
{
  *m_outstream << "(";
  foreach( ElementBase *element, ids )
    {
      *m_outstream << element->id() << " ";
      if( m_outstream->bad() ) return -DS_WRITE_FILE;
    }
  *m_outstream << ") ";
  return m_outstream->good() ? 0 : -DS_WRITE_FILE;
}

int LispDataset::ser( const char *val, bool symbol )
{
  if( symbol )
    {
      *m_outstream << val << " ";
    }
  else
    {
      *m_outstream << "\"" << val << "\" ";
    }
  return m_outstream->good() ? 0 : -DS_WRITE_FILE;
}

int LispDataset::ser( const std::string &val, bool symbol )
{
  return ser( val.c_str(), symbol );
}

int LispDataset::ser( const bool &val )
{
  return ser( val ? "#t" : "#f", /*symbol*/ true );
}

#define DES_TEMPLATE(_val, _schemetype, _schemeval, _ctype) \
do { \
  if( current() ) \
    { \
      if ( synlst_check_type_ptr( current(), _schemetype ) ) \
        { \
          _val = (_ctype)( _schemeval( current() ) ); \
          m_synlist = m_synlist->next; \
        } \
      else \
        return -DS_TYPE_MISMATCH; \
    } \
  else \
    return -DS_FAULT; \
} while(0)

int LispDataset::des( int &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_NUMBER, lispval_number, int); return 0;
}

int LispDataset::des( float &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_NUMBER, lispval_number, float); return 0;
}

int LispDataset::des( double &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_NUMBER, lispval_number, double); return 0;
}

int LispDataset::des( bool &val )
{
  DES_TEMPLATE(val, SCHEME_VAL_BOOLEAN, lispval_boolean, bool); return 0;
}

int LispDataset::des( std::string &val, bool symbol )
{
  const char *buff;

  if( symbol )
    {
      DES_TEMPLATE(buff, SCHEME_VAL_SYMBOL, lispval_symbol, const char *);
    }
  else
    {
      DES_TEMPLATE(buff, SCHEME_VAL_STRING, lispval_string, const char *);
    }
  val.assign( buff );
  return 0;
}

#define DES_LIST_TEMPLATE(_listclear, _elemtype, _schemaval, _ctype, _listappend ) \
do { \
  if( !current() ) return -DS_FAULT; \
    if( synlst_check_type_ptr( current(), SCHEME_VAL_PAIR ) ) \
      { \
        _listclear(); \
        ds_scheme_synlist_t *node = current()->child; \
        while( node ) \
          { \
            if( synlst_check_type_ptr( node, _elemtype ) ) \
              { \
                _listappend( (_ctype)( _schemaval(node) ) ); \
              } \
            else \
              return -DS_TYPE_MISMATCH; \
            node = node->next; \
          } \
      } \
    else \
      return -DS_TYPE_MISMATCH; \
    m_synlist = m_synlist->next; \
} while(0)

int LispDataset::des( QList<int> &ids )
{
  DES_LIST_TEMPLATE( ids.clear, SCHEME_VAL_NUMBER, lispval_number, int, ids.append );
  return 0;
}

int LispDataset::des( DomEntry &ent, const char *symbol )
{
  if( current()->synnode.type == SCHEME_VAL_PAIR )
    {
      if( current()->child && current()->child->synnode.type == SCHEME_VAL_SYMBOL )
        {
          if( 0==std::strcmp( lispval_symbol(current()->child), symbol ) )
            {
              ent = DomEntry( current()->child->next );
              m_synlist = current()->next;
              return 0;
            }
        }
      else if( current()->child && symbol[0] == '\0' )
        {
          ent = DomEntry( current()->child );
          m_synlist = current()->next;
          return 0;
        }
    }

  return -DS_TYPE_MISMATCH;
}

void LispDataset::clear()
{ m_items.clear(); }

int LispDataset::beginEntry( const char *symbol )
{
  ++m_node_deepth;
  outstream() << "(" << symbol << "\n"; return outstream().good() ? 0 : -DS_WRITE_FILE;
}
int LispDataset::endEntry()
{
  --m_node_deepth;
  trace_assert( m_node_deepth >= 0 );

  outstream() << ")\n"; return outstream().good() ? 0 : -DS_WRITE_FILE;
}
int LispDataset::beginNode( bool sub, const char *symbol )
{
  if( !sub )
    for( int i=0; i < m_node_deepth * 2; i++ )
      {
        outstream() << " ";
      }
  if( symbol ) outstream() << "(" << symbol << " ";
  else         outstream() << "(";
  return outstream().good() ? 0 : -DS_WRITE_FILE;
}
int LispDataset::endNode( bool sub )
{
  if( sub )
    {
      outstream() << ") "; return outstream().good() ? 0 : -DS_WRITE_FILE;
    }
  else
    {
      outstream() << ")\n"; return outstream().good() ? 0 : -DS_WRITE_FILE;
    }
}

void LispDataset::deserializePush( ds_scheme_synlist_t *synlist )
{
  m_synqueue.push( m_synlist );
  m_synlist = synlist;
}

void LispDataset::deserializePop()
{
  m_synlist = m_synqueue.top();
  m_synqueue.pop();
}

void LispDataset::uninit()
{
  if( m_synlist_root ) lisp_synlist_free( m_synlist_root );
  if( m_scheme ) lisp_release( m_scheme );

  m_synlist_root = 0l;
  m_synlist = 0l;
  m_scheme = 0l;
}

}
