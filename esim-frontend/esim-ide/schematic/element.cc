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

#include "element.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "schematic.h"
#include "renderdevice.h"
#include "util.h"
#include <sstream>
#include <cassert>
#include <cmath>
#include <climits>
#include <QPainter>

namespace schematic
{

Element::Element() :
    m_selected(false),
    m_modifiable(true)
{
}

Element::~Element()
{
}

int Element::serialize(persistence::PersistenceDom *dom)
{
  return 0;
}

int Element::deserialize(persistence::PersistenceDom *dom)
{
  return 0;
}

void Element::origin(int *x, int *y)
{
  int x1, y1, x2, y2;
  this->bounding(&x1, &y1, &x2, &y2);
  *x = (x1 + x2)/2;
  *y = (y1 + y2)/2;
}

/**
 * @brief Get the string message of element position.
 * @return strings.
 */
void Element::getLocationMsg(std::string *msg)
{
  msg->clear();

  std::stringstream ss;

  int type = this->locationType();
  if (type & S0_POS_2P)
    {
      int x1, y1, x2, y2;
      this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
      ss << "(" << x1 << ", " << y1 << ")-(" << x2 << ", " << y2 << ")";
    }
  else if (type & S0_POS_1P)
    {
      int x1, y1;
      this->locate(locateGetPos, 2 * sizeof(int*), &x1, &y1);
      ss << "(" << x1 << ", " << y1 << ")";
    }
  else if (type & S0_POS_4P)
    {
      int x1, y1, x2, y2;
      this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
      ss << "(" << x1 << ", " << y1 << ")-(" << x2 << ", " << y2 << ")";
    }
  else if (type & S0_POS_1P1R)
    {
      int x1, y1, radus;
      this->locate(locateGetPos, 3 * sizeof(int*), &x1, &y1, &radus);
      ss << "(" << x1 << ", " << y1 << ")-" << radus;
    }
  else if (type & S0_POS_2P1D)
    {
      int x1, y1, radus, a0, a1;
      this->locate(locateGetPos, 5 * sizeof(int*), &x1, &y1, &radus, &a0, &a1);
      ss << "(" << x1 << ", " << y1 << ")-" << radus << "-(" << a0 << " rad, " << a1 << " rad)";
    }
  msg->assign(ss.str());
}

void Element::moveDelta(int dx, int dy)
{
  int x1, y1, x2, y2, radius, distance;

  int type = this->locationType();
  if (type & S0_POS_2P)
    {
      this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
      x1 += dx;
      x2 += dx;
      y1 += dy;
      y2 += dy;
      this->locate(locateSetPos, 4 * sizeof(int), x1, y1, x2, y2);
    }
  else if (type & S0_POS_1P)
    {
      this->locate(locateGetPos, 2 * sizeof(int*), &x1, &y1);
      x1 += dx;
      y1 += dy;
      this->locate(locateSetPos, 2 * sizeof(int), x1, y1);
    }
  else if (type & S0_POS_4P)
    {
      this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
      x1 += dx;
      x2 += dx;
      y1 += dy;
      y2 += dy;
      this->locate(locateSetPos, 4 * sizeof(int), x1, y1, x2, y2);
    }
  else if (type & S0_POS_1P1R)
    {
      this->locate(locateGetPos, 3 * sizeof(int*), &x1, &y1, &radius);
      x1 += dx;
      y1 += dy;
      this->locate(locateSetPos, 3 * sizeof(int), x1, y1, radius);
    }
  else if (type & S0_POS_2P1D)
    {
      this->locate(locateGetPos, 5 * sizeof(int*), &x1, &y1, &x2, &y2, &distance);
      x1 += dx;
      y1 += dy;
      x2 += dx;
      y2 += dy;
      this->locate(locateSetPos, 5 * sizeof(int), x1, y1, x2, y2, distance);
    }
}

void Element::getAnchors(int *anctype, int *num)
{
  *anctype = AnchorNone;
  *num = 0;

  int type = this->locationType();
  if (type & S0_POS_2P)
    {
      *anctype = AnchorPoint;
      *num = 2;
    }
  else if (type & S0_POS_4P)
    {
      *anctype = AnchorPoint;
      *num = 4;
    }
  else if (type & S0_POS_1P1R)
    {
      *anctype = AnchorOrthogonal;
      *num = 4;
    }
  else if (type & S0_POS_2P1D)
    {
      *anctype = AnchorTshaped;
      *num = 3;
    }
}

/**
 * @brief Moving the anchor point
 * @param type
 * @param point
 * @param cx
 * @param cy
 * @param dx
 * @param dy
 */
void Element::moveAnchor(int type, int point, int cx, int cy, int dx, int dy)
{
  switch (type)
  {
    case AnchorPoint:
      {
        int x1, y1, x2, y2;

        int type = this->locationType();

        if (type & S0_POS_2P)
          {
            this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
            switch (point)
            {
              case 0:
                x1 += dx;
                y1 += dy;
                break;
              case 1:
                x2 += dx;
                y2 += dy;
                break;
            }
            this->locate(locateSetPos, 4 * sizeof(int), x1, y1, x2, y2);
          }
        else if (type & S0_POS_4P)
          {
            this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
            switch (point)
            {
              case 0:
                x1 += dx;
                y1 += dy;
                break;
              case 1:
                y1 += dy;
                x2 += dx;
                break;
              case 2:
                x1 += dx;
                y2 += dy;
                break;
              case 3:
                x2 += dx;
                y2 += dy;
                break;
            }
            this->locate(locateSetPos, 4 * sizeof(int), x1, y1, x2, y2);
          }
        else
          assert(0);
        break;
      }

    case AnchorOrthogonal:
      {
        int x1, y1, radus;
        this->locate(locateGetPos, 3 * sizeof(int*), &x1, &y1, &radus);
        switch (point)
        {
          case 0:
            radus = cy - y1;
            break;
          case 1:
            radus = cx - x1;
            break;
          case 2:
            radus = y1 - cy;
            break;
          case 3:
            radus = x1 - cx;
            break;
        }
        this->locate(locateSetPos, 3 * sizeof(int), x1, y1, radus);
        break;
      }

    case AnchorTshaped:
      {
        int sX1, sY1, sX2, sY2, distance;
        this->locate(locateGetPos, 5 * sizeof(int*), &sX1, &sY1, &sX2, &sY2, &distance);

        switch (point)
        {
          case 0:
            sX1 = cx;
            sY1 = cy;
            break;
          case 1:
            sX2 = cx;
            sY2 = cy;
            break;
          case 2:
            float mod = std::sqrt((sX2-sX1)*(sX2-sX1) + (sY2-sY1)*(sY2-sY1));
            int d = int(((cx-sX1)*(sY2-sY1)-(cy-sY1)*(sX2-sX1)) / mod);
            if (d >= 0)
              { distance = d; }
            else
              { return; } // cancel
            break;
        }
        this->locate(locateSetPos, 5 * sizeof(int*), sX1, sY1, sX2, sY2, distance);
        break;
      }
  }

}

void Element::renderAnchors(RenderDevice *device, Schematic *document)
{
  int anchorType, numPoint;
  this->getAnchors(&anchorType, &numPoint);
  int x1, y1, x2, y2, radius, a1, a2;

  switch (anchorType)
  {
    case AnchorPoint:
      {
        this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
        int cx1 = document->mapToDeviceX(x1), cy1 = document->mapToDeviceY(y1);
        int cx2 = document->mapToDeviceX(x2), cy2 = document->mapToDeviceY(y2);

        switch (numPoint)
        {
          case 2:
            document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx1, cy1);
            document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx2, cy2);
            document->appendAnchor(Schematic::anchor(this, anchorType, 0, x1, y1));
            document->appendAnchor(Schematic::anchor(this, anchorType, 1, x2, y2));
            break;

          case 4:
            document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx1, cy1);
            document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx2, cy1);
            document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx1, cy2);
            document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx2, cy2);
            document->appendAnchor(Schematic::anchor(this, anchorType, 0, x1, y1));
            document->appendAnchor(Schematic::anchor(this, anchorType, 1, x2, y1));
            document->appendAnchor(Schematic::anchor(this, anchorType, 2, x1, y2));
            document->appendAnchor(Schematic::anchor(this, anchorType, 3, x2, y2));
            break;
        }
      }
      break;

    case AnchorOrthogonal:
      {
        this->locate(locateGetPos, 3 * sizeof(int*), &x1, &y1, &radius);
        int cx = document->mapToDeviceX(x1), cy = document->mapToDeviceY(y1);
        int cr = document->mapToDeviceDx(radius);

        document->paintAnchor(device, Schematic::ANCHOR_CROSS_1, cx, cy); // small cross at origin

        document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx, cy+cr);
        document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx+cr, cy);
        document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx, cy-cr);
        document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx-cr, cy);
        document->appendAnchor(Schematic::anchor(this, anchorType, 0, x1, y1+radius));
        document->appendAnchor(Schematic::anchor(this, anchorType, 1, x1+radius, y1));
        document->appendAnchor(Schematic::anchor(this, anchorType, 2, x1, y1-radius));
        document->appendAnchor(Schematic::anchor(this, anchorType, 3, x1-radius, y1));
      }
      break;

    case AnchorTshaped:
      {
        this->locate(locateGetArc, 5 * sizeof(int*), &x1, &y1, &radius, &a1, &a2);
        int sX1, sY1, sX2, sY2, distance;
        this->locate(locateGetPos, 5 * sizeof(int*), &sX1, &sY1, &sX2, &sY2, &distance);

        int cx1 = document->mapToDeviceX(x1), cy1 = document->mapToDeviceY(y1);
        int csX1 = document->mapToDeviceX(sX1), csY1 = document->mapToDeviceY(sY1);
        int csX2 = document->mapToDeviceX(sX2), csY2 = document->mapToDeviceY(sY2);

        document->paintAnchor(device, Schematic::ANCHOR_ELLIPSE_1, csX1, csY1);
        document->appendAnchor(Schematic::anchor(this, anchorType, 0, sX1, sY1));

        document->paintAnchor(device, Schematic::ANCHOR_ELLIPSE_2, csX2, csY2);
        document->appendAnchor(Schematic::anchor(this, anchorType, 1, sX2, sY2));

        document->paintAnchor(device, Schematic::ANCHOR_RECT_1, cx1, cy1);
        document->appendAnchor(Schematic::anchor(this, anchorType, 2, x1, y1));

        document->paintAnchor(device, Schematic::ANCHOR_LINE_1, cx1, cy1, csX1, csY1);
        document->paintAnchor(device, Schematic::ANCHOR_LINE_1, cx1, cy1, csX2, csY2);
        document->paintAnchor(device, Schematic::ANCHOR_LINE_1, csX1, csY1, csX2, csY2);
      }
      break;
  }
}

