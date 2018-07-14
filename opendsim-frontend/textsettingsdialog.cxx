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

#include "textsettingsdialog.h"

namespace dsim
{

TextSettingsDialog::TextSettingsDialog( const QString &text, QWidget *parent )
                      : QDialog( parent )
{
  QHBoxLayout *centralLayout = new QHBoxLayout( this );
  centralLayout->setMargin( 2 );
  centralLayout->setSpacing( 2 );

  QLabel *label = new QLabel( tr("Set Text:"), this );
  m_edit = new QLineEdit( text, this );
  QPushButton *ok = new QPushButton( tr("OK"), this );
  QPushButton *cancel = new QPushButton( tr("Cancel"), this );
  m_edit->selectAll();

  centralLayout->addWidget( label );
  centralLayout->addWidget( m_edit );
  centralLayout->addWidget( ok );
  centralLayout->addWidget( cancel );

  this->setWindowTitle( tr("Settings for text") );
  this->update();

  connect( ok, SIGNAL(pressed()), this, SLOT(accept()) );

  connect( cancel, SIGNAL(pressed()), this, SLOT(reject()) );
}

TextSettingsDialog::~TextSettingsDialog()
{
}

QString TextSettingsDialog::text() const
{
  return m_edit->text();
}

}
