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
  QHBoxLayout *centralLayout = new QHBoxLayout( this );

  m_renderer = new OscopeRenderer( 411, 341, this );
  centralLayout->addWidget( m_renderer );

  QVBoxLayout *pannelLayout = new QVBoxLayout();
  m_divLabel = new QLabel( "Div:  0 S", this );
  m_ampLabel = new QLabel( "Amp: 0.00 V", this );
  m_freqLabel = new QLabel( "Freq: 000 Hz", this );
  pannelLayout->addWidget( m_divLabel );
  pannelLayout->addWidget( m_ampLabel );
  pannelLayout->addWidget( m_freqLabel );

  m_HscaleDial = new QDial( this );
  m_HscaleDial->setFixedSize( 32, 32 );
  m_HscaleDial->setNotchesVisible( true );
  m_HscaleDial->setWrapping(true);
  m_HscaleDial->setMinimum( 1 );
  m_HscaleDial->setMaximum( 20 );
  m_HscaleDial->setSingleStep( 1 );
  pannelLayout->addWidget( m_HscaleDial );

  m_VscaleDial = new QDial( this );
  m_VscaleDial->setFixedSize( 32, 32 );
  m_VscaleDial->setNotchesVisible( true );
  m_VscaleDial->setWrapping( true );
  m_VscaleDial->setMinimum( 1 );
  m_VscaleDial->setMaximum( 20 );
  m_VscaleDial->setSingleStep( 1 );
  pannelLayout->addWidget( m_VscaleDial );

  centralLayout->addLayout( pannelLayout );

  connect(m_HscaleDial, SIGNAL(valueChanged( int )), this, SLOT(onHscaleChanged( int )) );

  connect(m_VscaleDial, SIGNAL(valueChanged( int )), this, SLOT(onVscaleChanged( int )) );
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
  UNUSED(max); UNUSED(min);
}

void OscopeForm::onReadSamples()
{
  m_renderer->setSamples( m_oscope->samples(), m_oscope->sampleSize() );
  m_oscope->setState( OSCOPE_WAITING_TRIGGER );
}

void OscopeForm::onHscaleChanged( int /*value*/ )
{
}

void OscopeForm::onVscaleChanged( int /*value*/ )
{
}

}

#include "oscopeform.moc"