void Element::renderBounding(RenderDevice *device, int viewX1, int viewY1, float scaleX, float scaleY)
{
  QPen pen = device->painter->pen();
  int x1, y1, x2, y2, radius, a0, a1;
  int type = this->locationType();
  if (type & S0_POS_2P)
    {
      this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
      x1 = float(x1) * scaleX + viewX1, y1 = float(y1) * scaleY + viewY1;
      x2 = float(x2) * scaleX + viewX1, y2 = float(y2) * scaleY + viewY1;

      device->painter->drawLine(x1, y1, x2, y2);
    }
  else if (type & S0_POS_1P)
    {
      this->locate(locateGetPos, 2 * sizeof(int*), &x1, &y1);
      x1 = float(x1) * scaleX + viewX1, y1 = float(y1) * scaleY + viewY1;

      device->painter->drawPoint(x1, y1);
    }
  else if (type & S0_POS_4P)
    {
      this->locate(locateGetPos, 4 * sizeof(int*), &x1, &y1, &x2, &y2);
      x1 = float(x1) * scaleX + viewX1, y1 = float(y1) * scaleY + viewY1;
      x2 = float(x2) * scaleX + viewX1, y2 = float(y2) * scaleY + viewY1;

      device->painter->drawRect(s_min(x1,x2), s_min(y1,y2), std::abs(x2-x1), std::abs(y2-y1));
    }
  else if (type & S0_POS_1P1R)
    {
      this->locate(locateGetPos, 3 * sizeof(int*), &x1, &y1, &radius);
      x1 = float(x1) * scaleX + viewX1, y1 = float(y1) * scaleY + viewY1;
      int radiusX = float(radius) * scaleX;
      int radiusY = float(radius) * scaleY;

      device->painter->drawEllipse(x1-radiusX, y1-radiusY, radiusX << 1, radiusY << 1);
    }
  else if (type & S0_POS_2P1D)
    {
      this->locate(locateGetArc, 5 * sizeof(int*), &x1, &y1, &radius, &a0, &a1);
      x1 = float(x1) * scaleX + viewX1, y1 = float(y1) * scaleY + viewY1;
      int radiusX = float(radius) * scaleX;
      int radiusY = float(radius) * scaleY;

      device->painter->drawArc(QRect(x1-radiusX, y1-radiusY, radiusX<<1, radiusY<<1), a0, a1);
    }
  device->painter->setPen(pen);
}

} // namespace schematic
