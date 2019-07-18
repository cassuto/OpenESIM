#ifndef ELEMENT_PIN_H_
#define ELEMENT_PIN_H_

#include "element.h"
#include "styletemplate.h"
#include "ieeetypes.h"

namespace schematic
{

class ElementText;

class ElementPin : public Element
{
public:
  ElementPin(int x1=0, int y1=0);

  ESD_OVERRIDE const char *classname() const;

  ESD_OVERRIDE int locationType() const;

  ESD_OVERRIDE void locate(int opcode, int paramSize, ...);

  ESD_OVERRIDE void render(RenderDevice *device);

  ESD_OVERRIDE void bounding(int *x1, int *y1, int *x2, int *y2) const;

  ESD_OVERRIDE int serialize(persistence::PersistenceDom *dom);
  ESD_OVERRIDE int deserialize(persistence::PersistenceDom *dom);

private:
  void relayout();

protected:
  int m_x1;
  int m_y1;
  int m_length;
  int m_symbolSpacing;
  int m_numSpacing;
  std::string m_symbol;
  std::string m_num;
  int m_angle;
  util::ElectricalType m_elecType;
  util::PinType m_pinType;
  int m_lineX1;
  int m_lineY1;
  int m_lineX2;
  int m_lineY2;
  LineStyle m_lineStyle;
  ElementText *m_pinSymbolText;
  ElementText *m_pinNumText;
};

} // namespace schematic

#endif // ELEMENT_PIN_H_
