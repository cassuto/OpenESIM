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

#include <dsim/error.h>
#include <dsim/scheme-intr.h>

class DeviceLibraryEntry;

namespace dsim
{

class DomItem;
class DomPool;

class SchemaView;

enum DomType
{
  DOM_SCHEMA = 0,
  DOM_SCHEMA_SYMBOL
};

class DomDataset
{
public:
  DomDataset( DomPool *dompool );
  ~DomDataset();

public:
  inline void ref() { refcount++; }
  int release();

public:
  int serialize( DomType type, std::ofstream & outstream );

  int ser( const std::string &val );

  template<typename T>
    int ser( const T &val )
      {
        using namespace std;

        stringstream ss;
        ss << val;
        *m_outstream << ss.str() << " ";
        return m_outstream->good() ? 0 : -DS_WRITE_FILE;
      }

  int deserialize( DomType type, std::ifstream & instream );

  int des( int &val );
  int des( float &val );
  int des( double &val );
  int des( std::string &val );

  void clear();

  void beginDeserialize( ds_scheme_synlist_t *synlist );
  void endDeserialize();

public:
  const std::list<const DeviceLibraryEntry *> &pickedDevices() const;

  void addPickedDevice( const DeviceLibraryEntry *entry );
  void addItem( DomItem *item );

public:
  inline void setSchemaView( SchemaView *view  ) { m_schemaView = view; }

  inline SchemaView *schemaView() { return m_schemaView; }

private:
  inline ds_scheme_synlist_t *synlist() { return m_synlist; }

private:
  int           refcount;

  std::ofstream *m_outstream;
  std::ifstream *m_instream;

  std::list<const DeviceLibraryEntry *> m_pickedDevices;
  std::list<DomItem *> m_items;

  DomPool      *m_dompool;

  ds_scheme_synlist_t *m_synlist;

  SchemaView   *m_schemaView;
};

}

#endif //!defined(DOCDATASET_H_)
