/* ****************************************************************************
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
 * ****************************************************************************/

#include "renderdevice.h"
#include "schematic.h"
#include "styletemplate.h"

#include <QtCore/QRect>
#include <QtGui/QFont>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>

namespace schematic {

RenderDevice::RenderDevice(Schematic *doc, QPainter *painter) :
  GraphAntiAliasing(true),
  TextAntiAliasing(true),
  painter(painter),
  scale(doc->getScale()),
  fontScale(doc->getFontScale()),
  m_doc(doc)
{
  painter->setWorldMatrixEnabled(false); // we use our own coordinate transformation
  if (fontScale == 0)
   fontScale = scale;
  QFont f = painter->font();
  f.setPointSizeF( fontScale * float(f.pointSize()) );
  painter->setFont(f);
  lineSpacing = painter->fontMetrics().lineSpacing();

  QPainter::RenderHints hints = 0;
  // Enable antialias drawings if requested
  if (GraphAntiAliasing) hints |= QPainter::Antialiasing;
  // Enable antialias text if requested
  if (TextAntiAliasing) hints |= QPainter::TextAntialiasing;
  painter->setRenderHints(hints);
}

/**
 * Set line style of current context.
 * @param line
 * @return last style (but without any symbol information)
 */
LineStyle RenderDevice::setLineStyle(const LineStyle &line)
{
  QPen pen = painter->pen();
  painter->setPen(line.pen());
  return LineStyle("", pen);
}

/**
 * Set fill style of current context.
 * @param fill
 * @return last style (but without any symbol information)
 */
FillStyle RenderDevice::setFillStyle(const FillStyle &fill)
{
  QPen pen = painter->pen();
  painter->setPen(fill.pen());
  QBrush brush = painter->brush();
  painter->setBrush(fill.brush());
  return FillStyle("", pen, brush);
}

/**
 * Set text style of current context.
 * @param text
 * @return last style (but without any symbol information)
 */
TextStyle RenderDevice::setTextStyle(const TextStyle &text)
{
  QPen pen = painter->pen();
  painter->setPen(text.pen());
  QFont font = text.font();
  painter->setFont(font);
  return TextStyle("", pen, font);
}

/**
 * @brief Draw a point on painter.
 * @param x
 * @param y
 */
void RenderDevice::point(int x, int y)
{
  int xd, yd;
  m_doc->mapToDevice(&xd, &yd, x, y);
  painter->drawPoint(QPointF(xd, yd));
}

/**
 * @brief Draw a line on painter.
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void RenderDevice::line(int x1, int y1, int x2, int y2)
{
  int x1d, x2d, y1d, y2d;
  m_doc->mapToDevice(&x1d, &y1d, x1, y1);
  m_doc->mapToDevice(&x2d, &y2d, x2, y2);
  painter->drawLine(QLineF(x1d, y1d, x2d, y2d));
}

/**
 * @brief Draw a rect on painter.
 * @param x
 * @param y
 * @param dx
 * @param dy
 */
void RenderDevice::rect(int x, int y, int dx, int dy)
{
  int xd, yd;
  m_doc->mapToDevice(&xd, &yd, x, y);
  painter->drawRect(QRectF(xd, yd, m_doc->mapToDeviceDx(dx), m_doc->mapToDeviceDy(dy)));
}

/**
 * @brief Draw an ellipse on painter.
 * @param x
 * @param y
 * @param dx
 * @param dy
 */
void RenderDevice::ellipse(int x, int y, int dx, int dy)
{
  int xd, yd;
  m_doc->mapToDevice(&xd, &yd, x, y);
  painter->drawEllipse(QRectF(xd, yd, m_doc->mapToDeviceDx(dx), m_doc->mapToDeviceDy(dy)));
}

/**
 * @brief Draw an arc on painter.
 * @param x
 * @param y
 * @param dx
 * @param dy
 * @param startAngle
 * @param spanAngle
 */
void RenderDevice::arc(int x, int y, int dx, int dy, int startAngle, int spanAngle)
{
  int xd, yd;
  m_doc->mapToDevice(&xd, &yd, x, y);
  painter->drawArc(QRectF(xd, yd, m_doc->mapToDeviceDx(dx), m_doc->mapToDeviceDy(dy)), startAngle, spanAngle);
}

/**
 * @brief Draw a chord on painter.
 * @param x
 * @param y
 * @param dx
 * @param dy
 * @param startAngle = 16 * degrees
 * @param spanAngle = 16 * degrees
 */
void RenderDevice::chord(int x, int y, int dx, int dy, int startAngle, int spanAngle)
{
  int xd, yd;
  m_doc->mapToDevice(&xd, &yd, x, y);
  painter->drawChord(QRectF(xd, yd, m_doc->mapToDeviceDx(dx), m_doc->mapToDeviceDy(dy)), startAngle, spanAngle);
}

/**
 * @brief Draw a text on painter.
 * @param text
 * @param x
 * @param y
 * @param height Optional, Where to store the height of drew text.
 * @param angle
 * @return width.
 */
int RenderDevice::text(const char *text, int x, int y, int *height/*=0l*/, int angle/*=0*/)
{
  int xd, yd;
  m_doc->mapToDevice(&xd, &yd, x, y);
  QRect rf = painter->boundingRect(QRect(xd, yd, 0, 0), Qt::TextDontClip, text);

  painter->save();

  if (angle != 180)
    {
      painter->translate(xd, yd);
      painter->scale(scale, scale);
      painter->rotate(angle);
    }
  else
    {
      painter->translate(xd - rf.width(), yd);
      painter->scale(scale, scale);
      painter->rotate(0);
    }

  painter->drawText(QRect(0, 0, 0, 0), Qt::TextDontClip, text);

  painter->restore();

  if (height)
    *height = rf.height();
  return rf.width();
}

/**
 * @brief Get the size of given text under text style.
 * @param text
 * @param style
 * @param width Where to store the width.
 * @param height Where to store the height.
 */
void RenderDevice::getTextSize(const char *text, const TextStyle &style, int *width, int *height)
{
  QFontMetrics m = QFontMetrics(style.font());
  QSize s = m.size(0, text); // get size of text
  *width = s.width();
  *height = s.height();
}

} // namesapce
