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

#ifndef OSCOPEFORM_H_
#define OSCOPEFORM_H_

#include <instrument/instrumentpanel.h>

class inst_oscilloscope;

namespace dsim
{

class OscopeRenderer;
class OscopeForm : public InstrumentPanel
{
  Q_OBJECT
public:
  OscopeForm( inst_oscilloscope *oscope, InstrumentPanel *parent = 0l );

private slots: // from class OscilloscopeInst
  void onDivChanged( double div, QString unit );
  void onAmpliChanged( double ampli );
  void onFreqChanged( double freq );
  void onAmpMaxMinChanged( double max, double min );
  void onReadSamples();

private slots:
  void onHscaleChanged( int value );
  void onVscaleChanged( int value );
  void onHposChanged( int value );
  void onVposChanged( int value );
  void onAutoConfigChanged( int checked );

private:
  inst_oscilloscope *m_oscope;
  OscopeRenderer *m_renderer;
  QLabel         *m_divLabel;
  QLabel         *m_ampLabel;
  QLabel         *m_freqLabel;
  QLabel         *m_Vmax;
  QLabel         *m_Vmin;
  QLabel         *m_coupling;
  QCheckBox      *m_autoConfig;
  QDial          *m_HscaleDial;
  QDial          *m_VscaleDial;
  QDial          *m_HposDial;
  QDial          *m_VposDial;
};

}

#endif
