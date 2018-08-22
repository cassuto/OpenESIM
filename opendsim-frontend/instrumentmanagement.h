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

#ifndef INSTRUMENTMANAGEMENT_H_
#define INSTRUMENTMANAGEMENT_H_

#include <QList>
#include <dsim/misc.h>
#include <dsim/rbtree.h>

class InstrumentLibraryEntry;
class InstrumentBase;

namespace dsim
{

class InstrumentPair
{
public:
  InstrumentPair( InstrumentLibraryEntry *entry, InstrumentBase *base ) : entry(entry), base(base)
{}
  InstrumentLibraryEntry *entry;
  InstrumentBase *base;

  bool operator==( const InstrumentPair &pair ) { return pair.entry == entry && pair.base == base; } // explicit
};

class InstrumentManagement
{
public:
  InstrumentManagement();

  int addInstrument( InstrumentLibraryEntry *entry, InstrumentBase DS_OUT **instance );
  int addInstrument( const char *classname, InstrumentBase DS_OUT **instance );
  void removeInstrument( const InstrumentPair &pair );
  rb_tree_t *instrumentTree() const;
  inline const QList<InstrumentPair> & instruments() const { return m_insts; }
  void clockTick();

private:
  QList<InstrumentPair> m_insts;
};

}

#endif
