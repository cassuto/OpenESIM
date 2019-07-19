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

#include "schematicview.h"
#include "schematic.h"
#include "renderdevice.h"
#include "element.h"
#include "undocommands.h"

#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QMenu>
#include <QCursor>
#include <cmath>
#include <cstdio>

namespace schematic {

////////////////////////////////////////////////////////////////////////////////
// SchematicView Class                                                        //
////////////////////////////////////////////////////////////////////////////////

SchematicView::SchematicView(bool symbolMode, QWidget *parent) :
  QScrollArea(parent),
  m_document(new Schematic(symbolMode))
{
  m_viewport = new SchematicViewport(m_document, this);
  setWidget(m_viewport);
}

SchematicView::~SchematicView()
{
  delete m_document;
  delete m_viewport;
}

void SchematicView::test()
{ m_document->test(); }

void SchematicView::keyPressEvent(QKeyEvent *event)
{
  m_viewport->keyPressEvent(event);
}



////////////////////////////////////////////////////////////////////////////////
// SchematicViewport Class                                                    //
////////////////////////////////////////////////////////////////////////////////

SchematicViewport::SchematicViewport(Schematic *document, QWidget *parent) :
  QOpenGLWidget(parent),
  m_initDistance(100),
  m_mx1(0), m_my1(0),
  m_mx2(0), m_my2(0),
  m_tx1(0), m_ty1(0),
  m_document(document),
  m_mode(EDIT_MODE_SELECT),
  m_mousePressAction(0l),
  m_mouseMoveAction(0l),
  m_mouseReleaseAction(0l),
  m_modified(false)
{
  m_undoStack = new QUndoStack(this);
  m_assureSelectionMenu = new QMenu(this);
  connect(m_assureSelectionMenu, SIGNAL(triggered(QAction *)), this, SLOT(slotAssureSelectionMenuTriggered(QAction *)));
  m_assureSelectId = -1;

  setSize(0);
  switchMode(m_mode);
}

/**
 * @brief Set size of schematic.
 * @param w Width, px, logical CS
 * @paeam h Height, px, logical CS
 */
void SchematicViewport::setSize(int w, int h)
{
  m_document->setSizeRect(0, 0, w, h);
  adjSize(w, h);
  changed();
}

/**
 * @brief Set size of schematic.
 * @param type Index of standard size type.
 */
void SchematicViewport::setSize(int type)
{
  int width, height;
  if (m_document->getStandardSize(type, &width, &height))
    {
      setSize(width, height);
    }
}

/**
 * @brief When schematic document is modified, call this.
 * @param update Whether to redraw view.
 */
void SchematicViewport::changed(bool doupdate/*=false*/)
{
  if (doupdate) { update(); }
  emit schematicChanged();
}

/**
 * @brief Scale view
 * @param factor 1 means 1:1
 */
void SchematicViewport::scale(float factor)
{
  m_document->setScale(factor);
  adjSize();
}

void SchematicViewport::adjSize(int w /*=-1*/, int h /*=-1*/)
{
  if (w == -1 || h == -1)
    {
      int x,y;
      m_document->getSizeRect(&x, &y, &w, &h);
    }
  resize(m_document->mapToDeviceDx(w), m_document->mapToDeviceDy(h));
}

/**
 * Unified Modification Management Mechanism:
 * Patterns of schematic modification and how to process:
 *
 *      1. Edit: i.e. Changing attributes of element.
 *              beginEdit() ---> (accepted) ---> setModified(true) ---> endEdit()
 *                               (rejected) -------------------------------^
 *
 *      2. Insert: Attaching an element to document.
 *              (accepted) ---> endInsert()
 *
 *      3. Delete: Detaching an element from document.
 *              (accepted) ---> endDelete()
 *
 * New pattern goes here...
 */

/**x
 * @brief Called before beginning element editing.
 */
void SchematicViewport::beginEdit(const std::vector<Element *> &targets)
{
  m_undoPrevStream.str("");
  m_document->exportElements(m_undoPrevStream, targets);
  setModified(false);
}
void SchematicViewport::beginEdit(Element *element)
{
  std::vector<Element *> targets;
  targets.push_back(element);
  beginEdit(targets);
}
void SchematicViewport::setModified(bool val)
{
  m_modified = val;
}
/**
 * @brief Called after ended element editing, which creates a restore point for undo.
 *      Note that this only makes sense when self.modified is True
 */
void SchematicViewport::endEdit(const QString &command, const std::vector<Element *> &targets)
{
  if (m_modified)
    {
      m_undoNextStream.str("");
      m_document->exportElements(m_undoNextStream, targets);

      QUndoCommand *cmd = new SchematicUndoEditCommand(command, this, targets, m_undoPrevStream.str(), m_undoNextStream.str());
      m_undoStack->push(cmd);

      setModified(false);
      changed();
    }
}
void SchematicViewport::endEdit(const QString &command, Element *element)
{
  std::vector<Element *> targets;
  targets.push_back(element);
  endEdit(command, targets);
}

void SchematicViewport::endInsert(Element *target)
{
  std::vector<Element *> targets;
  targets.push_back(target);

  m_undoPrevStream.str("");
  m_document->exportElements(m_undoPrevStream, targets);

  m_undoStack->push(new SchematicUndoInsertCommand(QString("insert element \"%1\"").arg(target->classname()),
                                                   this, target,
                                                   m_undoPrevStream.str()));
  changed();
}

void SchematicViewport::endDelete(Element *target)
{
  std::vector<Element *> targets;
  targets.push_back(target);

  m_undoPrevStream.str("");
  m_document->exportElements(m_undoPrevStream, targets);

  m_undoStack->push(new SchematicUndoDeleteCommand(QString("delete element \"%1\"").arg(target->classname()),
                                                   this, target,
                                                   m_undoPrevStream.str()));
  changed();
}

void SchematicViewport::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  RenderDevice device(m_document, &painter);
  QRect rect(event->rect());
  m_document->render(&device, rect.top(), rect.left(), rect.width(), rect.height(), /*preview*/false);
}

