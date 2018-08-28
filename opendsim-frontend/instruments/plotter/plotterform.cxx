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

#include "plotterrenderer.h"
#include "plotterform.h"

namespace dsim
{

PlotterForm::PlotterForm( inst_plotter *plotter, InstrumentPanel *parent /*= 0l*/ ) : InstrumentPanel( parent )
            , m_plotter( plotter )
{
  setWindowTitle( tr("Plotter") );
  QVBoxLayout *layout = new QVBoxLayout( this );
  m_renderer = new PlotterRenderer( 1000, 200, this );
  layout->addWidget( m_renderer );
}

void PlotterForm::onSetSample( int sample ) { m_renderer->setSample( sample ); }

void PlotterForm::onPrintGrid() { m_renderer->printGrid(); }

void PlotterForm::onPrintSample() { m_renderer->printSample(); }

}

#include "plotterform.moc"
