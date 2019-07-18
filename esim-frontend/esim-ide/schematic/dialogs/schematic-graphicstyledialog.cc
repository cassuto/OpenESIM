/******************************************************************************
 * OpenESIM (Electrical Engineering Simulator)                                *
 * Copyright (C) 2019, ESIM team. <diyer175@hotmail.com>                      *
 *                                                                            *
 * This project is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public License(GPL)    *
 * as published by the Free Software Foundation; either version 2.1           *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This project is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 ******************************************************************************/

#include "preview.h"
#include "schematic-graphicstyledialog.h"
#include "styletemplate.h"
#include "schematic.h"
#include "elementrect.h"
#include "elementtext.h"
#include <QColorDialog>
#include <QHBoxLayout>
#include <QMessageBox>

namespace schematic
{

static QString colorToString(const templatedColor &color)
{
  return QString("(%1,%2,%3)").arg(color.R).arg(color.G).arg(color.B);
}

template <typename T>
  static void setColorSelectionButton(T *button, const templatedColor &color)
  {
    QPixmap pix = QPixmap(16, 16);
    pix.fill(QColor(color.R, color.G, color.B));
    button->setIcon(QIcon(pix));
    button->setText(colorToString(color));
  }

template <typename T>
  static void setTemplateCombo(QComboBox *combox, const std::string &currentSymbol, const std::map<std::string, T *> &styles)
  {
    combox->clear();
    for(typename std::map<std::string, T *>::const_iterator it = styles.begin(); it!=styles.end(); it++)
      {
        combox->addItem(it->second->symbol().c_str());
       if (it->second->symbol().compare(currentSymbol) == 0)
         { combox->setCurrentIndex(combox->count()-1); }
      }
  }

void SchematicGraphicsStyleDialog::setupLine(const LineStyle &line)
{
  m_blockUpdating = true;
    {
      setTemplateCombo(ui.templateCombox, line.symbol(), StyleTemplates::getInstance()->lineStyles());

      for(int i=LINE_NONE; i<LINEMAX; i++)
        {
          ui.linePen->addItem(StyleTemplates::getTemplatedStyleName(templatedStyle(i)));
        }

      setColorSelectionButton(ui.lineColor, line.color());
      ui.lineWidth->setValue(line.width());
      ui.linePen->setCurrentIndex(int(line.line() - LINE_NONE));

      m_line = line;
      m_lineGlobal = *StyleTemplates::getInstance()->getLineStyleRef(line.symbol());
      setLine();
    }
  m_blockUpdating = false;
}

SchematicGraphicsStyleDialog::SchematicGraphicsStyleDialog(const LineStyle &line, QWidget *parent) :
    QDialog(parent),
    m_line(line),
    m_editLine(true),
    m_editFill(false),
    m_editText(false),
    m_blockUpdating(false),
    m_graphicsPreview(0l),
    m_textPreview(0l),
    m_previewRect(0l),
    m_previewText(0l)
{
  setupUi();
  ui.groupLine->setVisible(true);
  ui.groupFill->setVisible(false);
  ui.groupText->setVisible(false);
  ui.groupGraphicsPreview->setVisible(true);
  ui.groupTextPreview->setVisible(false);
  setMaximumSize(width(), 0);

  setupLine(line);
}

void SchematicGraphicsStyleDialog::setupFill(const FillStyle &fill)
{
  m_blockUpdating = true;
    {
      setTemplateCombo(ui.templateCombox, fill.symbol(), StyleTemplates::getInstance()->fillStyles());

      for(int i=LINE_NONE; i<LINEMAX; i++)
        {
          ui.fillLinePen->addItem(StyleTemplates::getTemplatedStyleName(templatedStyle(i)));
        }
      for(int i=BRUSH_NONE; i<BRUSHMAX; i++)
        {
          ui.fillBrush->addItem(StyleTemplates::getTemplatedStyleName(templatedStyle(i)));
        }

      setColorSelectionButton(ui.fillColor, fill.color());
      setColorSelectionButton(ui.fillBkcolor, fill.bkcolor());
      ui.fillLineWidth->setValue(fill.width());
      ui.fillLinePen->setCurrentIndex(int(fill.line() - LINE_NONE));
      ui.fillBrush->setCurrentIndex(int(fill.brushS() - BRUSH_NONE));

      m_fill = fill;
      m_fillGlobal = *StyleTemplates::getInstance()->getFillStyleRef(fill.symbol());
      setFill();
    }
  m_blockUpdating = false;
}

SchematicGraphicsStyleDialog::SchematicGraphicsStyleDialog(const FillStyle &fill, QWidget *parent) :
    QDialog(parent),
    m_editLine(false),
    m_fill(fill),
    m_editFill(true),
    m_editText(false),
    m_blockUpdating(false),
    m_graphicsPreview(0l),
    m_textPreview(0l),
    m_previewRect(0l),
    m_previewText(0l)
{
  setupUi();
  ui.groupLine->setVisible(false);
  ui.groupFill->setVisible(true);
  ui.groupText->setVisible(false);
  ui.groupGraphicsPreview->setVisible(true);
  ui.groupTextPreview->setVisible(false);
  setMaximumSize(width(), 0);

  setupFill(fill);
  /*
   * Create preview widget for rectangle element.
   */
  QHBoxLayout *layout = new QHBoxLayout(ui.groupGraphicsPreview);
  m_graphicsPreview = new PreviewElement(this);
  m_previewRect = static_cast<ElementRect *>(m_graphicsPreview->document()->appendElement("rect"));
  layout->addWidget(m_graphicsPreview);

  connect(m_graphicsPreview, SIGNAL(resize(int, int)), this, SLOT(onGraphicsPreviewResize(int, int)));
}

void SchematicGraphicsStyleDialog::setupText(const TextStyle &text)
{
  m_blockUpdating = true;
    {
      setTemplateCombo(ui.templateCombox, text.symbol(), StyleTemplates::getInstance()->textStyles());

      setColorSelectionButton(ui.textColor, text.color());
      ui.textFont->setFont(QFont(text.fontName().c_str()));
      ui.textSizePt->setValue(text.sizePt());
      ui.textBold->setChecked(text.bold());
      ui.textItalic->setChecked(text.italic());
      ui.textUnderline->setChecked(text.underline());

      m_text = text;
      m_textGlobal = *StyleTemplates::getInstance()->getTextStyleRef(text.symbol());
      setText();
    }
  m_blockUpdating = false;
}

SchematicGraphicsStyleDialog::SchematicGraphicsStyleDialog(const TextStyle &text, QWidget *parent) :
    QDialog(parent),
    m_editLine(false),
    m_editFill(false),
    m_text(text),
    m_editText(true),
    m_blockUpdating(false),
    m_graphicsPreview(0l),
    m_textPreview(0l),
    m_previewRect(0l),
    m_previewText(0l)
{
  setupUi();
  ui.groupLine->setVisible(false);
  ui.groupFill->setVisible(false);
  ui.groupText->setVisible(true);
  ui.groupGraphicsPreview->setVisible(false);
  ui.groupTextPreview->setVisible(true);
  setMaximumSize(width(), 0);

  setupText(text);
  /*
   * Create preview widget for rectangle element.
   */
  QHBoxLayout *layout = new QHBoxLayout(ui.groupTextPreview);
  m_textPreview = new PreviewElement(this);
  m_previewText = static_cast<ElementText *>(m_textPreview->document()->appendElement("text"));
  layout->addWidget(m_textPreview);

  connect(m_textPreview, SIGNAL(resize(int, int)), this, SLOT(onTextPreviewResize(int, int)));
}

void SchematicGraphicsStyleDialog::onGraphicsPreviewResize(int width, int height)
{
  if (m_previewRect)
    {
      m_previewRect->locate(locateSetPos,
                            sizeof(int)*4,
                            20, 20,
                            m_graphicsPreview->document()->mapToLogicalX(width-20),
                            m_graphicsPreview->document()->mapToLogicalY(height-20));
    }
}

void SchematicGraphicsStyleDialog::onTextPreviewResize(int width, int height)
{
  if (m_previewText)
    {
      int x1, y1, x2, y2;
      m_previewText->bounding(&x1, &y1, &x2, &y2);
      int tw = std::abs(x2-x1) + 1;
      int th = std::abs(y2-y1) + 1;

      int dX = m_textPreview->document()->mapToLogicalDx(width);
      int dY = m_textPreview->document()->mapToLogicalDy(height);

      m_previewText->locate(locateSetPos, sizeof(int)*2, dX/2-tw/2, dY/2-th/2);
    }
}

void SchematicGraphicsStyleDialog::onTemplateChanged(int index)
{
  if (!m_blockUpdating)
    {
      QString templateName = ui.templateCombox->itemText(index);
      if (m_editLine)
        { m_line = *StyleTemplates::getInstance()->getLineStyleRef(templateName.toStdString()); setupLine(m_line); }
      if (m_editFill)
        { m_fill = *StyleTemplates::getInstance()->getFillStyleRef(templateName.toStdString()); setupFill(m_fill); }
      if (m_editText)
        { m_text = *StyleTemplates::getInstance()->getTextStyleRef(templateName.toStdString()); setupText(m_text); }
    }
}

void SchematicGraphicsStyleDialog::setLine()
{
  m_line.setWidth(float(ui.lineWidth->value()));
  m_line.setLine(templatedStyle(LINE_NONE + ui.linePen->currentIndex()));

  ui.lineColorGlobal->setChecked(m_line.color() == m_lineGlobal.color());
  ui.lineWidthGlobal->setChecked(m_line.width() == m_lineGlobal.width());
  ui.linePenGlobal->setChecked(m_line.line() == m_lineGlobal.line());
}
void SchematicGraphicsStyleDialog::setFill()
{
  m_fill.setWidth(float(ui.fillLineWidth->value()));
  m_fill.setLine(templatedStyle(LINE_NONE +  ui.fillLinePen->currentIndex()));
  m_fill.setBrushS(templatedStyle(BRUSH_NONE + ui.fillBrush->currentIndex()));

  ui.fillColorGlobal->setChecked(m_fill.color() == m_fillGlobal.color());
  ui.fillBkcolorGlobal->setChecked(m_fill.bkcolor() == m_fillGlobal.bkcolor());
  ui.fillLineWidthGlobal->setChecked(m_fill.width() == m_fillGlobal.width());
  ui.fillLinePenGlobal->setChecked(m_fill.line() == m_fillGlobal.line());
  ui.fillBrushGlobal->setChecked(m_fill.brushS() == m_fillGlobal.brushS());

  if (m_previewRect)   // Update rectangle preview.
    {
      m_previewRect->setFillStyle(m_fill);
      m_graphicsPreview->update();
    }
}
void SchematicGraphicsStyleDialog::setText()
{
  m_text.setSizePt(ui.textSizePt->value());
  m_text.setFontName(ui.textFont->currentText().toStdString());
  m_text.setBold(ui.textBold->checkState() == Qt::Checked);
  m_text.setItalic(ui.textItalic->checkState() == Qt::Checked);
  m_text.setUnderline(ui.textUnderline->checkState() == Qt::Checked);

  ui.textColorGlobal->setChecked(m_text.color() == m_textGlobal.color());
  ui.textFontGlobal->setChecked(m_text.fontName().compare(m_textGlobal.fontName()) == 0);
  ui.textSizePtGlobal->setChecked(m_text.sizePt() == m_textGlobal.sizePt());
  ui.textBoldGlobal->setChecked(m_text.bold() == m_textGlobal.bold());
  ui.textItalicGlobal->setChecked(m_text.italic() == m_textGlobal.italic());
  ui.textUnderlineGlobal->setChecked(m_text.underline() == m_textGlobal.underline());

  if (m_previewText)   // Update text preview.
    {
      m_previewText->setTextStyle(m_text);
      onTextPreviewResize(m_textPreview->width(), m_textPreview->height());
      m_textPreview->update();
    }
}

void SchematicGraphicsStyleDialog::updateGlobals()
{
  if (!m_blockUpdating)
    {
      if ((ui.lineColorGlobal->checkState() == Qt::Checked))
        {
          m_line.setColor(m_lineGlobal.color());
          setColorSelectionButton(ui.lineColor, m_lineGlobal.color());
          setLine();
        }
      if (ui.lineWidthGlobal->checkState() == Qt::Checked)
        { ui.lineWidth->setValue(m_lineGlobal.width()); }
      if (ui.linePenGlobal->checkState() == Qt::Checked)
        { ui.linePen->setCurrentIndex(int(m_lineGlobal.line() - LINE_NONE)); }

      if (ui.fillColorGlobal->checkState() == Qt::Checked)
        {
          m_fill.setColor(m_fillGlobal.color());
          setColorSelectionButton(ui.fillColor, m_fillGlobal.color());
          setFill();
        }
      if (ui.fillBkcolorGlobal->checkState() == Qt::Checked)
        {
          m_fill.setBkcolor(m_fillGlobal.bkcolor());
          setColorSelectionButton(ui.fillBkcolor, m_fillGlobal.bkcolor());
          setFill();
        }
      if (ui.fillLineWidthGlobal->checkState() == Qt::Checked)
        { ui.fillLineWidth->setValue(m_fillGlobal.width()); }
      if (ui.fillLinePenGlobal->checkState() == Qt::Checked)
        { ui.fillLinePen->setCurrentIndex(int(m_fillGlobal.line() - LINE_NONE)); }
      if (ui.fillBrushGlobal->checkState() == Qt::Checked)
        { ui.fillBrush->setCurrentIndex(int(m_fillGlobal.brushS() - BRUSH_NONE)); }

      if (ui.textColorGlobal->checkState() == Qt::Checked)
        {
          m_text.setColor(m_textGlobal.color());
          setColorSelectionButton(ui.textColor, m_textGlobal.color());
          setText();
        }
      if (ui.textFontGlobal->checkState() == Qt::Checked)
        { ui.textFont->setFont(QFont(m_textGlobal.fontName().c_str())); }
      if (ui.textSizePtGlobal->checkState() == Qt::Checked)
        { ui.textSizePt->setValue(m_textGlobal.sizePt()); }
      if (ui.textBoldGlobal->checkState() == Qt::Checked)
        { ui.textBold->setChecked(m_textGlobal.bold()); }
      if (ui.textItalicGlobal->checkState() == Qt::Checked)
        { ui.textItalic->setChecked(m_textGlobal.italic()); }
      if (ui.textUnderlineGlobal->checkState() == Qt::Checked)
        { ui.textUnderline->setChecked(m_textGlobal.underline()); }
    }
}

void SchematicGraphicsStyleDialog::setupUi()
{
  ui.setupUi(this);

  /*
   * Mouse pressed Signals
   */
  connect(ui.lineColor, SIGNAL(pressed()), this, SLOT(onLineColorPress()));

  connect(ui.fillColor, SIGNAL(pressed()), this, SLOT(onFillColorPress()));

  connect(ui.fillBkcolor, SIGNAL(pressed()), this, SLOT(onFillBkcolorPress()));

  connect(ui.textColor, SIGNAL(pressed()), this, SLOT(onTextColorPress()));

  connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(onAccept()));

  connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

  connect(ui.templateCombox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTemplateChanged(int)));

  /*
   * Value Changed Signals
   */
  connect(ui.lineColor, SIGNAL(pressed()), this, SLOT(setLine()));
  connect(ui.lineColorGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.lineWidth, SIGNAL(valueChanged(double)), this, SLOT(setLine()));
  connect(ui.lineWidthGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.linePen, SIGNAL(currentIndexChanged(int)), this, SLOT(setLine()));
  connect(ui.linePenGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.fillColor, SIGNAL(pressed()), this, SLOT(setFill()));
  connect(ui.fillColorGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.fillBkcolor, SIGNAL(pressed()), this, SLOT(setFill()));
  connect(ui.fillBkcolorGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.fillLineWidth, SIGNAL(valueChanged(double)), this, SLOT(setFill()));
  connect(ui.fillLineWidthGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.fillLinePen, SIGNAL(currentIndexChanged(int)), this, SLOT(setFill()));
  connect(ui.fillLinePenGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.fillBrush, SIGNAL(currentIndexChanged(int)), this, SLOT(setFill()));
  connect(ui.fillBrushGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.textColor, SIGNAL(pressed()), this, SLOT(setText()));
  connect(ui.textColorGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.textFont, SIGNAL(currentIndexChanged(int)), this, SLOT(setText()));
  connect(ui.textFontGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.textSizePt, SIGNAL(valueChanged(int)), this, SLOT(setText()));
  connect(ui.textSizePtGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.textBold, SIGNAL(stateChanged(int)), this, SLOT(setText()));
  connect(ui.textBoldGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.textItalic, SIGNAL(stateChanged(int)), this, SLOT(setText()));
  connect(ui.textItalicGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
  connect(ui.textUnderline, SIGNAL(stateChanged(int)), this, SLOT(setText()));
  connect(ui.textUnderlineGlobal, SIGNAL(stateChanged(int)), this, SLOT(updateGlobals()));
}