void SchematicViewport::mousePressEvent(QMouseEvent *event)
{
  int x = m_document->mapToLogicalX(event->pos().x());
  int y = m_document->mapToLogicalY(event->pos().y());

  if (Element *element = m_document->anchorPress(x, y))
    {
      this->beginEdit(element); // start editing of anchors.
    }
  else
    {
      if (m_mousePressAction)
        { (this->*m_mousePressAction)(event, x, y); } // dispatch handler
    }
}
void SchematicViewport::mouseDoubleClickEvent(QMouseEvent *event)
{
  int x = m_document->mapToLogicalX(event->pos().x());
  int y = m_document->mapToLogicalY(event->pos().y());

  if (m_mouseDbclickAction)
    { (this->*m_mouseDbclickAction)(event, x, y); } // dispatch handler
}
void SchematicViewport::mouseMoveEvent(QMouseEvent *event)
{
  int x = m_document->mapToLogicalX(event->pos().x());
  int y = m_document->mapToLogicalY(event->pos().y());

  if (m_document->anchorMove(x, y))
    {
      this->setModified(true);
      this->update();
    }
  else
    {
      if (m_mouseMoveAction)
        { (this->*m_mouseMoveAction)(event, x, y); } // dispatch handler
    }
}
void SchematicViewport::mouseReleaseEvent(QMouseEvent *event)
{
  int x = m_document->mapToLogicalX(event->pos().x());
  int y = m_document->mapToLogicalY(event->pos().y());

  if (Element *element = m_document->anchorRelease(x, y))
    {
      this->endEdit(tr("drag anchor"), element); // finished editing of anchor
    }
  else
    {
      if (m_mouseReleaseAction)
        { (this->*m_mouseReleaseAction)(event, x, y); } // dispatch handler
    }
}

void SchematicViewport::leaveEvent(QEvent *event)
{
  if (m_mouseLeaveAction)
    { (this->*m_mouseLeaveAction)(0l, 0, 0); } // dispatch handler
}

void SchematicViewport::keyPressEvent(QKeyEvent *event)
{
  if(m_keyPressAction)
    { (this->*m_keyPressAction)(event); } // dispatch handler
}

