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

#include <QtWidgets>

#include "componentpicktree.h"
#include "componentpickwidget.h"

namespace dsim
{

ComponentPickWidget::ComponentPickWidget( QWidget *parent )
  : QDockWidget( parent )
{
  QWidget *central = new QWidget( this );
  QVBoxLayout *layout = new QVBoxLayout( central );

  filterText = new QLineEdit( central );
  filterText->setPlaceholderText( tr("Filter...") );
  layout->addWidget( filterText );

  componentPickTree = new ComponentPickTree( central );
  componentPickTree->addCategory(tr("Components"));
  componentPickTree->addComponent("1N4148", "Components", "");
  layout->addWidget( componentPickTree );

  layout->setMargin( 0 );
  central->setLayout( layout );

  this->setWidget( central );
  this->setWindowTitle(tr("Pick a device"));
}

ComponentPickWidget::~ComponentPickWidget()
{
}

}
