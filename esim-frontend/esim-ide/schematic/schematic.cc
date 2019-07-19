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

#include "schematic.h"
#include "renderdevice.h"
#include "persistence.h"
#include "persistencedom.h"
#include "errtrace.h"
#include "element.h"
#include "util.h"

#include <QtGui/QPen>
#include <QtGui/QPainter>
#include <QtGui/QColor>
#include <QtGui/QBrush>
#include <cmath>
#include <cstring>

namespace schematic {

int Schematic::standardSizes[N_STANDARD_SIZES][2] = {
  {1020, 765},  // DIN A5 landscape
  {765, 1020},  // DIN A5 portrait
  {1530, 1020}, // DIN A4 landscape
  {1020, 1530}, // DIN A4 portrait
  {2295, 1530}, // DIN A3 landscape
  {1530, 2295}, // DIN A3 portrait
  {1414, 1054}, // letter landscape
  {1054, 1414}, // letter portrait
};

#define SCHEMATIC_VERMAJ 1
#define SCHEMATIC_VERMIN 0

Schematic::Schematic(bool symbolMode) :
  m_scale(1),
  m_fontScale(0),
  m_anchorSize(10),
  m_crosslineSize(10),
  m_showGrid(true),
  m_gridX(10), m_gridY(10),
  m_gridR(200), m_gridG(200), m_gridB(200),
  m_backgroundR(255), m_backgroundG(255), m_backgroundB(255),
  m_symbolMode(symbolMode),
  m_sizeType(-1),
  m_anchor_dR(10),
  m_showFrame(true),
  m_currentAnchor(0l),
  m_amx1(0),
  m_amy1(0),
  m_amx2(0),
  m_amy2(0)
{

  m_persistence =  new persistence::Persistence(
                          symbolMode ?persistence::PERSISTENCE_SSYM : persistence::PERSISTENCE_SCH,
                          SCHEMATIC_VERMAJ, SCHEMATIC_VERMIN);

  setSizeRect(0, 0, 800, 800);

  m_frameText[0] = "Title";
  m_frameText[1] = "Drawn By:";
  m_frameText[2] = "Date:";
  m_frameText[3] = "Revision:";
}

Schematic::~Schematic()
{
  delete m_persistence;
}

/**
 * @brief Get the size of schematic.
 * @param type Type of schematic size.
 * @param width
 * @param height
 * @return true if the type is valied.
 */
bool Schematic::getStandardSize(int type, int *width, int *height)
{
  // Values exclude border of 1.5cm at each side.
  if(type >= 0 && type < N_STANDARD_SIZES) {
    *width = standardSizes[type][0]; *height = standardSizes[type][1];
  } else {
    *width = -1; *height = -1;
    return false;
  }
  return true;
}

/**
 * @brief Set the size rectangle of schematic.
 * @param x1
 * @param y1
 * @param x2
 * @param y2
 */
void Schematic::setSizeRect(int x1, int y1, int x2, int y2)
{
  m_viewX1 = x1; m_viewY1 = y1;
  m_viewX2 = x2; m_viewY2 = y2;
}

void Schematic::getSizeRect(int *x1, int *y1, int *x2, int *y2)
{
  *x1 = m_viewX1; *y1 = m_viewY1;
  *x2 = m_viewX2; *y2 = m_viewY2;
}

/**
 * @brief Sets an arbitrary coordinate onto the next grid coordinate.
 */
int Schematic::togridX(int x) const
{
  if (x < 0)
    x -= (m_gridX >> 1) - 1;
  else
    x += m_gridX >> 1;
  x -= x % m_gridX;
  return x;
}
int Schematic::togridY(int y) const
{
  if (y < 0)
    y -= (m_gridY >> 1) - 1;
  else
    y += m_gridY >> 1;
  y -= y % m_gridY;
  return y;
}
void Schematic::togrid(int *x, int *y) const {
  int cx = *x, cy = *y;
  *x = togridX(cx);
  *y = togridY(cy);
}

/**
 * @brief Map from logical coordinate to device coordinate.
 */
int Schematic::mapToDeviceX(int x) const {
  return float(x + m_viewX1) * m_scale;
}
int Schematic::mapToDeviceY(int y) const {
  return float(y + m_viewY1) * m_scale;
}
int Schematic::mapToDeviceDx(int dx) const {
  return float(dx) * m_scale;
}
int Schematic::mapToDeviceDy(int dy) const {
  return float(dy) * m_scale;
}
void Schematic::mapToDevice(int *x, int *y, int cx, int cy) const {
  *x = mapToDeviceX(cx);
  *y = mapToDeviceY(cy);
}

/**
 * @brief Map from device coordinate to logical coordinate.
 */
int Schematic::mapToLogicalX(int x) const {
  return float(x) / m_scale - m_viewX1;
}
int Schematic::mapToLogicalY(int y) const {
  return float(y) / m_scale - m_viewY1;
}
int Schematic::mapToLogicalDx(int dx) const {
  return float(dx) / m_scale;
}
int Schematic::mapToLogicalDy(int dy) const {
  return float(dy) / m_scale;
}
void Schematic::mapToLogical(int *x, int *y, int cx, int cy) const {
  *x = mapToLogicalX(cx);
  *y = mapToLogicalY(cy);
}

/**
 * @brief Get the logical width and height of schematic.
 */
int Schematic::width() const {
  return std::abs(m_viewX2 - m_viewX1);
}
int Schematic::height() const {
  return std::abs(m_viewY2 - m_viewY1);
}

void Schematic::clear()
{
  for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
    {
      delete (*it);
    }
  m_elements.clear();
}

/**
 * @brief Save the current document to stream.
 */
int Schematic::save(std::ostream & stream)
{ using namespace persistence;
  int rc = 0;
  m_persistence->clear();

  PersistenceDom *dom = m_persistence->append("schematic");
  if (RC_FAILURE(rc=dom->ser(m_sizeType)))
    { goto out; }
  if (RC_FAILURE(rc=dom->ser(m_viewX1)))
    { goto out; }
  if (RC_FAILURE(rc=dom->ser(m_viewY1)))
    { goto out; }
  if (RC_FAILURE(rc=dom->ser(m_viewX2)))
    { goto out; }
  if (RC_FAILURE(rc=dom->ser(m_viewY2)))
    { goto out; }

  for(int i=0; i < N_FRAME_TEXTS; i++)
    {
      if (RC_FAILURE(dom->ser(m_frameText[i])))
        { goto out; }
    }

  for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
    { Element *element = *it;

      dom = m_persistence->append(element->classname());
      if (RC_FAILURE(rc = element->serialize(dom)))
        { goto out; }
    }
  rc = m_persistence->save(stream);
out:
  m_persistence->clear();
  return rc;
}

/**
 * Woker - Called by m_persistence->open()
 */
int Schematic::openCallback(const char *classname, persistence::PersistenceDom *dom, void *)
{
  int rc = 0;
  Element *instance = 0l;
  if (std::strcmp(classname, "schematic") == 0)
    {
      if (RC_FAILURE(rc=dom->des(&m_sizeType)))
        { goto out; }
      if (RC_FAILURE(rc=dom->des(&m_viewX1)))
        { goto out; }
      if (RC_FAILURE(rc=dom->des(&m_viewY1)))
        { goto out; }
      if (RC_FAILURE(rc=dom->des(&m_viewX2)))
        { goto out; }
      if (RC_FAILURE(rc=dom->des(&m_viewY2)))
        { goto out; }

      for(int i=0; i < N_FRAME_TEXTS; i++)
        {
          if (RC_FAILURE(dom->des(&m_frameText[i])))
            { goto out; }
        }

      this->setSizeRect(m_viewX1, m_viewY1, m_viewX2, m_viewY2);
    }
  else
    {
      instance = appendElement(classname); // create instance of element
      if (!instance)
        { return -RC_UNKNOWN_ELEMENT; }
      if (RC_FAILURE(rc=instance->deserialize(dom)))
        { goto out; }
    }
  return 0;
out:
  removeElement(instance);
  return rc;
}

/**
 * @brief Open a schematic document from stream.
 */
int Schematic::open(std::istream & stream)
{
  using namespace persistence;
  int rc = 0;
  this->clear();

  /*
   * Do deserialization. This will call openCallback() for each DOM and be blocked
   * until all the jobs are done.
   */
  PersistenceCallback<Schematic> callback = PersistenceCallback<Schematic>(this, &Schematic::openCallback);
  if (RC_FAILURE(rc = m_persistence->open(stream, callback)))
    { goto out; }

out:
  m_persistence->clear();
  return rc;
}

int Schematic::exportElements(std::ostream & stream, const std::vector<Element *> &elements)
{
  int rc = 0;
  m_persistence->clear();
  for(std::size_t i=0; i < elements.size(); i++)
    {
      Element *element = elements[i];
      persistence::PersistenceDom *dom = m_persistence->append(element->classname());
      if (RC_FAILURE(rc = element->serialize(dom)))
        { goto out; }
    }
  return m_persistence->save(stream);
out:
  return rc;
}

struct ImportState {
  const std::vector<Element *> &elements;
  std::size_t pos;
};

/**
 * Woker - Called by m_persistence->open().
 */
int Schematic::importCallback(const char *classname, persistence::PersistenceDom *dom, void *opaque)
{
  int rc = 0;
  ImportState *state = static_cast<ImportState *>(opaque);

  if (state->pos < state->elements.size())
    {
      Element *instance = state->elements[state->pos++];
      if (!instance)
        { goto out; }
      if (RC_FAILURE(rc = instance->deserialize(dom)))
        { goto out; }
      return 0;
    }
  else
    { return -RC_FAILED; }

out:
  return rc;
}

int Schematic::importElements(std::istream & stream, const std::vector<Element *> &elements)
{
  using namespace persistence;
  int rc = 0;
  ImportState state = {elements, 0};

  PersistenceCallback<Schematic> callback = PersistenceCallback<Schematic>(this, &Schematic::importCallback);
  if (RC_FAILURE(rc = m_persistence->open(stream, callback, &state)))
    { goto out; }

out:
  m_persistence->clear();
  return rc;
}

/**
 * @brief Post event of painting. Accept logical position.
 */
void Schematic::postPaintEvent(int op, int a0, int a1, int a2, int a3, int a4, int a5)
{
  paintEvent event = {op, a0, a1, a2, a3, a4, a5};
  m_postedPaintEvents.push_back(event);
}

void Schematic::clearPaintEvent()
{
  m_postedPaintEvents.clear();
}

/**
 * @brief Select elements in specified rectangle area.
 */
void Schematic::selectElements(int x1, int y1, int x2, int y2)
{
    // exchange rectangle coordinates to obtain x1 < x2 and y1 < y2
    int cx1 = s_min(x1, x2);
    int cx2 = s_max(x1, x2);
    int cy1 = s_min(y1, y2);
    int cy2 = s_max(y1, y2);
    x1 = cx1, x2 = cx2;
    y1 = cy1, y2 = cy2;

    for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
      {
        Element *element = *it;
        if (element->isModifiable())
          {
            int bx1, by1, bx2, by2;
            element->bounding(&bx1, &by1, &bx2, &by2);
            if (bx1 >= x1 && bx2 <= x2 && by1 >= y1 && by2 <= y2)
              { element->setSelected(true); }
          }
      }
}

/**
 * @brief get all the elements that is located at the specified point
 * @param x
 * @param y
 * @param out Where to store the result (cleared by function).
 */
void Schematic::getElememntsByPos(int x, int y, std::vector<Element *> *out)
{
  out->clear();
  for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
    {
      Element *element = *it;
      if (element->isModifiable())
        {
          int x1, y1, x2, y2;
          element->bounding(&x1, &y1, &x2, &y2);

          if (x1 <= x && x2 >= x && y1 <= y && y2 >= y)
            { out->push_back(element); }
        }
    }
}

void Schematic::attachElement(Element *element)
{
  assert(std::find(m_elements.begin(), m_elements.end(), element) == m_elements.end());
  m_elements.push_front(element);
}
void Schematic::detachElement(Element *element)
{
  assert(std::find(m_elements.begin(), m_elements.end(), element) != m_elements.end());
  m_elements.remove(element);
}

/**
 * @brief Remove the target element from document, deleting its instance.
 * @param target
 */
void Schematic::removeElement(Element *target)
{
  if (target)
    { detachElement(target); }
  delete target;
}

/**
 * @brief Clear flags of all the selected elements
 */
void Schematic::clearSelectedElements()
{
  for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
    {
      (*it)->setSelected(false);
    }
}

void Schematic::getSelectedElements(std::vector<Element *> *out)
{
  out->clear();
  for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
    {
      Element *element = *it;
      if (element->selected())
        { out->push_back(element); }
    }
}

/**
 * @brief Handle mouse press for anchors.
 * @retval pointer to element occupying the anchor, if there is an anchor selected.
 * @retval Null if none anchor is selected.
 */
Element *Schematic::anchorPress(int x, int y)
{
  m_currentAnchor = 0l;
  for(std::size_t i=0; i < m_anchors.size(); i++)
    {
      int ax = m_anchors[i].x;
      int ay = m_anchors[i].y;
      int dR = m_anchor_dR/2;
      if (ax-dR <= x && ax+dR >= x &&
          ay-dR <= y && ay+dR >= y)
        {
          m_amx1 = this->togridX(x);
          m_amy1 = this->togridY(y);
          m_currentAnchor = &m_anchors[i];
          return m_currentAnchor->element;
        }
    }
  return 0l;
}

/**
 * @brief Handle mouse move for anchors.
 * @retval pointer to element occupying the anchor, if there is an anchor selected.
 * @retval Null if none anchor is selected.
 */
Element *Schematic::anchorMove(int x, int y)
{
  if (m_currentAnchor)
    {
      Element *element = m_currentAnchor->element;

      int cx = this->togridX(x);
      int cy = this->togridY(y);
      int dX = cx - m_amx1;
      int dY = cy - m_amy1;
      m_amx1 = cx;
      m_amy1 = cy;
      element->moveAnchor(m_currentAnchor->type, m_currentAnchor->point, cx, cy, dX, dY);

      return element;
    }
  return 0l;
}

/**
 * @brief Handle mouse release for anchors.
 * @retval pointer to element occupying the anchor, if there is an anchor selected.
 * @retval Null if none anchor is selected.
 */
Element *Schematic::anchorRelease(int x, int y)
{
  Element *element = m_currentAnchor ? m_currentAnchor->element : 0l;
  m_currentAnchor = 0l;
  return element;
}

void Schematic::paintEvents(RenderDevice *device)
{
  for(std::size_t i=0; i < m_postedPaintEvents.size(); i++)
    {
      paintEvent &event = m_postedPaintEvents[i];
      int opcode = event.op;
      int a0 = event.a0, a1 = event.a1;
      int x1 = mapToDeviceX(a0);
      int y1 = mapToDeviceY(a1);

      switch (opcode)
      {
        case PAINT_OP_RECT:
          device->rect(x1, y1, mapToDeviceDx(event.a2), mapToDeviceDy(event.a3));
          break;
        case PAINT_OP_DOT_RECT:
          {
            QPen pen = device->painter->pen();
            device->painter->setPen(Qt::DotLine);
            device->painter->drawRect(x1, y1, mapToDeviceDx(event.a2), mapToDeviceDy(event.a3));
            device->painter->setPen(pen);
            break;
          }
        case PAINT_OP_DOT_LINE:
          {
            QPen pen = device->painter->pen();
            device->painter->setPen(Qt::DotLine);
            device->painter->drawLine(x1, y1, mapToDeviceDx(event.a2), mapToDeviceDy(event.a3));
            device->painter->setPen(pen);
            break;
          }
      }
    }
}

/**
 * @brief Paint dot grid of the schematic.
 * @param painter
 * @param cx Device, start X position.
 * @param cy Device, start Y position.
 * @param width Device
 * @param height Device
 */
void Schematic::paintGrid(RenderDevice *device, int cx, int cy, int w, int h)
{
  device->painter->setPen(QPen(QColor(m_gridR, m_gridG, m_gridB), 1));

  int x1 = mapToLogicalX(cx);
  int y1 = mapToLogicalY(cy);

  togrid(&x1, &y1);
  if (x1<0)
    x1 -= m_gridX - 1;
  else
    x1 += m_gridX;
  x1 -= x1 % (m_gridX << 1);
  if (y1<0)
    y1 -= m_gridY - 1;
  else
    y1 += m_gridY;
  y1 -= y1 % (m_gridY << 1);

  float X = mapToDeviceX(x1);
  float Y = mapToDeviceY(y1);
  float Y0 = Y;
  x1 = X > 0.0 ? int(X + 0.5) : int(X - 0.5);
  y1 = Y > 0.0 ? int(Y + 0.5) : int(Y - 0.5);

  int dx = mapToDeviceDx(togridX(width() << 1));
  int dy = mapToDeviceDy(togridY(height() << 1));

  if (m_symbolMode)
    {
      int widthDev = mapToDeviceDx(width());
      int heightDev = mapToDeviceDy(height());
      device->painter->drawLine(0, dy, widthDev, dy); // origin cross
      device->painter->drawLine(dx,0, dx, heightDev);
    }
  else
    {
      device->painter->drawLine(-m_crosslineSize+dx, dy, m_crosslineSize+dx, dy); // small cross at origin
      device->painter->drawLine(dx,-m_crosslineSize+dy, dx, m_crosslineSize+dy);
    }

  int xEnd = x1 + w;
  int yEnd = y1 + h;
  float DX = mapToDeviceDx(m_gridX << 1); // every second grid a point
  float DY = mapToDeviceDy(m_gridY << 1);
  while(DX <= 8.0)
    DX *= 1.5;  // if too narrow, every third grid a point
  while (DY <= 8.0)
    DY *= 1.5;  // if too narrow, every third grid a point

  while(x1 < xEnd)
    {
      Y = Y0;
      y1 = Y > 0.0 ? int(Y + 0.5) : int(Y - 0.5);
      while (y1 < yEnd)
        {
          device->painter->drawPoint(x1, y1);    // paint grid
          Y += DY;
          y1 =  Y > 0.0 ? int(Y + 0.5) : int(Y - 0.5);
        }
      X += DX;
      x1 = X > 0.0 ? int(X + 0.5) : int(X - 0.5);
    }
}

/**
 * @Paint the document frame.
 */
void Schematic::paintFrame(RenderDevice *device)
{
  if (m_symbolMode)
      return;

  // dimensions:  X cm / 2.54 * 144
  int xall = width();
  int yall = height();

  device->painter->setPen(QPen(Qt::darkGray, 1));

  int d = device->lineSpacing + int(mapToDeviceDx(4.0));
  int x1_ = mapToDeviceDx(xall);
  int y1_ = mapToDeviceDy(yall);
  int x2_ = int(mapToDeviceDx(xall)) + 1;
  int y2_ = int(mapToDeviceDy(yall)) + 1;
  device->painter->drawRect(x1_, y1_, -x2_, -y2_);
  device->painter->drawRect(x1_-d, y1_-d, 2*d-x2_, 2*d-y2_);

  int z;
  int step = xall / ((xall+127) / 255);
  for(z=step; z <= xall-step; z+=step)
    {
      x2_ = mapToDeviceX(z);
      y2_ = mapToDeviceY(0);
      device->painter->drawLine(x2_, y2_, x2_, y2_+d);
      device->painter->drawLine(x2_, y1_-d, x2_, y1_);
    }

  char Letter[2] = {'1', 0};
  for(z=step/2+5; z < xall; z+=step)
    {
      mapToDevice(&x2_, &y2_, z, 3);
      device->painter->drawText(x2_, y2_, 0, 0, Qt::TextDontClip, Letter);
      mapToDevice(&x2_, &y2_, z, yall+3);
      device->painter->drawText(x2_, y2_-d, 0, 0, Qt::TextDontClip, Letter);
      Letter[0]+=1;
    }

  step = yall / ((yall+127) / 255);
  for(z=step; z <= yall-step; z+=step)
    {
      mapToDevice(&x2_, &y2_, 0, z);
      device->painter->drawLine(x2_, y2_, x2_+d, y2_);
      device->painter->drawLine(x1_-d, y2_, x1_, y2_);
    }

  Letter[0] = 'A';
  for(z=step/2+5; z<yall; z+=step)
    {
      mapToDevice(&x2_, &y2_, 5, z);
      device->painter->drawText(x2_, y2_, 0, 0, Qt::TextDontClip, Letter);
      mapToDevice(&x2_, &y2_, xall+5, z);
      device->painter->drawText(x2_-d, y2_, 0, 0, Qt::TextDontClip, Letter);
      Letter[0]+=1;
    }

  // draw text box with text
  mapToDevice(&x1_, &y1_, xall-340, yall-3);
  mapToDevice(&x2_, &y2_, xall-3,   yall-3);
  x1_ -= d;  x2_ -= d;
  y1_ -= d;  y2_ -= d;
  d = int(mapToDeviceDx(6.0));
  z = int(mapToDeviceDy(200.0));
  y1_ -= device->lineSpacing + d;
  device->painter->drawLine(x1_, y1_, x2_, y1_);
  device->painter->drawText(x1_+d, y1_+(d>>1), 0, 0, Qt::TextDontClip, m_frameText[2].c_str());
  device->painter->drawLine(x1_+z, y1_, x1_+z, y1_ + device->lineSpacing+d);
  device->painter->drawText(x1_+d+z, y1_+(d>>1), 0, 0, Qt::TextDontClip, m_frameText[3].c_str());
  y1_ -= device->lineSpacing + d;
  device->painter->drawLine(x1_, y1_, x2_, y1_);
  device->painter->drawText(x1_+d, y1_+(d>>1), 0, 0, Qt::TextDontClip, m_frameText[1].c_str());
  int numLines = 1; //FIXME! m_frameText[0].count('\n');
  y1_ -= (numLines+1) * device->lineSpacing + d;
  device->painter->drawRect(x2_, y2_, x1_-x2_-1, y1_-y2_-1);
  device->painter->drawText(x1_+d, y1_+(d>>1), 0, 0, Qt::TextDontClip, m_frameText[0].c_str());
}

/**
 * @brief Render graphics of anchor.
 * @param device
 * @param legend
 * @param cx1 Device X
 * @param cy1 Device Y
 * @param cx2 Device X2
 * @param cy2 Device Y2
 */
void Schematic::paintAnchor(RenderDevice *device, anchorLegend legend, int cx, int cy, int cx2/*=0*/, int cy2/*=0*/)
{
  QPen pen = device->painter->pen();
  switch (legend)
  {
    case ANCHOR_RECT_1:
      device->painter->setPen(QColor(0, 0, 128));
      device->painter->drawRect(cx-(m_anchorSize>>1), cy-(m_anchorSize>>1), m_anchorSize, m_anchorSize);
      break;
    case ANCHOR_ELLIPSE_1:
      device->painter->setPen(QColor(128, 0, 0));
      device->painter->drawEllipse(cx-(m_anchorSize>>1), cy-(m_anchorSize>>1), m_anchorSize, m_anchorSize);
      break;
    case ANCHOR_ELLIPSE_2:
      device->painter->setPen(QColor(0, 128, 0));
      device->painter->drawEllipse(cx-(m_anchorSize>>1), cy-(m_anchorSize>>1), m_anchorSize, m_anchorSize);
      break;
    case ANCHOR_LINE_1:
      device->painter->setPen(QPen(QColor(0, 128, 0), 1, Qt::DotLine));
      device->painter->drawLine(cx, cy, cx2, cy2);
      break;
    case ANCHOR_CROSS_1:
      device->painter->drawLine(-m_crosslineSize+cx, cy, m_crosslineSize+cx, cy);
      device->painter->drawLine( cx,-m_crosslineSize+cy, cx, m_crosslineSize+cy);
      break;
  }
  device->painter->setPen(pen);
}

void Schematic::appendAnchor(const anchor &anchor)
{
  m_anchors.push_back(anchor);
}

/**
 * @brief Render schematic to painter device.
 * @param device
 * @param cx
 * @param cy
 * @param w
 * @param h
 * @param preview Indicate whether to render preview
 * @param previewThresholdX Height threshold determines whether to show each element in preview.
 * @param previewThresholdY Width threshold
 * @param previewW
 * @param previewH
 */
void Schematic::render(RenderDevice *device, int cx, int cy, int w, int h, bool preview,
                       int previewThresholdX/*=-1*/, int previewThresholdY/*=-1*/,
                       int previewW/*=-1*/, int previewH/*=-1*/)
{
  int xd = mapToDeviceX(0);
  int yd = mapToDeviceY(0);
  int widthd = mapToDeviceDx(width());
  int heightd = mapToDeviceDy(height());
  device->painter->fillRect(xd, yd, widthd, heightd, QBrush(QColor(m_backgroundR, m_backgroundG, m_backgroundB)));

  if (!preview)
    {
      if (m_showGrid)
        { paintGrid(device, xd, yd, widthd, heightd); }
      if (m_showFrame)
        { paintFrame(device); }

      m_anchors.clear();

      for(std::list<Element *>::iterator it = m_elements.begin(); it != m_elements.end(); it++)
        { Element *element = *it;
          element->render(device);

          if (element->selected())
            {
              int x1, y1, x2, y2;
              element->bounding(&x1, &y1, &x2, &y2);
              x1 = mapToDeviceX(x1); x2 = mapToDeviceX(x2);
              y1 = mapToDeviceY(y1); y2 = mapToDeviceY(y2);
              int w = std::abs(x2-x1)+1;
              int h = std::abs(y2-y1)+1;

              // Draw selection rectangle
              if (w > 0 && h > 0)
                {
                  QPen pen = device->painter->pen();
                  device->painter->setPen(Qt::DotLine);
                  device->painter->drawRect(x1, y1, w, h);
                  device->painter->setPen(pen);
                }

              // Draw anchor points for modifiable element
              if (element->isModifiable())
                {
                  element->renderAnchors(device, this); // calling appendAnchor()
                }
            }
        }

      paintEvents(device);
    }
  else // preview
    {
      QPen pen = device->painter->pen();

      device->painter->setPen(QColor(0, 0, 128));
      int viewX1 = m_viewX1, viewY1 = m_viewY1 + 50;
      int viewX2 = m_viewX2, viewY2 = m_viewY2 - 50;

      float scaleX2 = float(previewW) / (m_viewX2-m_viewX1);
      float scaleY2 = float(previewH) / (m_viewY2-m_viewY1);
      int vpW = int((viewX2 - viewX1) * scaleX2);
      int vpH = int((viewY2 - viewY1) * scaleY2);

      float scaleX = float(vpW) / (m_viewX2-m_viewX1);
      float scaleY = float(vpH) / (m_viewY2-m_viewY1);

      // Draw schematic bounding
      device->painter->drawRect(int(viewX1 * scaleX2), int(viewY1 * scaleY2), vpW, vpH);

      // Draw element bounding
      device->painter->setPen(QColor(0, 0, 0));

      for(std::list<Element *>::const_iterator i=m_elements.begin(); i!=m_elements.end(); i++)
        {
          Element *element = *i;

          int x1, y1, x2, y2;
          element->bounding(&x1, &y1, &x2, &y2);
          x1 = float(x1 + viewX1) * scaleX, y1 = float(y1 + viewY1) * scaleY;
          x2 = float(x2 + viewX1) * scaleX, y2 = float(y2 + viewY1) * scaleY;
          int w = std::abs(x2-x1)+1;
          int h = std::abs(y2-y1)+1;

          if (w > previewThresholdX && h > previewThresholdY)
            {
              element->renderBounding(device, viewX1, viewY1, scaleX, scaleY);
            }
        }

      device->painter->setPen(pen);
    }
}

} // namespace
