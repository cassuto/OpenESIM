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

#include "elementtext.h"
#include "renderdevice.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "util.h"
#include <cmath>
#include "dialogs/schematic-graphicstyledialog.h"

namespace schematic
{

ElementText::ElementText(int x1, int y1, const std::string &text) :
    Element(),
    m_x1(x1),
    m_y1(y1),
    m_text(text),
    m_angle(0),
    m_textWidth(0),
    m_textHeight(0),
    m_textStyle(*(StyleTemplates::getInstance()->getTextStyleRef("component")))
{
  setText(text);
}

const char *ElementText::classname() const
{
  return "text";
}

int ElementText::locationType() const
{
  return S0_POS_1P | S1_ROTATION_1A;
}

void ElementText::locate(int opcode, int paramSize, ...)
{
  va_list vlist;
  va_start(vlist, paramSize);
  switch(opcode)
  {
    case locateSetPos:
      ASSERT_PARAMS_RETURN (paramSize == 2 * sizeof(int));

      m_x1 = va_arg(vlist, int);
      m_y1 = va_arg(vlist, int);
      break;
    case locateGetPos:
      ASSERT_PARAMS_RETURN (paramSize == 2 * sizeof(int*));

      *va_arg(vlist, int*) = m_x1;
      *va_arg(vlist, int*) = m_y1;
      break;
    case locateSetRotation:
      ASSERT_PARAMS_RETURN (paramSize == 1 * sizeof(int));
        {
          int ca = va_arg(vlist, int);
          int angle = std::abs(ca);
          int coeff = ca > 0 ? 1 : -1;
          while (angle > 360)
            { angle -= 360; }
          if (0 <= angle && angle < 90) // only do angles in four directios are used
            { angle = 0; }
          else if (90 <= angle && angle < 180)
            { angle = 90; }
          else if (180 <= angle && angle < 270)
            { angle = 180; }
          else if (270 <= angle && angle < 360)
            { angle = 270; }
          angle *= coeff;
          m_angle = (angle < 0) ? (angle + 360) : angle; // normalize the angle to positive
        }
      break;
    case locateGetRotation:
      ASSERT_PARAMS_RETURN (paramSize == 1 * sizeof(int*));

      *va_arg(vlist, int*) = m_angle;
      break;
  }
  va_end(vlist);
  return;
}

void ElementText::setText(const std::string &text)
{
  m_text = text;
  this->update();
}

void ElementText::setTextStyle(const TextStyle &style)
{
  m_textStyle = style;
  this->update();
}

/**
 * @brief this must be called after the text or font has been changed.
 */
void ElementText::update()
{
  RenderDevice::getTextSize(m_text.c_str(), m_textStyle, &m_textWidth, &m_textHeight);
}

void ElementText::render(RenderDevice *device)
{
  TextStyle oldtext = device->setTextStyle(m_textStyle);

  device->text(m_text.c_str(), m_x1, m_y1, 0l, m_angle);

  device->setTextStyle(oldtext);
}

void ElementText::bounding(int *x1, int *y1, int *x2, int *y2) const
{
  switch (m_angle)
  {
    case 0:
      *x1 = m_x1;                *y1 = m_y1;
      *x2 = m_x1 + m_textWidth;  *y2 = m_y1 + m_textHeight;
      break;
    case 90:
      *x1 = m_x1 - m_textHeight; *y1 = m_y1;
      *x2 = m_x1;                *y2 = m_y1 + m_textWidth;
      break;
    case 180:
      *x1 = m_x1 - m_textWidth;  *y1 = m_y1;
      *x2 = m_x1;                *y2 = m_y1 + m_textHeight;
      break;
    case 270:
      *x1 = m_x1;                *y1 = m_y1 - m_textWidth;
      *x2 = m_x1 + m_textHeight; *y2 = m_y1;
      break;
  }

  *x1 = s_min(*x1, *x2); *y1 = s_min(*y1, *y2);
  *x2 = s_max(*x1, *x2), *y2 = s_max(*y1, *y2);
}

int ElementText::serialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::serialize(dom));

  RC_CHECK_RETURN(dom->ser(m_x1));
  RC_CHECK_RETURN(dom->ser(m_y1));
  RC_CHECK_RETURN(dom->ser(m_text));
  RC_CHECK_RETURN(dom->ser(m_angle));

  RC_CHECK_RETURN(m_textStyle.serialize(dom));
  return 0;
}

int ElementText::deserialize(persistence::PersistenceDom *dom)
{
  RC_CHECK_RETURN(Element::deserialize(dom));

  RC_CHECK_RETURN(dom->des(&m_x1));
  RC_CHECK_RETURN(dom->des(&m_y1));
  RC_CHECK_RETURN(dom->des(&m_text));
  RC_CHECK_RETURN(dom->des(&m_angle));

  RC_CHECK_RETURN(m_textStyle.deserialize(dom));

  this->update();
  return 0;
}

void ElementText::config()
{
  SchematicGraphicsStyleDialog *dialog = new SchematicGraphicsStyleDialog(m_textStyle);
  if (dialog->exec() == QDialog::Accepted)
    {
      m_textStyle = dialog->textStyle();
      this->update();
    }
  dialog->deleteLater();
}

} // namespace schematic