void SchematicGraphicsStyleDialog::onLineColorPress()
{
  QColor color = QColorDialog::getColor(m_line.pen().color());
  m_line.setColor(templatedColor(color.red(), color.green(), color.blue()));
  setColorSelectionButton(ui.lineColor, m_line.color());
}
void SchematicGraphicsStyleDialog::onFillColorPress()
{
  QColor color = QColorDialog::getColor(m_line.pen().color());
  m_fill.setColor(templatedColor(color.red(), color.green(), color.blue()));
  setColorSelectionButton(ui.fillColor, m_fill.color());
}
void SchematicGraphicsStyleDialog::onFillBkcolorPress()
{
  QColor color = QColorDialog::getColor(m_line.pen().color());
  m_fill.setBkcolor(templatedColor(color.red(), color.green(), color.blue()));
  setColorSelectionButton(ui.fillColor, m_fill.bkcolor());
}

void SchematicGraphicsStyleDialog::onTextColorPress()
{
  QColor color = QColorDialog::getColor(m_line.pen().color());
  m_text.setColor(templatedColor(color.red(), color.green(), color.blue()));
  setColorSelectionButton(ui.fillColor, m_fill.bkcolor());
}

void SchematicGraphicsStyleDialog::onAccept()
{
  accept();
}

} // namespace schematic
