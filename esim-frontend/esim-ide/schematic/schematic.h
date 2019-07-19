/* -*-c++-*- */
#ifndef SCHEMATICWIDGET_H
#define SCHEMATICWIDGET_H

#define N_STANDARD_SIZES 8
#define N_FRAME_TEXTS 4

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "typedefs.h"

class QUndoStack;

namespace schematic {

enum PaintEventOpcode
{
  PAINT_OP_RECT = 0,
  PAINT_OP_DOT_RECT,
  PAINT_OP_DOT_LINE
};


class RenderDevice;
class Element;

class Schematic {
public:
  Schematic(bool symbolMode);
  ~Schematic();

  void render(RenderDevice *device, int cx, int cy, int width, int height, bool preview,
              int previewThresholdX=-1, int previewThresholdY=-1,
              float previewScaleX=0, float previewScaleY=0);

  bool getStandardSize(int type, int *width, int *height);
  void setSizeRect(int x1, int y1, int x2, int y2);
  void getSizeRect(int *x1, int *y1, int *x2, int *y2);

  inline void setShowGrid(bool grid)
    {
      m_showGrid = grid;
    }
  inline void setShowFrame(bool frame)
    {
      m_showFrame = frame;
    }

  /*inline*/ int togridX(int x) const;
  /*inline*/ int togridY(int y) const;
  /*inline*/ void togrid(int *x, int *y) const;

  /*inline*/ int mapToDeviceX(int x) const;
  /*inline*/ int mapToDeviceY(int y) const;
  /*inline*/ int mapToDeviceDx(int w) const;
  /*inline*/ int mapToDeviceDy(int h) const;
  /*inline*/ void mapToDevice(int *x, int *y, int cx, int cy) const;
  /*inline*/ int mapToLogicalX(int x) const;
  /*inline*/ int mapToLogicalY(int y) const;
  /*inline*/ int mapToLogicalDx(int w) const;
  /*inline*/ int mapToLogicalDy(int h) const;
  /*inline*/ void mapToLogical(int *x, int *y, int cx, int cy) const;

  /*inline*/ int width() const;
  /*inline*/ int height() const;
  inline float getScale() const { return m_scale; }
  inline float getFontScale() const { return m_fontScale; }
  inline void setScale(float scale) { m_scale = scale; }

  Element *appendElement(const char *classname);
  void getElememntsByPos(int x, int y, std::vector<Element *> *out);
  void removeElement(Element *target);
  void attachElement(Element *element);
  void detachElement(Element *element);

  void clear();
  int open(std::istream & stream);
  int save(std::ostream & stream);
  int exportElements(std::ostream & stream, const std::vector<Element *> &elements);
  int importElements(std::istream & stream, const std::vector<Element *> &elements);

  void postPaintEvent(int op, int a0, int a1, int a2, int a3, int a4=0, int a5=0);
  void clearPaintEvent();

  void selectElements(int x1, int y1, int x2, int y2);
  void clearSelectedElements();
  void getSelectedElements(std::vector<Element *> *out);

  Element *anchorPress(int x, int y);
  Element *anchorMove(int x, int y);
  Element *anchorRelease(int x, int y);

  void test();

protected:
  void paintEvents(RenderDevice *device);
  void paintGrid(RenderDevice *device, int cx, int cy, int width, int height);
  void paintFrame(RenderDevice *device);

  class anchor
  {
  public:
    anchor(Element *element, int type, int point, int x, int y) :
      element(element),
      type(type),
      point(point),
      x(x), y(y)
    {}
  public:
    Element *element;
    int type, point;
    int x, y;
  };

  enum anchorLegend
  {
    ANCHOR_RECT_1,
    ANCHOR_ELLIPSE_1,
    ANCHOR_ELLIPSE_2,
    ANCHOR_LINE_1,
    ANCHOR_CROSS_1
  };

  void paintAnchor(RenderDevice *device, anchorLegend legend, int cx, int cy, int cx2=0, int cy2=0);

  void appendAnchor(const anchor &anchor);

private:
  int openCallback(const char *classname, persistence::PersistenceDom *dom, void *opaque);
  int importCallback(const char *classname, persistence::PersistenceDom *dom, void *opaque);

private:
  struct paintEvent
  {
    int op, a0, a1, a2, a3;
    int a4, a5;
  };

  float m_scale;
  float m_fontScale;
  int m_anchorSize;
  int m_crosslineSize;
  bool m_showGrid;
  int m_gridX, m_gridY;
  unsigned char m_gridR, m_gridG, m_gridB;
  unsigned char m_backgroundR, m_backgroundG, m_backgroundB;
  int m_viewX1, m_viewY1;
  int m_viewX2, m_viewY2;
  bool m_symbolMode;
  int m_sizeType;
  int m_anchor_dR;

  bool m_showFrame;
  std::string m_frameText[N_FRAME_TEXTS];

  static int standardSizes[N_STANDARD_SIZES][2];

  std::list<Element *> m_elements;
  std::vector<paintEvent> m_postedPaintEvents;
  std::vector<anchor> m_anchors;
  const anchor *m_currentAnchor;
  int m_amx1, m_amy1;
  int m_amx2, m_amy2;
  persistence::Persistence *m_persistence;

  friend class Element;
};

} // namespace

#endif // SCHEMATICWIDGET_H

