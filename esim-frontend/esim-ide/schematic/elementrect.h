#ifndef ELEMENT_RECT_H_
#define ELEMENT_RECT_H_

#include "element.h"
#include "styletemplate.h"

namespace schematic
{
class ElementRect : public Element
{
public:
  ElementRect(int x1=0, int y1=0, int x2=0, int y2=0);

  ESD_OVERRIDE const char *classname() const;

  ESD_OVERRIDE int locationType() const;

  ESD_OVERRIDE void locate(int opcode, int paramSize, ...);

  ESD_OVERRIDE void render(RenderDevice *device);

  ESD_OVERRIDE void bounding(int *x1, int *y1, int *x2, int *y2) const;

  ESD_OVERRIDE int serialize(persistence::PersistenceDom *dom);
  ESD_OVERRIDE int deserialize(persistence::PersistenceDom *dom);

  ESD_OVERRIDE void config();

public:
  inline void setFillStyle(const FillStyle &fillstyle) { m_fillStyle = fillstyle; }

protected:
  int m_x1;
  int m_y1;
  int m_x2;
  int m_y2;
  FillStyle m_fillStyle;
};
} // namespace schematic


#endif // ELEMENT_RECT_H_