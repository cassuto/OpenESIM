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

#ifndef PROPERTYWIDGETCUSTOM_H_
#define PROPERTYWIDGETCUSTOM_H_

#include <string>

#include "propertywidget.h"
#include "templatecustom.h"

namespace dsim
{

class PressColorBox;
class TemplatestylePreview;

class PropertyWidgetCustom : public PropertyWidget
{
  Q_OBJECT

public:
  PropertyWidgetCustom( const TemplateCustom *lineCustom, const TemplateCustom *fillCustom, const TemplateCustom *textCustom, QWidget *parent = 0l );
  ~PropertyWidgetCustom();

  const char *name() const { return "Style template"; }

  void loadTemplate( const char *name );

  std::string styleName();
  TemplateCustom lineCustom();
  TemplateCustom fillCustom();
  TemplateCustom textCustom();

private slots:
  void onStyleChanged( int index );
  void onLineStyleChanged( int index );
  void onFillStyleChanged( int index );
  void onUseBkcolorChanged( int state );
  void onCustomLineStyleChanged( int state );
  void onCustomLineWidthChanged( int state );
  void onCustomLineColorChanged( int state );
  void onCustomFillStyleChanged( int state );
  void onCustomFillUseBkcolorChanged( int state );
  void onCustomFillBkColorChanged( int state );
  void onCustomTextColorChanged( int state );
  void onCustomTextHeightChanged( int state );
  void onCustomTextBoldChanged( int state );
  void onCustomTextItalicChanged( int state );

private:
  void updateEnableState();

private:
  TemplateCustom m_lineCustom, m_fillCustom, m_textCustom;
  bool m_AllowLineCustom, m_AllowFillCustom, m_AllowTextCustom;
  bool m_loadingStyle;
  QComboBox *m_style, *m_lineStyle, *m_fillStyle;
  PressColorBox *m_lineColor, *m_fillBkColor;
  QDoubleSpinBox *m_lineWidth;
  QCheckBox *m_customLineStyle, *m_customLineWidth, *m_customLineColor;
  QCheckBox *m_customFillStyle, *m_customFillUseBkcolor, *m_customFillBkColor;
  QCheckBox *m_fillUseBkcolor;
  TemplatestylePreview *m_rectPreview;
  PressColorBox *m_textColor;
  QSpinBox *m_textHeight;
  QCheckBox *m_textBold, *m_textItalic;
  QCheckBox *m_customTextColor, *m_customTextHeight, *m_customTextBold, *m_customTextItalic;
};

}

#endif
