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

#ifndef DOCDATASET_H_
#define DOCDATASET_H_

#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <queue>

#include <dsim/error.h>
#include <dsim/scheme-intr.h>
#include <QList>

namespace dsim
{

class ElementGraphItem; // for ElementGraphItem::id(£©

class DomItem;

enum DomType
{
  DOM_SCHEMA = 0,
  DOM_SCHEMA_SYMBOL
};

class DomNode
{
public:
  DomNode( ds_scheme_synlist_t *synlist );
  DomNode( const DomNode &node );

  void begin();
  bool valid();
  DomNode &next();
  ds_scheme_synlist_t *current();

protected:
  ds_scheme_synlist_t *m_synlist;
  ds_scheme_synlist_t *m_syn_current;
};

class DomEntry : public DomNode
{
public:
  DomEntry( ds_scheme_synlist_t *synlist );
  DomEntry( const DomEntry &entry );

  ds_scheme_synlist_t *current();

private:
  inline ds_scheme_synlist_t *synlist() const { return m_synlist; }
  inline ds_scheme_synlist_t *syn_current() const { return m_syn_current; }
};

class DomDataset
{
public:
  DomDataset( DomType type );
  ~DomDataset();

public:
  int init();
  void uninit();

  DomEntry entry( ds_scheme_synlist_t *node, const char *symbol );
  DomNode node( ds_scheme_synlist_t *node, const char *symbol );

public:
  int serialize( std::ofstream & outstream );

  int ser( const char *val, bool symbol = false );
  int ser( const std::string &val, bool symbol = false );
  int ser( const QList<ElementGraphItem*> &ids );

  template<typename T>
    int ser( const T &val )
      {
        using namespace std;

        stringstream ss;
        ss << val;
        *m_outstream << ss.str() << " ";
        return m_outstream->good() ? 0 : -DS_WRITE_FILE;
      }

  int deserialize( std::ifstream & instream );

  int des( int &val );
  int des( float &val );
  int des( double &val );
  int des( std::string &val, bool symbol = false );
  int des( QList<int> &ids );

  void clear();

  ds_scheme_synlist_t *current();

  int beginEntry( const char *symbol );
  int endEntry();
  int beginNode( const char *symbol = 0 );
  int endNode();

  void deserializePush( ds_scheme_synlist_t *synlist );
  void deserializePop();

  class AutoPop
  {
  public:
    AutoPop( DomDataset *dom ) : m_dom( dom ) {}
    ~AutoPop() { m_dom->deserializePop(); }
  private:
    DomDataset *m_dom;
  };

public:
  void addItem( DomItem *item );
  void removeItem( DomItem *item );

  inline std::ofstream &outstream() const { return *m_outstream; }
  inline std::ifstream &instream() const { return *m_instream; }
  inline std::ifstream *pinstream() const { return m_instream; }
  inline DomType type() const { return m_type; }

private:
  inline ds_scheme_synlist_t *synlist() const { return m_synlist; }

private:
  DomType       m_type;

  std::ofstream *m_outstream;
  std::ifstream *m_instream;

  std::list<DomItem *> m_items;

  ds_scheme_t   *m_scheme;
  ds_scheme_synlist_t *m_synlist;
  ds_scheme_synlist_t *m_synlist_root;

  std::queue<ds_scheme_synlist_t *> m_synqueue;
  int           m_node_deepth;
};

#define SCHEMA_TYPE ("schema")
#define SCHEMA_SYMBOL_TYPE ("ssymbol")
#define SCHEMA_VERSION (1.0)
#define SCHEMA_SYMBOL_VERSION (1.0)

}

#endif //!defined(DOCDATASET_H_)
