#ifndef ELEMENT_CHORD_H_
#define ELEMENT_CHORD_H_

#include "elementarc.h"

namespace schematic
{

class ElementChord : public ElementArc
{
public:
  ElementChord(int x1=0, int y1=0, int radius=0, int startAngle=0, int spanAngle=0);

  ESD_OVERRIDE const char *classname() const;

  ESD_OVERRIDE void render(RenderDevice *device);

  ESD_OVERRIDE void config();
};

} // namespace schematic

#endif // ELEMENT_CHORD_H_
