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

#ifndef INSTPLOTTER_H
#define INSTPLOTTER_H

#include <QObject>
#include <instrument/instrumentbase.h>

namespace dsim
{
class PlotterForm;
}
class inst_plotter : public QObject, public InstrumentBase
{
  Q_OBJECT
public:
  explicit inst_plotter( int index );
  ~inst_plotter();

  static InstrumentLibraryEntry *libraryEntry();
  static InstrumentBase *construct( int index );

  int clockTick();
  void open();
  void close();

signals:
  void setSample( int sample );
  void printGrid();
  void printSample();

private:
  dsim::PlotterForm *m_form;
  int m_maxVolt;
  int m_minVolt;
  int m_offset;
  int m_ticksPs;
  int m_counter;
  int m_numchan;
  int m_sample;
};

#endif

