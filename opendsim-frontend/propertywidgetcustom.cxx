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

#include "propertywidgetcustom.h"

#include <string>
#include <cstdio>
#include "templatestyle.h"
#include "templatecustom.h"
#include "presscolorbox.h"
#include "templatestylepreview.h"


namespace dsim
{

PropertyWidgetCustom::PropertyWidgetCustom( const TemplateCustom *lineCustom, const TemplateCustom *fillCustom, const TemplateCustom *textCustom, QWidget *parent )
                       : PropertyWidget( 0l, parent )
                       , m_AllowLineCustom( 0l!=lineCustom )
                       , m_AllowFillCustom( 0l!=fillCustom )
                       , m_AllowTextCustom( 0l!=textCustom )
{
  if( lineCustom) m_lineCustom = *lineCustom; // copy backup version
  if( fillCustom ) m_fillCustom = *fillCustom;
  if( textCustom ) m_textCustom = *textCustom;

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

  connect( m_style, SIGNAL(currentIndexChanged(int)), this, SLOT(onStyleChanged(int)) );

  m_loadingStyle = false;
  centralLayout->addWidget( m_style, 0, 1, 1, 4 );

  QWidget *attributes = new QWidget( this );
  QHBoxLayout *attrLayout = new QHBoxLayout( attributes );
  QGroupBox *groups = 0l;
  QGridLayout *groupLayout = 0l;

  /*
   * Line && Fill style attributes
   */
  if( m_AllowLineCustom )
    {
      groups = new QGroupBox( tr("Line attributes"), attributes );
      groups->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
      groupLayout = new QGridLayout( groups );
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
      m_lineColor = new PressColorBox( groups );
      groupLayout->addWidget( m_lineColor, 2, 1, 1, 1 );
      attrLayout->addWidget( groups );

      for( int i=0; i<(int)LINE_RESERVED_MAX; i++ )
        {
          m_lineStyle->addItem( Templatestyle::toLineStyleName( (LineStyle)i ), i );
        }

      connect( m_customLineStyle, SIGNAL(stateChanged(int)), this, SLOT(onCustomLineStyleChanged(int)) );

      connect( m_customLineWidth, SIGNAL(stateChanged(int)), this, SLOT(onCustomLineWidthChanged(int)) );

      connect( m_customLineColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomLineColorChanged(int)) );
    }

  if( m_AllowFillCustom )
    {
      groups = new QGroupBox( tr("Fill attributes"), attributes );
      groups->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
      groupLayout = new QGridLayout( groups );
      m_customFillStyle = new QCheckBox( tr("Fill style:"), groups );
      groupLayout->addWidget( m_customFillStyle, 0, 0, 1, 1 );
      m_fillStyle = new QComboBox( groups );
      groupLayout->addWidget( m_fillStyle, 0, 1, 1, 1 );
      m_customFillUseBkcolor = new QCheckBox( groups );
      groupLayout->addWidget( m_customFillUseBkcolor, 1, 0, 1, 2 );
      m_fillUseBkcolor = new QCheckBox( tr("Use background color"), groups );
      groupLayout->addWidget( m_fillUseBkcolor, 1, 1, 1, 2 );
      m_customFillBkColor = new QCheckBox( tr("Background:"), groups );
      groupLayout->addWidget( m_customFillBkColor, 2, 0, 1, 1 );
      m_fillBkColor = new PressColorBox( groups );
      groupLayout->addWidget( m_fillBkColor, 2, 1, 1, 1 );
      attrLayout->addWidget( groups );

      for( int i=0; i<(int)BRUSH_RESERVED_MAX; i++ )
        {
          m_fillStyle->addItem( Templatestyle::toBrushStyleName( (BrushStyle)i ), i );
        }

      connect( m_fillStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineStyleChanged(int)) );

      connect( m_lineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(onLineStyleChanged(int)) );

      connect( m_fillUseBkcolor, SIGNAL(stateChanged(int)), this, SLOT(onUseBkcolorChanged(int)) );

      connect( m_fillUseBkcolor, SIGNAL(stateChanged(int)), this, SLOT(onUseBkcolorChanged(int)) );

      connect( m_customFillStyle, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillStyleChanged(int)) );

      connect( m_customFillUseBkcolor, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillUseBkcolorChanged(int)) );

      connect( m_customFillBkColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomFillBkColorChanged(int)) );
    }

  if( m_AllowLineCustom || m_AllowFillCustom )
    {
      groups = new QGroupBox( tr("Sample"), attributes );
      attrLayout->addWidget( groups );

      centralLayout->addWidget( attributes, 1, 0, 1, 5 );
    }

  /*
   * Text attributes
   */
  if( m_AllowTextCustom )
    {
      attributes = new QWidget( this );
      attrLayout = new QHBoxLayout( attributes );

      groups = new QGroupBox( tr("Text attributes"), attributes );
      groups->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Minimum );
      groupLayout = new QGridLayout( groups );
      m_customTextColor = new QCheckBox( tr("Text Color:"), groups );
      groupLayout->addWidget( m_customTextColor, 0, 0, 1, 1 );
      m_textColor = new PressColorBox( groups );
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
      attrLayout->addWidget( groups );

      groups = new QGroupBox( tr("Sample"), attributes );
      attrLayout->addWidget( groups );

      centralLayout->addWidget( attributes, 2, 0, 1, 5 );

      connect( m_customTextColor, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextColorChanged(int)) );

      connect( m_customTextHeight, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextHeightChanged(int)) );

      connect( m_customTextBold, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextBoldChanged(int)) );

      connect( m_customTextItalic, SIGNAL(stateChanged(int)), this, SLOT(onCustomTextItalicChanged(int)) );
    }

  std::string style = m_style->itemText(0).toStdString();
  loadTemplate( style.c_str() );
}