void SchematicViewport::switchMode(EditMode mode, const char *classname)
{
  setMousePressAction(0l);
  setMouseDbclickAction(0l);
  setMouseMoveAction(0l);
  setMouseReleaseAction(0l);
  setMouseLeaveAction(0l);
  setKeyPressAction(0l);

  /*
   * Deals with closing-up releasing of previous mode
   */
  if (mode != m_mode)
    {
      switch (m_mode)
      {
        case EDIT_MODE_SELECT:
          m_document->clearPaintEvent();
          m_document->clearSelectedElements();
          update();
          break;
        case EDIT_MODE_INSERT_ELEMENT:
          giveupElementInsertion();
          break;
        default:
          break;
      }
    }

  m_currentElements.clear();
  m_currentElement = 0l;

  /*
   * Switch state-machine of KM (Keyboard/Mouse) events
   */
  switch (mode)
  {
    case EDIT_MODE_SELECT:
      setMousePressAction(&SchematicViewport::mousePressSelect);
      setMouseDbclickAction(&SchematicViewport::mouseDbclickSelect);
      setKeyPressAction(&SchematicViewport::keyPressSelect);
      setCursor(Qt::ArrowCursor);
      break;
    case EDIT_MODE_INSERT_ELEMENT:
      if (classname)
        { m_currentClassname = classname; }
      setMousePressAction(&SchematicViewport::mousePressInsertElement);
      setMouseLeaveAction(&SchematicViewport::mouseLeaveInsertElement);
      setKeyPressAction(&SchematicViewport::keyPressInsertElement);
      setCursor(QCursor(QPixmap(":/assets/greenpen.xpm")));
      break;
  }

  if (mode != m_mode)
    {
      m_mode = mode;
      emit modeChanged(mode, m_currentClassname.c_str()/* not 'classname' pointer */);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Element Selection actions                                                  //
////////////////////////////////////////////////////////////////////////////////

void SchematicViewport::mousePressSelect(QMouseEvent *event, int x, int y)
{
  m_mx1 = x;
  m_my1 = y;

  std::vector<Element *> curSelectedElements;
  m_currentElements.clear();

  m_document->getElememntsByPos(m_mx1, m_my1, &curSelectedElements);
  m_document->getSelectedElements(&m_currentElements);

  Element *pressElement = 0l;

  if (curSelectedElements.size() == 1)
    { pressElement = curSelectedElements[0]; }
  else if (curSelectedElements.size() > 1)
    {
      if (m_currentElements.size() == 0) // ask for selection assurance
        {
          /*
           * Construct menu for selection assurance
           */
          m_assureSelectionMenu->clear();
          QAction *action = m_assureSelectionMenu->addAction(tr("Clear choice"));
          action->setEnabled(false);
          m_assureSelectionMenu->addSeparator();

          for(std::size_t i=0; i < curSelectedElements.size(); i++)
            {
              Element *element = curSelectedElements[i];
              std::string title;
              element->getLocationMsg(&title);
              title.append(": ");
              title.append(element->classname());
              action = m_assureSelectionMenu->addAction(title.c_str());
              action->setData(QVariant::fromValue<int>(i));
            }

          m_assureSelectId = -1;
          m_assureSelectionMenu->exec(event->globalPos()); // do modal
          if (m_assureSelectId != -1)
            {
              Element *element = curSelectedElements[m_assureSelectId];

              int cx1, cy1;
              element->origin(&cx1, &cy1);
              QCursor::setPos(mapToGlobal(QPoint(m_document->mapToDeviceX(cx1), m_document->mapToDeviceY(cy1)))); // goto view

              pressElement = element;
            }
        }
      else
        {
          pressElement = m_currentElements[0];
        }
    }

  /*
   * Status switching of editor in selection mode.
   */
  bool freeSelection = false;

  if (pressElement)
    {
      if (m_currentElements.size() == 0)
        {
          pressElement->setSelected(true); // do selection for assurance
          update();
        }
      else
        {
          freeSelection = true;
          for(std::size_t i=0; i < curSelectedElements.size(); i++)
            {
              Element *element = curSelectedElements[i];

              bool contains = false;
              for(std::size_t j=0; j < m_currentElements.size(); j++ )
                {
                  if (m_currentElements[j] == element)
                    {contains=true; break;}
                }
              if (contains)
                {
                  /*
                   * there were an(some) element(s) selected, in consequence we should
                   * cancel the selection and switch to moving mode.
                   */
                  freeSelection = false;
                  break;
                }
            }


          /* --- Enter to element moving mode --- */
          if (!freeSelection)
            {
              this->beginEdit(m_currentElements);
              m_mx1 = m_document->togridX(x);
              m_my1 = m_document->togridY(y);
              setMouseMoveAction(&SchematicViewport::mouseMoveElementMoving);
              setMouseReleaseAction(&SchematicViewport::mouseReleaseElementMoving);
            }
        }
    }
  else
    {
      freeSelection = true;
    }

  /* --- Enter to free selection mode --- */
  if (freeSelection) // open a rectangle
    {
      m_mx2 = 0;
      m_my2 = 0;
      setMouseMoveAction(&SchematicViewport::mouseMoveSelect);
      setMouseReleaseAction(&SchematicViewport::mouseReleaseSelect);

      m_document->clearSelectedElements();
      update();
    }
}

void SchematicViewport::mouseMoveSelect(QMouseEvent *event, int x, int y)
{
  m_mx2 = x - m_mx1;
  m_my2 = y - m_my1;

  m_document->clearPaintEvent();
  m_document->postPaintEvent(PAINT_OP_DOT_RECT, m_mx1, m_my1, m_mx2, m_my2);
  update();
}

void SchematicViewport::mouseReleaseSelect(QMouseEvent *event, int x, int y)
{
  // Selects all elements within the rectangle
  m_document->selectElements(m_mx1, m_my1, m_mx1 + m_mx2, m_my1 + m_my2);
  m_document->clearPaintEvent();
  update();

  switchMode(EDIT_MODE_SELECT);
}

void SchematicViewport::keyPressSelect(QKeyEvent *event)
{
  switch(event->key())
  {
    case Qt::Key_Delete:
      m_document->getSelectedElements(&m_currentElements);

      for(std::size_t i=0; i<m_currentElements.size(); i++)
        {
          Element *element = m_currentElements[i];
          if (element->isModifiable())
            {
              this->endDelete(element);
            }
        }
      update();
      break;

    case Qt::Key_Return:
    case Qt::Key_Enter:
      m_document->getSelectedElements(&m_currentElements);

      if (m_currentElements.size() == 1 && m_currentElements[0]->isModifiable())
        {
          m_currentElements[0]->config();
          update();
        }
      break;
  }
}

void SchematicViewport::mouseDbclickSelect(QMouseEvent *event, int logicalX, int logicalY)
{
  if (event->button() == Qt::LeftButton)
    {
      m_document->getSelectedElements(&m_currentElements);

      if (m_currentElements.size() == 1 && m_currentElements[0]->isModifiable())
        {
          m_currentElements[0]->config();
          update();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Element moving actions                                                     //
////////////////////////////////////////////////////////////////////////////////

void SchematicViewport::mouseMoveElementMoving(QMouseEvent *event, int x, int y)
{
  int cx = m_document->togridX(x);
  int cy = m_document->togridY(y);
  int dX = cx - m_mx1;
  int dY = cy - m_my1;
  m_mx1 = cx;
  m_my1 = cy;

  for(std::size_t i=0; i < m_currentElements.size(); i++)
    { m_currentElements[i]->moveDelta(dX, dY); }

  setModified(true);
  update();
}

void SchematicViewport::mouseReleaseElementMoving(QMouseEvent *event, int x, int y)
{
  endEdit(tr("moving element"), m_currentElements);
  switchMode(EDIT_MODE_SELECT);
}

void SchematicViewport::slotAssureSelectionMenuTriggered(QAction *action)
{
  m_assureSelectId = action->data().toInt();
}

////////////////////////////////////////////////////////////////////////////////
// Element insertion                                                          //
////////////////////////////////////////////////////////////////////////////////

void SchematicViewport::mousePressInsertElement(QMouseEvent *event, int logicalX, int logicalY)
{
  m_currentElement = m_document->appendElement(m_currentClassname.c_str());

  if (m_currentElement && event->button() == Qt::LeftButton)
    {
      m_mx1 = m_mx2 = m_document->togridX(logicalX);
      m_my1 = m_my2 = m_document->togridY(logicalY);

      int type = m_currentElement->locationType();
      if (type & S0_POS_4P)
        {
          m_currentElement->locate(locateSetPos, 4*sizeof(int), m_mx1, m_my1, m_mx2, m_my2);
          setMouseMoveAction(&SchematicViewport::mouseMoveInsert_4P);
          setMousePressAction(&SchematicViewport::mousePress2Insert_4P);
          setMouseTracking(true);
          setCursor(QCursor(QPixmap(":/assets/whitepen.xpm")));
        }
      else if (type & S0_POS_1P)
        {
          m_currentElement->locate(locateSetPos, 2*sizeof(int), m_mx1, m_my1);
          setMouseMoveAction(&SchematicViewport::mouseMoveInsert_1P);
          setMousePressAction(&SchematicViewport::mousePress2Insert_1P);
          setMouseTracking(true);
          QPixmap pix(16,16);
          pix.fill(QColor(0,0,0,0));
          setCursor(QCursor(pix));
          update();
        }
      else if (type & S0_POS_1P1R)
        {
          m_currentElement->locate(locateSetPos, 3*sizeof(int), m_mx1, m_my1, 0);
          setMouseMoveAction(&SchematicViewport::mouseMoveInsert_1P1R);
          setMousePressAction(&SchematicViewport::mousePress2Insert_4P); // yes, the bounding of circle is rectangle
          setMouseTracking(true);
          setCursor(QCursor(QPixmap(":/assets/whitepen.xpm")));
        }
      else if (type & S0_POS_2P1D)
        {
          setMouseMoveAction(&SchematicViewport::mouseMoveInsert_2P1D);
          setMousePressAction(&SchematicViewport::mousePress2Insert_2P1D);
          setMouseTracking(true);
          setCursor(QCursor(QPixmap(":/assets/whitepen.xpm")));
        }
      else // never reach here
        {
          assert(0);
          m_document->removeElement(m_currentElement);
          m_currentElement = 0l;
        }
      setKeyPressAction(&SchematicViewport::keyPressGiveupElementInsertion);
    }
  else
    { switchMode(EDIT_MODE_SELECT); }
}

void SchematicViewport::finishElementInsertion(QMouseEvent *event)
{
  if (m_currentElement)
    {
      if (event->button() == Qt::LeftButton)
        {
          this->endInsert(m_currentElement);
          setMouseTracking(false);
          switchMode(EDIT_MODE_INSERT_ELEMENT);
        }
      else
        {
          giveupElementInsertion();
        }
      m_currentElement = 0l;
    }
}

void SchematicViewport::giveupElementInsertion()
{
  if (m_currentElement) // give up element insertion
    {
      // TODO determine whether to scroll painting
      m_document->clearPaintEvent();
      m_document->removeElement(m_currentElement);
      m_currentElement = 0l;

      switchMode(EDIT_MODE_INSERT_ELEMENT);
      update();
    }
}

void SchematicViewport::mouseLeaveInsertElement(QMouseEvent *event, int logicalX, int logicalY)
{
  giveupElementInsertion();
}

void SchematicViewport::keyPressInsertElement(QKeyEvent *event)
{
  switch(event->key())
  {
    case Qt::Key_Backspace:
    case Qt::Key_Escape:
      switchMode(EDIT_MODE_SELECT);
      break;
  }
}

void SchematicViewport::keyPressGiveupElementInsertion(QKeyEvent *event)
{
  switch(event->key())
  {
    case Qt::Key_Backspace:
    case Qt::Key_Escape:
      giveupElementInsertion();
      setKeyPressAction(&SchematicViewport::keyPressInsertElement);
      break;
  }
}

void SchematicViewport::mouseMoveInsert_1P1R(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement)
    {
      m_mx2 = m_document->togridX(logicalX);
      m_my2 = m_document->togridY(logicalY);
      int radus = int(std::sqrt((m_mx2-m_mx1)*(m_mx2-m_mx1) + (m_my2-m_my1)*(m_my2-m_my1)));

      m_currentElement->locate(locateSetPos, 3*sizeof(int), m_mx1, m_my1, radus);
      update();
    }
}

void SchematicViewport::mouseMoveInsert_2P1D(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement)
    {
      m_mx2 = m_document->togridX(logicalX);
      m_my2 = m_document->togridY(logicalY);

      m_currentElement->locate(locateSetPos, 5*sizeof(int), m_mx1, m_my1, m_mx2, m_my2, m_initDistance);
      int x1, y1, radius, a0, a1;
      m_currentElement->locate(locateGetArc, 5*sizeof(int*), &x1, &y1, &radius, &a0, &a1);

      m_document->clearPaintEvent();
      m_document->postPaintEvent(PAINT_OP_DOT_LINE, m_mx1, m_my1, m_mx2, m_my2);
      m_document->postPaintEvent(PAINT_OP_DOT_LINE, m_mx1, m_my1, x1, y1);
      m_document->postPaintEvent(PAINT_OP_DOT_LINE, m_mx2, m_my2, x1, y1);
      update();

      m_tx1 = x1; m_ty1 = y1;
    }
}

void SchematicViewport::mousePress2Insert_2P1D(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement && event->button() == Qt::LeftButton)
    {
      if (std::abs(m_mx1-m_mx2) > 1 || std::abs(m_my1-m_my2) > 1)
        {
          int x1, y1, radius, a0, a1;
          m_currentElement->locate(locateGetArc, 5*sizeof(int*), &x1, &y1, &radius, &a0, &a1);
          QCursor::setPos(mapToGlobal(QPoint(m_document->mapToDeviceX(x1), m_document->mapToDeviceY(y1)))); // goto origin

          setMouseMoveAction(&SchematicViewport::mouseMove2Insert_2P1D);
          setMousePressAction(&SchematicViewport::mousePress3Insert_2P1D);
        }
    }
}

void SchematicViewport::mouseMove2Insert_2P1D(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement)
    {
      int cx = m_document->togridX(logicalX);
      int cy = m_document->togridY(logicalY);

      float mod = std::sqrt((m_mx2-m_mx1)*(m_mx2-m_mx1) + (m_my2-m_my1)*(m_my2-m_my1));
      int distance = int(((cx-m_mx1)*(m_my2-m_my1)-(cy-m_my1)*(m_mx2-m_mx1)) / mod);
      if (distance >= 0)
        {
          m_currentElement->locate(locateSetPos, 5*sizeof(int), m_mx1, m_my1, m_mx2, m_my2, distance);
          int x1, y1, radius, a0, a1;
          m_currentElement->locate(locateGetArc, 5*sizeof(int*), &x1, &y1, &radius, &a0, &a1);

          m_document->clearPaintEvent();
          m_document->postPaintEvent(PAINT_OP_DOT_LINE, m_mx1, m_my1, m_mx2, m_my2);
          m_document->postPaintEvent(PAINT_OP_DOT_LINE, m_mx1, m_my1, x1, y1);
          m_document->postPaintEvent(PAINT_OP_DOT_LINE, m_mx2, m_my2, x1, y1);
          update();
        }
    }
}

void SchematicViewport::mousePress3Insert_2P1D(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement && event->button() == Qt::LeftButton)
    {
      m_document->clearPaintEvent();
      update();
      if (std::abs(m_mx1-m_mx2) > 1 || std::abs(m_my1-m_my2) > 1)
        {
          finishElementInsertion(event);
        }
    }
}

void SchematicViewport::mouseMoveInsert_4P(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement)
    {
      m_mx2 = m_document->togridX(logicalX);
      m_my2 = m_document->togridY(logicalY);
      m_currentElement->locate(locateSetPos, 4*sizeof(int), m_mx1, m_my1, m_mx2, m_my2);
      update();
    }
}

void SchematicViewport::mousePress2Insert_4P(QMouseEvent *event, int logicalX, int logicalY)
{
  if (std::abs(m_mx1-m_mx2) > 1 && std::abs(m_my1 - m_my2) > 1 && event->button() == Qt::LeftButton)
    {
      finishElementInsertion(event);
    }
}

void SchematicViewport::mouseMoveInsert_1P(QMouseEvent *event, int logicalX, int logicalY)
{
  if (m_currentElement)
    {
      m_mx2 = m_document->togridX(logicalX);
      m_my2 = m_document->togridY(logicalY);
      m_currentElement->locate(locateSetPos, 2*sizeof(int), m_mx2, m_my2);
      update();
    }
}

void SchematicViewport::mousePress2Insert_1P(QMouseEvent *event, int logicalX, int logicalY)
{
  if (event->button() == Qt::LeftButton)
    {
      finishElementInsertion(event);
    }
}

} // namespace
