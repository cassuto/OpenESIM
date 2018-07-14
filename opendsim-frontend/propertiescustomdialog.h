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

#ifndef PROPERTIESCUSTOMDIALOG_H_
#define PROPERTIESCUSTOMDIALOG_H_

#include <QtWidgets>

#include "templatecustom.h"

namespace dsim
{

class PressComboBox;
class TemplatestylePreview;

class PropertiesCustomDialog : public QDialog
{
  Q_OBJECT

public:
  PropertiesCustomDialog( QWidget *parent = 0l );

  void loadTemplate( const char *name, TemplateCustom *lineCustom, TemplateCustom *fillCustom, TemplateCustom *textCustom );

  TemplateCustom lineCustom();
  TemplateCustom fillCustom();
  TemplateCustom textCustom();

private slots:
  void onStyleChanged( int index );
  void onLineColorClicked();
  void onFillFgColorClicked();
  void onFillBkColorClicked();
  void onUseBkcolorChanged( int state );
  void onCustomLineStyleChanged( int state );
  void onCustomLineWidthChanged( int state );
  void onCustomLineColorChanged( int state );
  void onCustomFillStyleChanged( int state );
  void onCustomFillFgColorChanged( int state );
  void onCustomFillUseBkcolorChanged( int state );
  void onCustomFillBkColorChanged( int state );
  void onTextColorClicked();
  void onCustomTextColorChanged( int state );
  void onCustomTextHeightChanged( int state );
  void onCustomTextBoldChanged( int state );
  void onCustomTextItalicChanged( int state );

private:
  void updateEnableState();

private:
  QComboBox *m_style, *m_lineStyle, *m_fillStyle;
  PressComboBox *m_lineColor, *m_fillFgColor, *m_fillBkColor;
  QDoubleSpinBox *m_lineWidth;
  QCheckBox *m_customLineStyle, *m_customLineWidth, *m_customLineColor;
  QCheckBox *m_customFillStyle, *m_customFillFgColor, *m_customFillUseBkcolor, *m_customFillBkColor;
  QCheckBox *m_fillUseBkcolor;
  TemplatestylePreview *m_rectPreview;
  PressComboBox *m_textColor;
  QSpinBox *m_textHeight;
  QCheckBox *m_textBold, *m_textItalic;
  QCheckBox *m_customTextColor, *m_customTextHeight, *m_customTextBold, *m_customTextItalic;
};

}

#endif
