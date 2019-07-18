#ifndef ELEMENT_ORIGIN_H_
#define ELEMENT_ORIGIN_H_

#include "element.h"
#include "styletemplate.h"

namespace schematic
{
class ElementOrigin : public Element
{
public:
  ElementOrigin(int x1=0, int y1=0);

  ESD_OVERRIDE const char *classname() const;

  ESD_OVERRIDE int locationType() const;

  ESD_OVERRIDE void locate(int opcode, int paramSize, ...);

  ESD_OVERRIDE void render(RenderDevice *device);

  ESD_OVERRIDE void bounding(int *x1, int *y1, int *x2, int *y2) const;

  ESD_OVERRIDE int serialize(persistence::PersistenceDom *dom);
  ESD_OVERRIDE int deserialize(persistence::PersistenceDom *dom);

protected:
  int m_x1;
  int m_y1;
};
} // namespace schematic


#endif // ELEMENT_ORIGIN_H_
