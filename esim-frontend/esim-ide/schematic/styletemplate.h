#ifndef STYLETEMPLAYE_H_
#define STYLETEMPLAYE_H_

#include <QtGui/QColor>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QFont>
#include <string>
#include <map>

#include "typedefs.h"

namespace schematic
{

enum templatedStyle
{
  LINE_NONE,
  LINE_SOLID,
  LINE_DASH,
  LINE_DOT,
  LINE_DASH_DOT,
  LINE_DASH_DOTDOT,
  LINEMAX,

  BRUSH_NONE,
  BRUSH_SOLID,
  BRUSH_HOR,
  BRUSH_VER,
  BRUSH_CROSS,
  BRUSHMAX
};

class templatedColor
{
public:
  templatedColor() : R(0), G(0), B(0) {}
  templatedColor(unsigned char R, unsigned char G, unsigned char B) : R(R), G(G), B(B) {}
  templatedColor(const QColor &color) : R(color.red()), G(color.blue()), B(color.green()) {}
  inline bool operator==(const templatedColor &src) const
    {
      return R == src.R && G == src.G && B == src.B;
    }
public:
  unsigned char R, G, B;
};

class StyleBase
{
public:
  StyleBase(const std::string &symbol) :
     m_symbol(symbol)
  {}
  inline const std::string & symbol() const { return m_symbol; }
protected:
  std::string m_symbol;
};

/**
 * Line style template
 */
class LineStyle : public StyleBase
{
public:
  LineStyle(const std::string &symbol, const QPen &penIns);
  LineStyle(const std::string &symbol="", const templatedColor & color=templatedColor(), float width=0, templatedStyle line=LINE_NONE);

  inline const QPen &pen() const { return m_penIns; }

  int serialize(persistence::PersistenceDom *dom);
  int deserialize(persistence::PersistenceDom *dom);

public:
  inline const templatedColor &color() const
    {
      return m_color;
    }
  inline void setColor(const templatedColor &color)
    {
      m_color = color;
      set();
    }
  inline float width() const
    {
      return m_width;
    }
  inline void setWidth(float width)
    {
      m_width = width;
      set();
    }
  inline templatedStyle line() const
    {
      return m_line;
    }
  inline void setLine(templatedStyle style)
    {
      m_line = style;
      set();
    }

public:
  inline bool operator==(const LineStyle &src) const;

protected:
  void set();
protected:
  templatedColor m_color;
  float m_width;
  templatedStyle m_line;
  QPen m_penIns;
};

/**
 * Fill style template
 */
class FillStyle : public LineStyle
{
public:
  FillStyle(const std::string &symbol, const QPen &penIns, const QBrush &brushIns);
  FillStyle(const std::string &symbol="",
            const templatedColor &color=templatedColor(),
            const templatedColor &bkcolor=templatedColor(),
            float width=0,
            templatedStyle line=LINE_NONE,
            templatedStyle brush=BRUSH_NONE);

  inline const QBrush &brush() const { return m_brushIns; }

  int serialize(persistence::PersistenceDom *dom);
  int deserialize(persistence::PersistenceDom *dom);

public:
  inline const templatedColor &bkcolor() const
    {
      return m_bkcolor;
    }
  inline void setBkcolor(const templatedColor &color)
    {
      m_bkcolor = color;
      set();
    }
  inline templatedStyle brushS() const
    {
      return m_brush;
    }
  inline void setBrushS(templatedStyle style)
    {
      m_brush = style;
      set();
    }
public:
  inline bool operator==(const FillStyle &src) const;

protected:
  void set();
protected:
  templatedColor m_bkcolor;
  templatedStyle m_brush;
  QBrush m_brushIns;
};

/**
 * Text style template
 */
class TextStyle : public StyleBase
{
public:
  TextStyle(const std::string &symbol, const QPen &penIns, const QFont &fontIns);
  TextStyle(const std::string &symbol="",
            const templatedColor & color=templatedColor(),
            const std::string &fontName="",
            int sizePt=0,
            bool bold=false,
            bool italic=false,
            bool underline=false);

  inline const QPen &pen() const { return m_penIns; }
  inline const QFont &font() const { return m_fontIns; }

  int serialize(persistence::PersistenceDom *dom);
  int deserialize(persistence::PersistenceDom *dom);

public:
  inline const templatedColor &color() const
    {
      return m_color;
    }
  inline void setColor(const templatedColor &color)
    {
      m_color = color;
      set();
    }
  inline const std::string &fontName() const
    {
      return m_fontName;
    }
  inline void setFontName(const std::string &font)
    {
      m_fontName = font;
      set();
    }
  inline int sizePt() const
    {
      return m_sizePt;
    }
  inline void setSizePt(int sizePt)
    {
      m_sizePt = sizePt;
      set();
    }
  inline bool bold() const
    {
      return m_bold;
    }
  inline void setBold(bool bold)
    {
      m_bold = bold;
      set();
    }
  inline bool italic() const
    {
      return m_italic;
    }
  inline void setItalic(bool italic)
    {
      m_italic = italic;
      set();
    }
  inline bool underline() const
    {
      return m_underline;
    }
  inline void setUnderline(bool underline)
    {
      m_underline = underline;
      set();
    }

public:
  inline bool operator==(const TextStyle &src) const;

protected:
  void set();
protected:
  templatedColor m_color;
  std::string m_fontName;
  int m_sizePt;
  bool m_bold;
  bool m_italic;
  bool m_underline;
  QPen m_penIns;
  QFont m_fontIns;
};

class StyleTemplates
{
public:
  StyleTemplates();
  ~StyleTemplates();

  static inline StyleTemplates *getInstance() {
    if (!m_instance)
      m_instance = new StyleTemplates();
    return m_instance;
  }

  static const char *getTemplatedStyleName(templatedStyle style);

public:
  /**
   * Get pointer of style entry, guaranteeing a non-null returning.
   * i.e. if there is no style matched the given symbol, return default style.
   */
  const LineStyle *getLineStyleRef(const std::string &symbol);
  const FillStyle *getFillStyleRef(const std::string &symbol);
  const TextStyle *getTextStyleRef(const std::string &symbol);

  LineStyle copyLineStyle(const std::string &symbol);
  FillStyle copyFillStyle(const std::string &symbol);
  TextStyle copyTextStyle(const std::string &symbol);

  inline const std::map<std::string, LineStyle *> &lineStyles() const
    {
      return m_lineStyles;
    }
  inline const std::map<std::string, FillStyle *> &fillStyles() const
    {
      return m_fillStyles;
    }
  inline const std::map<std::string, TextStyle *> &textStyles() const
    {
      return m_textStyles;
    }

private:
  static StyleTemplates *m_instance;
  std::map<std::string, LineStyle *> m_lineStyles;
  std::map<std::string, FillStyle *> m_fillStyles;
  std::map<std::string, TextStyle *> m_textStyles;
};

} // namespace schematic

#endif // STYLETEMPLAYE_H_
