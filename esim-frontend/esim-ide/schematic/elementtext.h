#ifndef ELEMENT_TEXT_H_
#define ELEMENT_TEXT_H_

#include "element.h"
#include "styletemplate.h"
#include <string>

namespace schematic
{
class ElementText : public Element
{
public:
  ElementText(int x1=0, int y1=0, const std::string &text="TEXT");

  ESD_OVERRIDE const char *classname() const;

  ESD_OVERRIDE int locationType() const;

  ESD_OVERRIDE void locate(int opcode, int paramSize, ...);

  ESD_OVERRIDE void render(RenderDevice *device);

  ESD_OVERRIDE void bounding(int *x1, int *y1, int *x2, int *y2) const;

  ESD_OVERRIDE int serialize(persistence::PersistenceDom *dom);
  ESD_OVERRIDE int deserialize(persistence::PersistenceDom *dom);

  ESD_OVERRIDE void config();

public:
  void setText(const std::string &text);
  void setTextStyle(const TextStyle &style);

  inline int getTextWidth() const { return m_textWidth; }
  inline int getTextHeight() const { return m_textHeight; }

private:
  void update();

protected:
  int m_x1;
  int m_y1;
  std::string m_text;
  int m_angle;
  int m_textWidth;
  int m_textHeight;
  TextStyle m_textStyle;
};
} // namespace schematic


#endif // ELEMENT_TEXT_H_
