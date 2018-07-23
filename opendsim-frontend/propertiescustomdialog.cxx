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

#include <string>
#include <cstdio>
#include "templatestyle.h"
#include "templatecustom.h"
#include "presscombobox.h"
#include "templatestylepreview.h"

#include "propertiescustomdialog.h"

namespace dsim
{

PropertiesCustomDialog::PropertiesCustomDialog( const TemplateCustom *lineCustom, const TemplateCustom *fillCustom, const TemplateCustom *textCustom, QWidget *parent )
                       : QDialog( parent )
                       , m_lineCustom( *lineCustom ) // copy backup version
                       , m_fillCustom( *fillCustom )
                       , m_textCustom( *textCustom )
                       , m_AllowLineCustom( 0l!=lineCustom )
                       , m_AllowFillCustom( 0l!=fillCustom )
                       , m_AllowTextCustom( 0l!=textCustom )
{
  QGridLayout *centralLayout = new QGridLayout( this );

  QLabel *label;
  label = new QLabel( tr("Set style:"), this );
  centralLayout->addWidget( label, 0, 0, 1, 1 );

  m_style = new QComboBox( this );
  m_loadingStyle = true;

  using namespace std;
  list<string> templates = Templatestyle::instance()->templates();

  for( list<string>::iterator it = templates.begin(); it!=templates.end(); it++ )
    {
      m_style->addItem( (*it).c_str() );
    }

  m_loadingStyle = false;
  centralLayout->addWidget( m_style, 0, 1, 1, 4 );

  /*
   * Line && Fill style attributes
   */
  QWidget *attributes = new QWidget( this );
  QHBoxLayout *attrLayout = new QHBoxLayout( attributes );

  QGroupBox *groups;
  groups = new QGroupBox( tr("Line attributes"), attributes );
  QGridLayout *groupLayout = new QGridLayout;
  m_customLineStyle = new QCheckBox( tr("Line style:"), groups );
  groupLayout->addWidget( m_customLineStyle, 0, 0, 1, 1 );
  m_lineStyle = new QComboBox( groups );
  groupLayout->addWidget( m_lineStyle, 0, 1, 1, 1 );
  m_customLineWidth = new QCheckBox( tr("Width:"), groups );
  groupLayout->addWidget( m_customLineWidth, 1, 0, 1, 1 );
  m_lineWidth = new QDoubleSpinBox( groups );
  m_lineWidth->setRange( 0.1, 10.0 );
  m_lineWidth->setSingleStep( 0.2 );
  groupLayout->addWidget( m_lineWidth, 1, 1, 1, 1 );
  m_customLineColor = new QCheckBox( tr("Color:"), groups );
  groupLayout->addWidget( m_customLineColor, 2, 0, 1, 1 );
  m_lineColor = new PressComboBox( groups );
  groupLayout->addWidget( m_lineColor, 2, 1, 1, 1 );
  groups->setLayout( groupLayout );
  attrLayout->addWidget( groups );

  groups = new QGroupBox( tr("Fill attributes"), attributes );
  groupLayout = new QGridLayout;
  m_customFillStyle = new QCheckBox( tr("Fill style:"), groups );
  groupLayout->addWidget( m_customFillStyle, 0, 0, 1, 1 );
  m_fillStyle = new QComboBox( groups );
  groupLayout->addWidget( m_fillStyle, 0, 1, 1, 1 );
  m_customFillFgColor = new QCheckBox( tr("Foreground:"), groups );
  groupLayout->addWidget( m_customFillFgColor, 1, 0, 1, 1 );
  m_fillFgColor = new PressComboBox( groups );
  groupLayout->addWidget( m_fillFgColor, 1, 1, 1, 1 );
  m_customFillUseBkcolor = new QCheckBox( groups );
  groupLayout->addWidget( m_customFillUseBkcolor, 2, 0, 1, 2 );
  m_fillUseBkcolor = new QCheckBox( tr("Use background color"), groups );
  groupLayout->addWidget( m_fillUseBkcolor, 2, 1, 1, 2 );
  m_customFillBkColor = new QCheckBox( tr("Background:"), groups );
  groupLayout->addWidget( m_customFillBkColor, 3, 0, 1, 1 );
  m_fillBkColor = new PressComboBox( groups );
  groupLayout->addWidget( m_fillBkColor, 3, 1, 1, 1 );
  groups->setLayout( groupLayout );
  attrLayout->addWidget( groups );

  for( int i=0; i<(int)LINE_RESERVED_MAX; i++ )
    {
      m_lineStyle->addItem( Templatestyle::toLineStyleName( (LineStyle)i ), i );
    }

  for( int i=0; i<(int)BRUSH_RESERVED_MAX; i++ )
    {
      m_fillStyle->addItem( Templatestyle::toBrushStyleName( (BrushStyle)i ), i );
    }

  groups = new QGroupBox( tr("Sample"), attributes );
  attrLayout->addWidget( groups );

  centralLayout->addWidget( attributes, 1, 0, 1, 5 );

  /*
   * Text attributes
   */
  attributes = new QWidget( this );
  attrLayout = new QHBoxLayout( attributes );

  groups = new QGroupBox( tr("Text attributes"), attributes );
  groupLayout = new QGridLayout;
  m_customTextColor = new QCheckBox( tr("Text Color:"), groups );
  groupLayout->addWidget( m_customTextColor, 0, 0, 1, 1 );
  m_textColor = new PressComboBox( groups );
  groupLayout->addWidget( m_textColor, 0, 1, 1, 1 );
  m_customTextHeight = new QCheckBox( tr("Text Height:"), groups );
  groupLayout->addWidget( m_customTextHeight, 1, 0, 1, 1 );
  m_textHeight = new QSpinBox( groups );
  m_textHeight->setRange( 1, 60 );
  groupLayout->addWidget( m_textHeight, 1, 1, 1, 1 );
  m_customTextBold = new QCheckBox( tr("Bold:"), groups );
  groupLayout->addWidget( m_customTextBold, 2, 0, 1, 1 );
  m_textBold = new QCheckBox( groups );
  groupLayout->addWidget( m_textBold, 2, 1, 1, 1 );
  m_customTextItalic = new QCheckBox( tr("Italic:"), groups );
  groupLayout->addWidget( m_customTextItalic, 3, 0, 1, 1 );
  m_textItalic = new QCheckBox( groups );
  groupLayout->addWidget( m_textItalic, 3, 1, 1, 1 );
  groups->setLayout( groupLayout );
  attrLayout->addWidget( groups );

  groups = new QGroupBox( tr("Sample"), attributes );
  attrLayout->addWidget( groups );

  centralLayout->addWidget( attributes, 2, 0, 1, 5 );

  QPushButton *button = new QPushButton( tr("OK"), this );
  centralLayout->addWidget( button, 3, 3, 1, 1 );

  connect( button, SIGNAL(clicked()), this, SLOT(accept()) );

  button = new QPushButton( tr("Cancel"), this );
  centralLayout->addWidget( button, 3, 4, 1, 1 );

  connect( button, SIGNAL(clicked()), this, SLOT(reject()) );

  connect( m_style, SIGNAL(currentIndexChanged(int)), this, SLOT(onStyleChanged(int)) );

  connect( m_lineColor, SIGNAL(clicked()), this, SLOT(onLineColorClicked()) );

  connect( m_fillFgColor, SIGNAL(clicked()), this, SLOT(onFillFgColorClicked()) );

  connect( m_fillBkColor, SIGNAL(clicked()), this, SLOT(onFillBkColorClicked()) );

  connect( m_fillUseBkcolor, SIGNAL(stateChanged(int)), this, SLOT(onUseBkcolorChanged(int)) );

  connect( m_customLineStyle, SIGNAL(stateChanged(int)), this, SLOT(onCustomLineStyleChanged(int)) );

  connect( m_customLineWidth, SIGNAL(stateChanged(int)), this, SLOT(onCustomLineWidthChanged(int)) );

  connect( m_customLineColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomLineColorChanged(int)) );

