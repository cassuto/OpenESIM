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

#include "inst_oscilloscope.h"
#include "oscoperenderer.h"
#include "oscopeform.h"

namespace dsim
{

OscopeForm::OscopeForm( inst_oscilloscope *oscope, InstrumentPanel *parent /*= 0l*/ ) : InstrumentPanel( parent )
           , m_oscope( oscope )
{
  setWindowTitle( tr("Oscilloscope") );
  QHBoxLayout *centralLayout = new QHBoxLayout( this );

  m_renderer = new OscopeRenderer( 411, 341, this );
  centralLayout->addWidget( m_renderer );

  QVBoxLayout *pannelLayout = new QVBoxLayout();
  QGridLayout *readoutLayout = new QGridLayout();
  m_divLabel = new QLabel( "Div:  0 S", this );
  m_ampLabel = new QLabel( "Amp: 0.00 V", this );
  m_freqLabel = new QLabel( "Freq: 000 Hz", this );
  m_Vmax = new QLabel( "Vmax: 0.00 V", this );
  m_Vmin = new QLabel( "Vmin: 0.00 V", this );
  m_coupling = new QLabel( "Couple: AC", this );
  m_autoConfig = new QCheckBox( tr("A"), this );
  m_autoConfig->setChecked( m_oscope->m_auto );
  readoutLayout->addWidget( m_divLabel, 0, 0, 1, 1 );
  readoutLayout->addWidget( m_ampLabel, 1, 0, 1, 1 );
  readoutLayout->addWidget( m_freqLabel, 2, 0, 1, 1 );
  readoutLayout->addWidget( m_Vmax, 0, 1, 1, 1 );
  readoutLayout->addWidget( m_Vmin, 1, 1, 1, 1 );
  readoutLayout->addWidget( m_coupling, 2, 1, 1, 1);
  readoutLayout->addWidget( m_autoConfig, 3, 0, 1, 1 );

  QGroupBox *frame = new QGroupBox( tr("Readout"), this );
  frame->setLayout( readoutLayout );
  pannelLayout->addWidget( frame );

  QGridLayout *encoderTurns = new QGridLayout();
  QLabel *label = new QLabel( tr("Div:"), this );
  encoderTurns->addWidget( label, 0, 0, 1, 1 );

  label = new QLabel( tr("TIME"), this );
  encoderTurns->addWidget( label, 0, 1, 1, 1 );
  label = new QLabel( tr("VOLT"), this );
  encoderTurns->addWidget( label, 0, 2, 1, 1 );

  m_HscaleDial = new QDial( this );
  m_HscaleDial->setFixedSize( 64, 64 );
  m_HscaleDial->setNotchesVisible( true );
  m_HscaleDial->setWrapping(true);
  m_HscaleDial->setMinimum( 1 );
  m_HscaleDial->setMaximum( 20 );
  m_HscaleDial->setSingleStep( 1 );
  encoderTurns->addWidget( m_HscaleDial, 1, 1, 1, 1 );

  m_VscaleDial = new QDial( this );
  m_VscaleDial->setFixedSize( 64, 64 );
  m_VscaleDial->setNotchesVisible( true );
  m_VscaleDial->setWrapping( true );
  m_VscaleDial->setMinimum( 1 );
  m_VscaleDial->setMaximum( 20 );
  m_VscaleDial->setSingleStep( 1 );
  encoderTurns->addWidget( m_VscaleDial, 1, 2, 1, 1 );

  label = new QLabel( tr("Pos:"), this );
  encoderTurns->addWidget( label, 2, 0, 1, 1 );

  label = new QLabel( tr("X"), this );
  encoderTurns->addWidget( label, 2, 1, 1, 1 );
  label = new QLabel( tr("Y"), this );
  encoderTurns->addWidget( label, 2, 2, 1, 1 );

  m_HposDial = new QDial( this );
  m_HposDial->setFixedSize( 64, 64 );
  m_HposDial->setNotchesVisible( true );
  m_HposDial->setWrapping(true);
  m_HposDial->setMinimum( 1 );
  m_HposDial->setMaximum( 20 );
  m_HposDial->setSingleStep( 1 );
  encoderTurns->addWidget( m_HposDial, 3, 1, 1, 1 );

  m_VposDial = new QDial( this );
  m_VposDial->setFixedSize( 64, 64 );
  m_VposDial->setNotchesVisible( true );
  m_VposDial->setWrapping( true );
  m_VposDial->setMinimum( 1 );
  m_VposDial->setMaximum( 20 );
  m_VposDial->setSingleStep( 1 );
  encoderTurns->addWidget( m_VposDial, 3, 2, 1, 1 );

  frame = new QGroupBox( tr("Adj."), this );
  frame->setLayout( encoderTurns );
  pannelLayout->addWidget( frame );

  centralLayout->addLayout( pannelLayout );

  connect( m_HscaleDial, SIGNAL(valueChanged( int )), this, SLOT(onHscaleChanged( int )) );

  connect( m_VscaleDial, SIGNAL(valueChanged( int )), this, SLOT(onVscaleChanged( int )) );

  connect( m_HposDial, SIGNAL(valueChanged( int )), this, SLOT(onHposChanged( int )) );

  connect( m_VposDial, SIGNAL(valueChanged( int )), this, SLOT(onVposChanged( int )) );

  connect( m_autoConfig, SIGNAL(stateChanged( int )), this, SLOT(onAutoConfigChanged( int )) );
}

void OscopeForm::onDivChanged( double div, QString unit )
{
  m_divLabel->setText( "Div:  "+QString::number( div,'f', 2 )+unit );
}

void OscopeForm::onAmpliChanged( double ampli )
{
  m_ampLabel->setText(  "Amp: " +QString::number( ampli,'f', 2 )+" V" );
}

void OscopeForm::onFreqChanged( double freq )
{
  m_freqLabel->setText( "Freq: "+QString::number( freq )+" Hz" );
}

void OscopeForm::onAmpMaxMinChanged( double max, double min )
{
  m_Vmax->setText( " Vmax: " +QString::number( max, 'f', 2 )+" V" );
  m_Vmin->setText( " Vmin: " +QString::number( min, 'f', 2 )+" V" );
}

void OscopeForm::onReadSamples()
{
  m_renderer->setSamples( m_oscope->samples(), m_oscope->sampleSize() );
  m_oscope->setState( OSCOPE_WAITING_TRIGGER );
}

void OscopeForm::onHscaleChanged( int value )
{
  m_oscope->m_auto = false;
  m_autoConfig->setChecked( false );

  if( value < m_oscope->m_prevHscale )
    {
      m_oscope->m_Hscale++;
      if( m_oscope->m_Hscale > 10000 ) m_oscope->m_Hscale = 10000;
    }
  else
    {
      m_oscope->m_Hscale--;
      if( m_oscope->m_Hscale < 1 ) m_oscope->m_Hscale = 1;
    }
  m_oscope->m_prevHscale = value;
}

void OscopeForm::onVscaleChanged( int value )
{
  m_oscope->m_auto = false;
  m_autoConfig->setChecked( false );

  double Vscale = (double)value;
  if( Vscale > m_oscope->m_prevVscale )
    {
      m_oscope->m_Vscale *= 1.005;
      if( m_oscope->m_Vscale > 1000 ) m_oscope->m_Vscale = 1000;
    }
  else
    {
      m_oscope->m_Vscale /= 1.005;
      if( m_oscope->m_Vscale < 0.001 ) m_oscope->m_Vscale = 0.001;
    }
  m_oscope->m_prevVscale = Vscale;
}

void OscopeForm::onHposChanged( int value )
{
  m_oscope->m_auto = false;
  m_autoConfig->setChecked( false );

  if( value < m_oscope->m_prevHpos )
    {
      m_oscope->m_Hpos += m_oscope->m_Hscale;
      if( m_oscope->m_Hpos > 200*m_oscope->m_Hscale ) m_oscope->m_Hpos = 200*m_oscope->m_Hscale;
    }
  else
    {
      m_oscope->m_Hpos -= m_oscope->m_Hscale;
      if( m_oscope->m_Hpos < 0 ) m_oscope->m_Hpos = 0;
    }
  m_oscope->m_prevHpos = value;
}

void OscopeForm::onVposChanged( int value )
{
  m_oscope->m_auto = false;
  m_autoConfig->setChecked( false );

  double Vpos = (double)value;

  if( Vpos < m_oscope->m_prevVpos )
    {
      m_oscope->m_Vpos += 0.01*m_oscope->m_Vscale;
    }
  else
    {
      m_oscope->m_Vpos -= 0.01*m_oscope->m_Vscale;
    }
  m_oscope->m_prevVpos = Vpos;
}


void OscopeForm::onAutoConfigChanged( int checked )
{
  m_oscope->m_auto = checked;
}

}

#include "oscopeform.moc"
