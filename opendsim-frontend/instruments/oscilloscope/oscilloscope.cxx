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

#include "oscopeform.h"
#include "inst_oscilloscope.h"

inst_oscilloscope::inst_oscilloscope()
                 : m_sampleSize( 0 )
                 , m_samples( 0l )
                 , m_smplCount( 0 )
                 , m_waitingTiggerCount( 0 )
                 , m_stepCount( 0 )
                 , m_updtCount( 0 )
                 , m_Htick( 0 )
                 , m_Hscale( 0 )
                 , m_prevHscale( 0 )
                 , m_Hpos( 0 )
                 , m_prevHpos( 0 )
                 , m_Vscale( 0 )
                 , m_prevVscale( 0 )
                 , m_Vpos( 0 )
                 , m_prevVpos( 0 )
                 , m_ampli( 0 )
                 , m_filter( 0 )
                 , max( 0 )
                 , mid( 0 )
                 , min( 0 )
                 , lastSample( 0 )
                 , Hpos( 0 )
                 , m_freq( 0 )
                 , m_numMax( 0 )
                 , down( false )
                 , up( false )
                 , m_auto( false )
{
  setSampleSize( 512 );
  clear();

  m_form = new dsim::OscopeForm( this, 0l );

  connect( this, SIGNAL(divChanged( double, QString )), m_form, SLOT(onDivChanged( double, QString )) );

  connect( this, SIGNAL(ampliChanged( double )), m_form, SLOT(onAmpliChanged( double )) );

  connect( this, SIGNAL(freqChanged( double )), m_form, SLOT(onFreqChanged( double )) );

  connect( this, SIGNAL(readSamples()), m_form, SLOT(onReadSamples()) );
}

inst_oscilloscope::~inst_oscilloscope()
{
  delete m_form;
  delete [] m_samples;
}

InstrumentLibraryEntry *inst_oscilloscope::libraryEntry()
{
  return new InstrumentLibraryEntry(
        "oscilloscope",
        inst_oscilloscope::construct);
}

InstrumentBase *inst_oscilloscope::construct() { return new inst_oscilloscope(); }

void inst_oscilloscope::open()
{ m_form->show(); }

void inst_oscilloscope::close()
{ m_form->hide(); }

void inst_oscilloscope::clockTick() // asynchronous
{
  double sample = 0;
  if ( probeDevice() ) sample = probeDevice()->probe_value();

  if( sample > max ) max = sample;
  if( sample < min ) min = sample;

  if( (sample-lastSample)>m_filter )                       // Filter noise
    {
      if( state() == OSCOPE_WAITING_TRIGGER )
        {
          mid = min + m_ampli/2;

          if( sample>=mid )                               // Rising edge
            {
              int per = 1e6/m_freq;
              if( per > 1 )
                {
                  if( m_auto )
                    {
                      m_Vpos = mid;
                      m_Hpos = 0;
                      m_Vscale = 5/m_ampli;
                      if( m_Vscale > 1000 ) m_Vscale = 1000;
                      if( m_Vscale < 0.001 ) m_Vscale = 0.001;

                      m_Hscale = (abs(per/70)+1);
                      if( m_Hscale > 10000 ) m_Hscale = 10000;
                      if( m_Hscale < 1 )    m_Hscale = 1;
                    }

                  Hpos = 0;
                  m_Htick = 0;
                  m_smplCount = 0;
                  m_waitingTiggerCount = 0;

                  setState( OSCOPE_SAMPLING );
                }
            }
        }
      if( down & !up )                                    // Min Found
        {
          m_ampli = max-min;
          emit ampMaxMinChanged( max, min );
          down = false;
          max = -1e12;
        }
      up = true;
      lastSample = sample;
    }
  else if( (sample-lastSample) < -m_filter )
    {
      if( up & !down )                                    // Max Found
        {
          m_numMax++;
          m_ampli = max-min;
          emit ampMaxMinChanged( max, min );
          up = false;
          min = 1e12;
        }
      down = true;
      lastSample = sample;
    }

  switch( state() )
  {
    case OSCOPE_WAITING_TRIGGER:
      if( ++m_waitingTiggerCount == 1000500 ) // timeout
        {
          m_ampli = 0;
          m_waitingTiggerCount = 0;
          clear();
        }
      break;

    case OSCOPE_SAMPLING:
      if( ++m_stepCount == 50000 )            // 5 ms Update
        {
          m_stepCount = 0;

          double tick = 20*m_Hscale;
          double val = tick/1e6;
          QString unit = " S";

          if( val < 1 )
            {
              unit = " mS";
              val = tick/1e3;
              if( val < 1 )
                {
                  unit = " uS";
                  val = tick;
                }
            }

          emit divChanged( val, unit );
          emit ampliChanged( m_ampli );

          if( ++m_updtCount >= 20 )                        // 1 Seg Update
            {
              m_freq = m_numMax;
              m_updtCount = 0;
              m_numMax = 0;
              emit freqChanged( m_freq );
            }
          if( m_smplCount == m_sampleSize )   // Data set Ready to display
            {
              emit readSamples();

              setState( OSCOPE_PROCESSING );
              m_smplCount = 0;
              break;
            }
        }
      /*
       * Sampling signal and storing
       */
      if( m_smplCount == m_sampleSize ) break; // buffer is filled, waiting for reading

      if( Hpos < m_Hpos ) Hpos++;
      else
        {
          if( ++m_Htick == m_Hscale )
            {
              m_samples[m_smplCount] = ((sample-m_Vpos)*m_Vscale+2.5)*28;
              m_smplCount++;
              m_Htick = 0;
              if( m_smplCount == m_sampleSize )
                {
                  down = false;
                  up = false;
                }
            }
        }
      break;

    case OSCOPE_PROCESSING:
    default:
      return;
  }
}

void inst_oscilloscope::setSampleSize( int size )
{
  m_sampleSize = size > 32 ? size : 32;
  delete [] m_samples;
  m_samples = new int[m_sampleSize];
  clear();
}

void inst_oscilloscope::clear()
{
  for( int i=0; i<m_sampleSize; i++ ) m_samples[i] = 2.5*28;
  emit readSamples();

  setState( OSCOPE_WAITING_TRIGGER );

  m_waitingTiggerCount = 0;
  m_stepCount = 0;
  m_updtCount = 0;
  m_smplCount = 0;

  emit freqChanged( 0 );
  emit ampliChanged( 0 );
}

#include "inst_oscilloscope.moc"