PropertyWidgetCustom::~PropertyWidgetCustom() {}

void PropertyWidgetCustom::loadTemplate( const char *name )
{
  if( !Templatestyle::instance()->isStyle( name ) )
    return;

  m_loadingStyle = true;
  m_style->setCurrentText( name );
  m_loadingStyle = false;

  StyleItem s;

  if( m_AllowLineCustom )
    {
      s = Templatestyle::instance()->lineStyle( name, /*selected*/ false );
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

      m_customLineStyle->setChecked( m_lineCustom.lineCustom );
      m_customLineWidth->setChecked( m_lineCustom.widthCustom );
      m_customLineColor->setChecked( m_lineCustom.colorCustom );
    }

  if( m_AllowFillCustom )
    {
      s = Templatestyle::instance()->fillStyle( name, /*selected*/ false );
      if( m_AllowFillCustom ) m_fillCustom.apply( &s );

      if( s.brush >= BRUSH_NONE && s.brush < BRUSH_RESERVED_MAX )
        {
          m_fillStyle->setCurrentIndex( (int)s.brush );
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
      m_customFillStyle->setChecked( m_fillCustom.brushCustom );
      m_customFillUseBkcolor->setChecked( m_fillCustom.usebkcolorCustom );
      m_customFillBkColor->setChecked( m_fillCustom.bkcolorCustom );
    }

  if( m_AllowTextCustom )
    {
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

      m_customTextColor->setChecked( m_textCustom.colorCustom );
      m_customTextHeight->setChecked( m_textCustom.sizeCustom );
      m_customTextBold->setChecked( m_textCustom.boldCustom );
      m_customTextItalic->setChecked( m_textCustom.italicCustom );
    }

  updateEnableState();
}

void PropertyWidgetCustom::onStyleChanged( int index )
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

void PropertyWidgetCustom::onLineStyleChanged( int )
{ updateEnableState(); }

void PropertyWidgetCustom::onFillStyleChanged( int )
{ updateEnableState(); }

void PropertyWidgetCustom::onUseBkcolorChanged( int state )
{ m_fillBkColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomLineStyleChanged( int state )
{ m_lineStyle->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomLineWidthChanged( int state )
{ m_lineWidth->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomLineColorChanged( int state )
{ m_lineColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomFillStyleChanged( int state )
{ m_fillStyle->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomFillUseBkcolorChanged( int state )
{ m_fillUseBkcolor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomFillBkColorChanged( int state )
{ m_fillBkColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomTextColorChanged( int state )
{ m_textColor->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomTextHeightChanged( int state )
{ m_textHeight->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomTextBoldChanged( int state )
{ m_textBold->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::onCustomTextItalicChanged( int state )
{ m_textItalic->setEnabled( state == Qt::Checked ); updateEnableState(); }

void PropertyWidgetCustom::updateEnableState()
{
  if( m_AllowLineCustom )
    {
      m_lineStyle->setEnabled( m_customLineStyle->checkState() == Qt::Checked );
      m_lineWidth->setEnabled( m_lineStyle->currentIndex()!=0 && m_customLineWidth->checkState() == Qt::Checked );
      m_lineColor->setEnabled( m_lineStyle->currentIndex()!=0 && m_customLineColor->checkState() == Qt::Checked );
    }
  if( m_AllowFillCustom )
    {
      m_fillStyle->setEnabled( m_customFillStyle->checkState() == Qt::Checked );
      m_fillUseBkcolor->setEnabled( m_customFillUseBkcolor->checkState() == Qt::Checked );

      if( m_fillStyle->currentIndex()==0 || m_fillUseBkcolor->checkState() == Qt::Checked )
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
  if( m_AllowTextCustom )
    {
      m_textColor->setEnabled( m_customTextColor->checkState() == Qt::Checked );
      m_textHeight->setEnabled( m_customTextHeight->checkState() == Qt::Checked );
      m_textBold->setEnabled( m_customTextBold->checkState() == Qt::Checked );
      m_textItalic->setEnabled( m_customTextItalic->checkState() == Qt::Checked );
    }
}

std::string PropertyWidgetCustom::styleName()
{ return m_style->currentText().toStdString(); }

TemplateCustom PropertyWidgetCustom::lineCustom()
{
  TemplateCustom cust;

  if( m_AllowLineCustom )
    {
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
    }
  return cust;
}

TemplateCustom PropertyWidgetCustom::fillCustom()
{
  TemplateCustom cust;

  if( m_AllowFillCustom )
    {
      if( m_customFillStyle->checkState() == Qt::Checked )
        {
          cust.brushCustom = true;
          cust.brush = (BrushStyle)m_fillStyle->currentIndex();
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
          cust.bkcolor.g = m_fillBkColor->color().green();
          cust.bkcolor.b = m_fillBkColor->color().blue();
        }
    }
  return cust;
}

TemplateCustom PropertyWidgetCustom::textCustom()
{
  TemplateCustom cust;

  if( m_AllowTextCustom )
    {
      if( m_customTextColor->checkState() == Qt::Checked )
        {
          cust.colorCustom = true;
          cust.color.r = m_textColor->color().red();
          cust.color.g = m_textColor->color().green();
          cust.color.b = m_textColor->color().blue();
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
    }
  return cust;
}

}

#include "propertywidgetcustom.moc"
