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

#include <instrument/instrumentbase.h>
#include <instrument/libentry.h>
#include "instrumentmanagement.h"
#include "instrumentrackcellwidget.h"

namespace dsim
{

InstrumentRackCellWidget::InstrumentRackCellWidget( const InstrumentPair &inst, int index, QWidget *parent /*= 0l*/ )
                   : QWidget( parent )
                   , m_inst( inst )
                   , m_index( index )
{
  QGridLayout *layout = new QGridLayout( this );

  m_nameLabel = new QLabel( inst.entry->classname, this );
  QFont f = m_nameLabel->font();
  f.setItalic( true );
  f.setPixelSize( 16 );
  m_nameLabel->setFont( f );
  m_nameLabel->setPixmap( QPixmap(":/bitmaps/modeinst.png") );
  layout->addWidget( m_nameLabel );
}

}
