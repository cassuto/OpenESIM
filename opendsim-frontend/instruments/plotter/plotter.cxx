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

#include "instrument-internal.h"

#include "plotterform.h"
#include "inst_plotter.h"

inst_plotter::inst_plotter( int index ) : InstrumentBase( index )
             , m_maxVolt( 500 )
             , m_minVolt( -500 )
             , m_offset( 0 )
             , m_ticksPs( 20 )
             , m_counter( 0 )
             , m_numchan( 0 )
             , m_sample( -1000 )
{
  m_form = new dsim::PlotterForm( this, 0l );

  connect( this, SIGNAL(setSample( int )), m_form, SLOT(onSetSample( int )) );

  connect( this, SIGNAL(printGrid()), m_form, SLOT(onPrintGrid()) );

  connect( this, SIGNAL(printSample()), m_form, SLOT(onPrintSample()) );

  for( int i=0; i<1000; i++ ) clockTick(); // prefill
}

inst_plotter::~inst_plotter(){ delete m_form; }

InstrumentLibraryEntry *inst_plotter::libraryEntry()
{
  return new InstrumentLibraryEntry(
        "plotter",
        inst_plotter::construct);
}

InstrumentBase *inst_plotter::construct( int index ) { return new inst_plotter( index ); }


void inst_plotter::clockTick()
{
  int sample = probeDevice() && probeDevice()->valid() ? probeDevice()->get()->probe_value() * 100.0 : 0;
  if( sample != m_sample )
    {
      m_sample = sample;
      int renderData = sample*1000/(m_maxVolt-m_minVolt)-m_offset;
      emit setSample( renderData );
    }

  if( ++m_counter == m_ticksPs )
    {
      m_counter = 0;
      emit printGrid();
    }
  emit printSample();
}

void inst_plotter::open() { m_form->show(); }
void inst_plotter::close() { m_form->hide(); }

#include "inst_plotter.moc"