  connect( m_customFillStyle, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillStyleChanged(int)) );

  connect( m_customFillFgColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillFgColorChanged(int)) );

  connect( m_customFillUseBkcolor, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillUseBkcolorChanged(int)) );

  connect( m_customFillBkColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillBkColorChanged(int)) );

  connect( m_textColor, SIGNAL(clicked()), this, SLOT(onTextColorClicked()) );

  connect( m_customTextColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextColorChanged(int)) );

  connect( m_customTextHeight, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextHeightChanged(int)) );

  connect( m_customTextBold, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextBoldChanged(int)) );

  connect( m_customTextItalic, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextItalicChanged(int)) );

  setWindowTitle( tr("Properties of custom template") );
  update();

  std::string style = m_style->itemText(0).toStdString();
  loadTemplate( style.c_str() );
}

void PropertiesCustomDialog::loadTemplate( const char *name )
{
  if( !Templatestyle::instance()->isStyle( name ) )
    return;

  m_loadingStyle = true;
  m_style->setCurrentText( name );
  m_loadingStyle = false;

  StyleItem s = Templatestyle::instance()->lineStyle( name, /*selected*/ false );
  if( m_AllowLineCustom ) m_lineCustom.apply( &s );

  if( s.line >= LINE_NONE && s.line < LINE_RESERVED_MAX )
    {
      m_lineStyle->setCurrentIndex( (int)s.line );
    }
  m_lineWidth->setValue( s.width );
  if( s.color.r >= 0 && s.color.g >= 0 && s.color.b >= 0 )
    {
      m_lineColor->setColor( QColor( s.color.r, s.color.g, s.color.b ) );
      m_lineColor->setEnabled( true );
    }
  else
    {
      m_lineColor->setColor( QColor( 255, 255, 255 ) );
      m_lineColor->setEnabled( false );
    }

  s = Templatestyle::instance()->fillStyle( name, /*selected*/ false );
  if( m_AllowFillCustom ) m_fillCustom.apply( &s );

  if( s.brush >= BRUSH_NONE && s.brush < BRUSH_RESERVED_MAX )
    {
      m_fillStyle->setCurrentIndex( (int)s.brush );
    }

  if( s.color.r >= 0 && s.color.g >= 0 && s.color.b >= 0 )
    {
      m_fillFgColor->setColor( QColor( s.color.r, s.color.g, s.color.b ) );
      m_fillFgColor->setEnabled( true );
    }
  else
    {
      m_fillFgColor->setColor( QColor( 255, 255, 255 ) );
      m_fillFgColor->setEnabled( false );
    }

  m_fillUseBkcolor->setChecked( s.usebkcolor );

  if( s.usebkcolor )
    {
      m_fillBkColor->setEnabled( false );
    }
  else
    {
      if( s.bkcolor.r >= 0 && s.bkcolor.g >= 0 && s.bkcolor.b >= 0 )
        {
          m_fillBkColor->setColor( QColor( s.bkcolor.r, s.bkcolor.g, s.bkcolor.b ) );
          m_fillBkColor->setEnabled( true );
        }
      else
        {
          m_fillBkColor->setColor( QColor( 255, 255, 255 ) );
          m_fillBkColor->setEnabled( false );
        }
    }

  s = Templatestyle::instance()->textStyle( name, /*selected*/ false );
  if( m_AllowTextCustom ) m_textCustom.apply( &s );

  if( s.color.r >= 0 && s.color.g >= 0 && s.color.b >= 0 )
    {
      m_textColor->setColor( QColor( s.color.r, s.color.g, s.color.b ) );
    }
  else
    {
      m_textColor->setColor( QColor( 255, 255, 255 ) );
      m_textColor->setEnabled( false );
    }
  m_textHeight->setValue( s.size );
  m_textBold->setChecked( s.bold );
  m_textItalic->setChecked( s.italic );

  if( m_AllowLineCustom )
    {
      m_customLineStyle->setChecked( m_lineCustom.lineCustom );
      m_customLineWidth->setChecked( m_lineCustom.widthCustom );
      m_customLineColor->setChecked( m_lineCustom.colorCustom );
    }

  m_customLineStyle->setEnabled( m_AllowLineCustom );
  m_customLineWidth->setEnabled( m_AllowLineCustom );
  m_customLineColor->setEnabled( m_AllowLineCustom );

  if( m_AllowFillCustom )
    {
      m_customFillStyle->setChecked( m_fillCustom.brushCustom );
      m_customFillFgColor->setChecked( m_fillCustom.colorCustom );
      m_customFillUseBkcolor->setChecked( m_fillCustom.usebkcolorCustom );
      m_customFillBkColor->setChecked( m_fillCustom.bkcolorCustom );
    }

  m_customFillStyle->setEnabled( m_AllowFillCustom );
  m_customFillFgColor->setEnabled( m_AllowFillCustom );
  m_customFillUseBkcolor->setEnabled( m_AllowFillCustom );
  m_customFillBkColor->setEnabled( m_AllowFillCustom );

  if( m_AllowTextCustom )
    {
      m_customTextColor->setChecked( m_textCustom.colorCustom );
      m_customTextHeight->setChecked( m_textCustom.sizeCustom );
      m_customTextBold->setChecked( m_textCustom.boldCustom );
      m_customTextItalic->setChecked( m_textCustom.italicCustom );
    }

  m_customTextColor->setEnabled( m_AllowTextCustom );
  m_customTextHeight->setEnabled( m_AllowTextCustom );
  m_customTextBold->setEnabled( m_AllowTextCustom );
  m_customTextItalic->setEnabled( m_AllowTextCustom );

  updateEnableState();
}

