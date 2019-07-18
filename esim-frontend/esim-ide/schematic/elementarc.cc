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

#include "elementarc.h"
#include "renderdevice.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "util.h"
#include "dialogs/schematic-graphicstyledialog.h"
#include <cmath>

namespace schematic
{

ElementArc::ElementArc(int x1, int y1, int radius, int startAngle, int spanAngle) :
    Element(),
    m_x1(x1), m_y1(y1),
    m_sX1(0), m_sY1(0), m_sX2(0), m_sY2(0),
    m_distance(0),
    m_radius(radius),
    m_startAngle(startAngle),
    m_spanAngle(spanAngle),
    m_boundX1(0), m_boundY1(0),
    m_boundX2(0), m_boundY2(0),
    m_lineStyle(*(StyleTemplates::getInstance()->getLineStyleRef("component")))
{
}

const char *ElementArc::classname() const
{
  return "arc";
}

int ElementArc::locationType() const
{
  return S0_POS_2P1D;
}

void ElementArc::locate(int opcode, int paramSize, ...)
{
  va_list vlist;
  va_start(vlist, paramSize);
  switch(opcode)
  {
    case locateSetPos:
      ASSERT_PARAMS_RETURN (paramSize == 5 * sizeof(int));
        {
          int x1, y1;
          m_sX1 = va_arg(vlist, int);
          m_sY1 = va_arg(vlist, int);
          m_sX2 = va_arg(vlist, int);
          m_sY2 = va_arg(vlist, int);
          m_distance = std::abs(va_arg(vlist, int));

          float C = 0;
          if (m_sX1 == m_sX2)
            { C = (m_sY2 < m_sY1) ? (M_PI / 2.0) : (3.0 * M_PI / 2.0); }
          else
            { C = std::atan2(m_sY1-m_sY2, m_sX2-m_sX1); }
          if (C < 0)
            { C += 2 * M_PI; }

          /*
           * Calculate the position of origin point and radius of arc.
           */
          int x0 = (m_sX1+m_sX2)>>1, y0 = (m_sY1+m_sY2)>>1;
          x1 = int(x0 - m_distance*std::sin(C));
          y1 = int(y0 - m_distance*std::cos(C));

          float L = std::sqrt((m_sX2-m_sX1)*(m_sX2-m_sX1) + (m_sY2-m_sY1)*(m_sY2-m_sY1));
          int radius = int(std::sqrt(m_distance*m_distance+L*L/4));

          int a0 = int(std::atan2(y1-m_sY1, m_sX1-x1) * 180 * 16 / M_PI);
          if (a0 < 0)
            { a0 += 360 << 4; }
          int span = 0;
          if (L/2/radius <= 1)
            { span = int(2 * std::asin(L/2/radius) * 180 * 16 / M_PI); }
          else
            { span = 180 << 4; }

          m_x1 = x1; m_y1 = y1;
          m_radius = radius;
          m_startAngle = a0;
          m_spanAngle = span;
          update();
        }
      break;
    case locateGetPos:
      ASSERT_PARAMS_RETURN (paramSize == 5 * sizeof(int*));

      *va_arg(vlist, int*) = m_sX1;
      *va_arg(vlist, int*) = m_sY1;
      *va_arg(vlist, int*) = m_sX2;
      *va_arg(vlist, int*) = m_sY2;
      *va_arg(vlist, int*) = m_distance;
      break;
    case locateSetArc:
      ASSERT_PARAMS_RETURN (paramSize == 5 * sizeof(int));

      m_x1 = va_arg(vlist, int);
      m_y1 = va_arg(vlist, int);
      m_radius = va_arg(vlist, int);
      m_startAngle = va_arg(vlist, int);
      m_spanAngle = va_arg(vlist, int);
      break;
    case locateGetArc:
      ASSERT_PARAMS_RETURN (paramSize == 5 * sizeof(int*));

      *va_arg(vlist, int*) = m_x1;
      *va_arg(vlist, int*) = m_y1;
      *va_arg(vlist, int*) = m_radius;
      *va_arg(vlist, int*) = m_startAngle;
      *va_arg(vlist, int*) = m_spanAngle;
      break;

    default:
      assert(0);
  }
  va_end(vlist);
  return;
}

void ElementArc::render(RenderDevice *device)
{
  LineStyle oldline = device->setLineStyle(m_lineStyle);

  int cx1 = m_x1-m_radius;
  int cy1 = m_y1-m_radius;

  device->arc(cx1, cy1, m_radius<<1, m_radius<<1, m_startAngle, m_spanAngle);

  device->setLineStyle(oldline);
}

int ElementArc::standardizeAngle(int angle)
{
  while(angle < 0 || angle >= 360)
    {
      if (angle >= 360)
        { angle -= 360; }
      if (angle < 0)
        { angle += 360; }
    }
  return angle;
}

void ElementArc::update()
{
  float a0_rad = m_startAngle/16.0 * M_PI / 180.0;
  float span_rad = m_spanAngle/16.0 * M_PI / 180.0;
  int x1 = m_x1 + m_radius * std::cos(a0_rad);
  int y1 = m_y1 - m_radius * std::sin(a0_rad);
  int x2 = m_x1 + m_radius * std::cos(a0_rad+span_rad);
  int y2 = m_y1 - m_radius * std::sin(a0_rad+span_rad);
  m_boundX1 = s_min(x1, x2);
  m_boundY1 = s_min(y1, y2);
  m_boundX2 = s_max(x1, x2);
  m_boundY2 = s_max(y1, y2);

  static int angles[] = {0, 90<<4, 180<<4, 270<<4, 360<<4, 450<<4, 540<<4};
  for(int i=0; i < 7; i++)
    {
      int endAngle = m_startAngle+m_spanAngle;
      int startAngle = m_startAngle;

      if (startAngle <= angles[i] && endAngle >= angles[i])
        {
          float arad = angles[i]/16.0 * M_PI / 180.0;

          m_boundX1 = s_min(m_boundX1, m_x1 + m_radius * std::cos(arad));
          m_boundY1 = s_min(m_boundY1, m_y1 - m_radius * std::sin(arad));
          m_boundX2 = s_max(m_boundX2, m_x1 + m_radius * std::cos(arad));
          m_boundY2 = s_max(m_boundY2, m_y1 - m_radius * std::sin(arad));
        }
    }
}

void ElementArc::bounding(int *x1, int *y1, int *x2, int *y2) const
{
  *x1 = m_boundX1; *y1 = m_boundY1;
  *x2 = m_boundX2, *y2 = m_boundY2;
}


int ElementArc::serialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::serialize(dom));

  RC_CHECK_RETURN(dom->ser(m_x1));
  RC_CHECK_RETURN(dom->ser(m_y1));
  RC_CHECK_RETURN(dom->ser(m_sX1));
  RC_CHECK_RETURN(dom->ser(m_sY1));
  RC_CHECK_RETURN(dom->ser(m_sX2));
  RC_CHECK_RETURN(dom->ser(m_sY2));
  RC_CHECK_RETURN(dom->ser(m_distance));

  RC_CHECK_RETURN(m_lineStyle.serialize(dom));
  return 0;
}

int ElementArc::deserialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::deserialize(dom));

  RC_CHECK_RETURN(dom->des(&m_x1));
  RC_CHECK_RETURN(dom->des(&m_y1));
  RC_CHECK_RETURN(dom->des(&m_sX1));
  RC_CHECK_RETURN(dom->des(&m_sY1));
  RC_CHECK_RETURN(dom->des(&m_sX2));
  RC_CHECK_RETURN(dom->des(&m_sY2));
  RC_CHECK_RETURN(dom->des(&m_distance));

  RC_CHECK_RETURN(m_lineStyle.deserialize(dom));

  this->locate(locateSetPos, 5*sizeof(int), m_sX1, m_sY1, m_sX2, m_sY2, m_distance);
  return 0;
}

void ElementArc::config()
{
  SchematicGraphicsStyleDialog *dialog = new SchematicGraphicsStyleDialog(m_lineStyle);
  if (dialog->exec() == QDialog::Accepted)
    {
      m_lineStyle = dialog->lineStyle();
    }
  dialog->deleteLater();
}

} // namespace schematic
