#ifndef ELEMENT_ARC_
#define ELEMENT_ARC_

#include "element.h"
#include "styletemplate.h"

namespace schematic
{
class ElementArc : public Element
{
public:
  ElementArc(int x1=0, int y1=0, int radius=0, int startAngle=0, int spanAngle=0);

  ESD_OVERRIDE const char *classname() const;

  ESD_OVERRIDE int locationType() const;

  ESD_OVERRIDE void locate(int opcode, int paramSize, ...);

  ESD_OVERRIDE void render(RenderDevice *device);

  ESD_OVERRIDE void bounding(int *x1, int *y1, int *x2, int *y2) const;

  ESD_OVERRIDE int serialize(persistence::PersistenceDom *dom);
  ESD_OVERRIDE int deserialize(persistence::PersistenceDom *dom);

  ESD_OVERRIDE void config();

protected:
  void update();
  void resolvePos(int stringX1, int stringY1, int stringX2, int stringY2, int distance);
  int standardizeAngle(int angle);

protected:
  int m_x1;
  int m_y1;
  int m_sX1, m_sY1, m_sX2, m_sY2;
  int m_distance;
  int m_radius;
  int m_startAngle;
  int m_spanAngle;
  int m_boundX1, m_boundY1;
  int m_boundX2, m_boundY2;
  LineStyle m_lineStyle;
};
} // namespace schematic

#endif // ELEMENT_ARC_
