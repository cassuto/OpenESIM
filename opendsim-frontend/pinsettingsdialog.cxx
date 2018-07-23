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

#include "elementpin.h"
#include "pinsettingsdialog.h"

namespace dsim
{

PinSettingsDialog::PinSettingsDialog( const ElementPin *pin, QWidget *parent )
                  : QDialog( parent )
{
  QGridLayout *centralLayout = new QGridLayout( this );

  QLabel *label = new QLabel( tr("Set Symbol:"), this );
  centralLayout->addWidget( label, 0, 0, 1, 1 );
  m_symbolEdit = new QLineEdit( pin->symbol(), this );
  m_symbolEdit->selectAll();
  centralLayout->addWidget( m_symbolEdit, 0, 1, 1, 1 );
  label = new QLabel( tr("Set Reference:"), this );
  centralLayout->addWidget( label, 1, 0, 1, 1 );
  m_referenceEdit = new QLineEdit( pin->reference(), this );
  m_referenceEdit->selectAll();
  centralLayout->addWidget( m_referenceEdit, 1, 1, 1, 1 );
  label = new QLabel( tr("Set Length:"), this );
  centralLayout->addWidget( label, 2, 0, 1, 1 );
  m_lengthSpin = new QSpinBox( this );
  m_lengthSpin->setRange( 1, 60 );
  m_lengthSpin->setValue( pin->length() );
  centralLayout->addWidget( m_lengthSpin, 2, 1, 1, 1 );
  m_showSymbol = new QCheckBox( tr("Show Symbol?"), this );
  m_showSymbol->setChecked( pin->showSymbol() );
  centralLayout->addWidget( m_showSymbol, 3, 0, 1, 1 );
  m_showReference = new QCheckBox( tr("Show Reference?"), this );
  m_showReference->setChecked( pin->showReference() );
  centralLayout->addWidget( m_showReference, 4, 0, 1, 1 );

  QPushButton *ok = new QPushButton( tr("OK"), this );
  QPushButton *cancel = new QPushButton( tr("Cancel"), this );

  centralLayout->addWidget( ok, 5, 2, 1, 1 );
  centralLayout->addWidget( cancel, 5, 3, 1, 1 );

  this->setWindowTitle( tr("Settings for text") );
  this->update();

  connect( ok, SIGNAL(pressed()), this, SLOT(accept()) );

  connect( cancel, SIGNAL(pressed()), this, SLOT(reject()) );
}

void PinSettingsDialog::apply( ElementPin *pin )
{
  pin->setSymbol( m_symbolEdit->text() );
  pin->setReference( m_referenceEdit->text() );
  pin->setLength( m_lengthSpin->value() );
  pin->setShowSymbol( m_showSymbol->checkState() == Qt::Checked );
  pin->setShowReference( m_showReference->checkState() == Qt::Checked );
}

}

#include "pinsettingsdialog.moc"
