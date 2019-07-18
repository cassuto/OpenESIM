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

#include "elementchord.h"
#include "renderdevice.h"
#include "util.h"
#include "dialogs/schematic-graphicstyledialog.h"

namespace schematic
{

ElementChord::ElementChord(int x1, int y1, int radius, int startAngle, int spanAngle) :
    ElementArc(x1, y1, radius, startAngle, spanAngle)
{
}

const char *ElementChord::classname() const
{
  return "chord";
}

void ElementChord::render(RenderDevice *device)
{
  LineStyle oldline = device->setLineStyle(m_lineStyle);

  int cx1 = m_x1-m_radius;
  int cx2 = m_x1-m_radius;
  int cy1 = m_y1+m_radius;
  int cy2 = m_y1+m_radius;

  device->chord(cx1, cy1, cx2-cx1+1, cy2-cy1+1, m_startAngle, m_spanAngle);

  device->setLineStyle(oldline);
}

void ElementChord::config()
{
  SchematicGraphicsStyleDialog *dialog = new SchematicGraphicsStyleDialog(m_lineStyle);
  if (dialog->exec() == QDialog::Accepted)
    {
      m_lineStyle = dialog->lineStyle();
    }
  dialog->deleteLater();
}

} // namespace schematic
