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

#ifndef OSCILLOSCOPE_H_
#define OSCILLOSCOPE_H_

#include <QObject>
#include <QReadWriteLock>
#include <instrument/instrumentbase.h>

enum OscopeState
{
  OSCOPE_WAITING_TRIGGER = 0,
  OSCOPE_SAMPLING,
  OSCOPE_PROCESSING
};

namespace dsim
{
class OscopeForm;
}

class inst_oscilloscope : public QObject, public InstrumentBase
{
  Q_OBJECT
public:
  explicit inst_oscilloscope( int index );
  ~inst_oscilloscope();

  static InstrumentLibraryEntry *libraryEntry();
  static InstrumentBase *construct( int index );

  int syncClockTick();
  void open();
  void close();

  void setSampleSize( int size );
  void clear();

  inline OscopeState state() { m_rwlock.lockForRead(); OscopeState rc = m_state; m_rwlock.unlock(); return rc; }
  inline void setState( OscopeState state ) { m_rwlock.lockForWrite(); m_state = state; m_rwlock.unlock(); }

  inline int sampleSize() const { return m_sampleSize; }
  inline const int *samples() const { return m_samples; }

signals:
  void divChanged( double div, QString unit );
  void ampliChanged( double ampli );
  void freqChanged( double freq );
  void ampMaxMinChanged( double max, double min );
  void readSamples();

public:
  int m_sampleSize;
  int *m_samples;
  int m_smplCount;
  int m_waitingTiggerCount;
  int m_stepCount;
  int m_updtCount;
  int m_Htick;
  int m_Hscale;
  int m_prevHscale;
  int m_Hpos;
  int m_prevHpos;
  double m_Vscale;
  double m_prevVscale;
  double m_Vpos;
  double m_prevVpos;
  double m_ampli;
  double m_filter;

  double max;
  double mid;
  double min;
  double lastSample;
  double Hpos;

  int m_freq;
  int m_numMax;

  bool down;
  bool up;

  bool m_auto;

  QReadWriteLock m_rwlock;
  OscopeState   m_state;
  dsim::OscopeForm *m_form;
};

#endif
