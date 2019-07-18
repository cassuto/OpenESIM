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
#include "schematic.h"
#include "element.h"
#include "renderdevice.h"
#include <QPainter>
#include <QResizeEvent>
#include <QRect>

namespace schematic
{

////////////////////////////////////////////////////////////////////////////////
// Element preview                                                            //
////////////////////////////////////////////////////////////////////////////////

PreviewElement::PreviewElement(QWidget *parent /*= 0l*/) :
    QWidget(parent),
    m_document(new Schematic(/*symbolMode*/false))
{
  m_document->setShowGrid(false);
  m_document->setShowFrame(false);
}

PreviewElement::~PreviewElement()
{
  delete m_document;
}

void PreviewElement::resizeEvent(QResizeEvent * event)
{
  int w = event->size().width(), h = event->size().height();
  m_document->setSizeRect(0, 0, w, h);
  emit resize(w, h);
}

void PreviewElement::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  RenderDevice device(m_document, &painter);
  QRect rect(event->rect());
  m_document->render(&device, rect.top(), rect.left(), rect.width(), rect.height(), /*preview*/false);
}

////////////////////////////////////////////////////////////////////////////////
// Schematic preview                                                          //
////////////////////////////////////////////////////////////////////////////////

PreviewSchematic::PreviewSchematic(Schematic *document, QWidget *parent /*= 0l*/) :
    QWidget(parent),
    m_document(document),
    m_thresholdX(-1),
    m_thresholdY(-1)
{
}

PreviewSchematic::~PreviewSchematic()
{
}

void PreviewSchematic::resizeEvent(QResizeEvent * event)
{
  int w = event->size().width(), h = event->size().height();
  m_thresholdX = int(w * 0.1);
  m_thresholdY = int(h * 0.1);
  emit resize(w, h);
}

void PreviewSchematic::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  RenderDevice device(m_document, &painter);
  QRect rect(event->rect());
  m_document->render(&device, rect.top(), rect.left(), rect.width(), rect.height(),
                     /*preview*/true, m_thresholdX, m_thresholdY);
}

}