void PropertiesCustomDialog::onStyleChanged( int index )
{
  std::string style = m_style->itemText( index ).toStdString();

  if( !m_loadingStyle )
    {
      m_lineCustom.setCustomAll( false );
      m_fillCustom.setCustomAll( false );
      m_textCustom.setCustomAll( false );

      loadTemplate( style.c_str() );
    }
}


template <typename comboT>
  void getColorComboBox( comboT *combo )
  {
    QColorDialog cd( combo->palette().color( QPalette::Base ) );

    if( cd.exec() == QDialog::Accepted )
      {
        combo->setColor( cd.selectedColor() );
      }
  }

void PropertiesCustomDialog::onLineColorClicked()
{ getColorComboBox( m_lineColor ); }

void PropertiesCustomDialog::onFillFgColorClicked()
{ getColorComboBox( m_fillFgColor ); }

void PropertiesCustomDialog::onFillBkColorClicked()
{ getColorComboBox( m_fillBkColor ); }

void PropertiesCustomDialog::onUseBkcolorChanged( int state )
{ m_fillBkColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomLineStyleChanged( int state )
{ m_lineStyle->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomLineWidthChanged( int state )
{ m_lineWidth->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomLineColorChanged( int state )
{ m_lineColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomFillStyleChanged( int state )
{ m_fillStyle->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomFillFgColorChanged( int state )
{ m_fillFgColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomFillUseBkcolorChanged( int state )
{ m_fillUseBkcolor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomFillBkColorChanged( int state )
{ m_fillBkColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onTextColorClicked()
{ getColorComboBox( m_textColor ); }

void PropertiesCustomDialog::onCustomTextColorChanged( int state )
{ m_textColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomTextHeightChanged( int state )
{ m_textHeight->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomTextBoldChanged( int state )
{ m_textBold->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::onCustomTextItalicChanged( int state )
{ m_textItalic->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertiesCustomDialog::updateEnableState()
{
  m_lineStyle->setEnabled( m_customLineStyle->checkState() == Qt::Checked );
  m_lineWidth->setEnabled( m_customLineWidth->checkState() == Qt::Checked );
  m_lineColor->setEnabled( m_customLineColor->checkState() == Qt::Checked );
  m_fillStyle->setEnabled( m_customFillStyle->checkState() == Qt::Checked );
  m_fillFgColor->setEnabled( m_customFillFgColor->checkState() == Qt::Checked );
  m_fillUseBkcolor->setEnabled( m_customFillUseBkcolor->checkState() == Qt::Checked );
  m_textColor->setEnabled( m_customTextColor->checkState() == Qt::Checked );
  m_textHeight->setEnabled( m_customTextHeight->checkState() == Qt::Checked );
  m_textBold->setEnabled( m_customTextBold->checkState() == Qt::Checked );
  m_textItalic->setEnabled( m_customTextItalic->checkState() == Qt::Checked );

  if( m_fillUseBkcolor->checkState() == Qt::Checked )
    {
      m_fillBkColor->setEnabled( false );
      m_customFillBkColor->setEnabled( false );
    }
  else
    {
      m_customFillBkColor->setEnabled( true );
      m_fillBkColor->setEnabled( m_customFillBkColor->checkState() );
    }
}

std::string PropertiesCustomDialog::styleName()
{ return m_style->currentText().toStdString(); }

TemplateCustom PropertiesCustomDialog::lineCustom()
{
  TemplateCustom cust;

  if( m_customLineStyle->checkState() == Qt::Checked )
    {
      cust.lineCustom = true;
      cust.line = (LineStyle)m_lineStyle->currentIndex();
    }
  if( m_customLineWidth->checkState() == Qt::Checked )
    {
      cust.widthCustom = true;
      cust.width = float( m_lineWidth->value() );
    }
  if( m_customLineColor->checkState() == Qt::Checked )
    {
      cust.colorCustom = true;
      cust.color.r = m_lineColor->color().red();
      cust.color.g = m_lineColor->color().green();
      cust.color.b = m_lineColor->color().blue();
    }
  return cust;
}

TemplateCustom PropertiesCustomDialog::fillCustom()
{
  TemplateCustom cust;

  if( m_customFillStyle->checkState() == Qt::Checked )
    {
      cust.brushCustom = true;
      cust.brush = (BrushStyle)m_fillStyle->currentIndex();
    }
  if( m_customFillFgColor->checkState() == Qt::Checked )
    {
      cust.colorCustom = true;
      cust.color.r = m_fillFgColor->color().red();
      cust.color.g = m_fillFgColor->color().blue();
      cust.color.b = m_fillFgColor->color().green();
    }
  if( m_customFillUseBkcolor->checkState() == Qt::Checked )
    {
      cust.usebkcolorCustom = true;
      cust.usebkcolor = (m_fillUseBkcolor->checkState() == Qt::Checked);
    }
  if( m_customFillBkColor->checkState() == Qt::Checked )
    {
      cust.bkcolorCustom = true;
      cust.bkcolor.r = m_fillBkColor->color().red();
      cust.bkcolor.g = m_fillBkColor->color().blue();
      cust.bkcolor.b = m_fillBkColor->color().green();
    }
  return cust;
}

TemplateCustom PropertiesCustomDialog::textCustom()
{
  TemplateCustom cust;

  if( m_customTextColor->checkState() == Qt::Checked )
    {
      cust.colorCustom = true;
      cust.color.r = m_textColor->color().red();
      cust.color.g = m_textColor->color().blue();
      cust.color.b = m_textColor->color().green();
    }
  if( m_customTextHeight->checkState() == Qt::Checked )
    {
      cust.sizeCustom = true;
      cust.size = m_textHeight->value();
    }
  if( m_customTextBold->checkState() == Qt::Checked )
    {
      cust.boldCustom = true;
      cust.bold = (m_textBold->checkState() == Qt::Checked);
    }
  if( m_customTextItalic->checkState() == Qt::Checked )
    {
      cust.italicCustom = true;
      cust.italic = (m_textItalic->checkState() == Qt::Checked);
    }
  return cust;
}

}

#include "propertiescustomdialog.moc"
