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

#include "styletemplate.h"
#include "persistencedom.h"
#include "errtrace.h"
#include <cassert>

namespace schematic
{

////////////////////////////////////////////////////////////////////////////////
// Line Style Template                                                        //
////////////////////////////////////////////////////////////////////////////////

LineStyle::LineStyle(const std::string &symbol, const templatedColor & color, float width, templatedStyle line) :
    StyleBase(symbol),
    m_color(color),
    m_width(width),
    m_line(line)
{
  set();
}

LineStyle::LineStyle(const std::string &symbol, const QPen &penIns) :
    StyleBase(symbol),
    m_color(penIns.color()),
    m_width(penIns.widthF()),
    m_penIns(penIns)
{
    switch(penIns.style())
    {
      case Qt::NoPen:
        m_line = LINE_NONE; break;
      case Qt::SolidLine:
        m_line = LINE_SOLID; break;
      case Qt::DashLine:
        m_line = LINE_DASH; break;
      case Qt::DotLine:
        m_line = LINE_DOT; break;
      case Qt::DashDotLine:
        m_line = LINE_DASH_DOT; break;
      case Qt::DashDotDotLine:
        m_line = LINE_DASH_DOTDOT; break;
      default:
        m_line = LINE_SOLID;
    }
}

void LineStyle::set()
{
  Qt::PenStyle attr = Qt::SolidLine;
  switch(m_line)
  {
    case LINE_NONE:
      attr = Qt::NoPen; break;
    case LINE_SOLID:
      attr = Qt::SolidLine; break;
    case LINE_DASH:
      attr = Qt::DashLine; break;
    case LINE_DOT:
      attr = Qt::DotLine; break;
    case LINE_DASH_DOT:
      attr = Qt::DashDotLine; break;
    case LINE_DASH_DOTDOT:
      attr = Qt::DashDotDotLine; break;
    default:
      attr = Qt::SolidLine;
  }
  m_penIns = QPen(QColor(m_color.R, m_color.G, m_color.B), m_width, attr);
}

int LineStyle::serialize(persistence::PersistenceDom *dom)
{
  bool custom;
  RC_CHECK_RETURN(dom->ser(m_symbol));

  const LineStyle* ref = StyleTemplates::getInstance()->getLineStyleRef(m_symbol);

  custom = !(this->m_color == ref->m_color);
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->ser(m_color.R));
      RC_CHECK_RETURN(dom->ser(m_color.G));
      RC_CHECK_RETURN(dom->ser(m_color.B));
    }
    
  custom = this->m_width != ref->m_width;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_width)); }

  custom = this->m_line != ref->m_line;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser((int)m_line)); }

  return 0;
}

int LineStyle::deserialize(persistence::PersistenceDom *dom)
{
  bool custom;
  RC_CHECK_RETURN(dom->des(&m_symbol));

  const LineStyle* ref = StyleTemplates::getInstance()->getLineStyleRef(m_symbol);

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->des(&m_color.R));
      RC_CHECK_RETURN(dom->des(&m_color.G));
      RC_CHECK_RETURN(dom->des(&m_color.B));
    }
  else
    {
      m_color = ref->m_color;
    }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_width)); }
  else
    { m_width = ref->m_width; }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      int line;
      RC_CHECK_RETURN(dom->des(&line));
      m_line = static_cast<templatedStyle>(line);
    }
  else
    {
      m_line = ref->m_line;
    }
  set();
  return 0;
}

bool LineStyle::operator ==(const LineStyle &src) const
{
  return (m_color == src.m_color &&
      m_width == src.m_width &&
      m_line == src.m_line);
}

////////////////////////////////////////////////////////////////////////////////
// Fill Style Template                                                        //
////////////////////////////////////////////////////////////////////////////////

FillStyle::FillStyle(const std::string &symbol,
                     const templatedColor &color,
                     const templatedColor &bkcolor,
                     float width,
                     templatedStyle line,
                     templatedStyle brush)
  : LineStyle(symbol, color, width, line),
    m_bkcolor(bkcolor),
    m_brush(brush)
{
  set();
}

FillStyle::FillStyle(const std::string &symbol, const QPen &penIns, const QBrush &brushIns)
  : LineStyle(symbol, penIns),
    m_bkcolor(brushIns.color()),
    m_brushIns(brushIns)
{
  switch(brushIns.style())
  {
    case Qt::NoBrush:
      m_brush = BRUSH_NONE; break;
    case Qt::SolidPattern:
      m_brush = BRUSH_SOLID; break;
    case Qt::HorPattern:
      m_brush = BRUSH_HOR; break;
    case Qt::VerPattern:
      m_brush = BRUSH_VER; break;
    case Qt::CrossPattern:
      m_brush = BRUSH_CROSS; break;
    default:
      m_brush = BRUSH_NONE;
  }
}

void FillStyle::set()
{
  LineStyle::set();
  Qt::BrushStyle attr = Qt::NoBrush;
  switch(m_brush)
  {
    case BRUSH_NONE:
      attr = Qt::NoBrush; break;
    case BRUSH_SOLID:
      attr = Qt::SolidPattern; break;
    case BRUSH_HOR:
      attr = Qt::HorPattern; break;
    case BRUSH_VER:
      attr = Qt::VerPattern; break;
    case BRUSH_CROSS:
      attr = Qt::CrossPattern; break;
    default:
      attr = Qt::NoBrush;
  }

  m_brushIns = QBrush(QColor(m_bkcolor.R,m_bkcolor.G,m_bkcolor.B), attr);
}

int FillStyle::serialize(persistence::PersistenceDom *dom)
{
  bool custom;
  RC_CHECK_RETURN(dom->ser(m_symbol));

  const FillStyle* ref = StyleTemplates::getInstance()->getFillStyleRef(m_symbol);

  custom = !(this->m_color == ref->m_color);
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->ser(m_color.R));
      RC_CHECK_RETURN(dom->ser(m_color.G));
      RC_CHECK_RETURN(dom->ser(m_color.B));
    }
  custom = !(this->m_bkcolor == ref->m_bkcolor);
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->ser(m_bkcolor.R));
      RC_CHECK_RETURN(dom->ser(m_bkcolor.G));
      RC_CHECK_RETURN(dom->ser(m_bkcolor.B));
    }

  custom = this->m_width != ref->m_width;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_width)); }

  custom = this->m_line != ref->m_line;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser((int)m_line)); }

  custom = this->m_brush != ref->m_brush;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser((int)m_brush)); }
  return 0;
}

int FillStyle::deserialize(persistence::PersistenceDom *dom)
{
  bool custom;
  RC_CHECK_RETURN(dom->des(&m_symbol));

  const FillStyle* ref = StyleTemplates::getInstance()->getFillStyleRef(m_symbol);

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->des(&m_color.R));
      RC_CHECK_RETURN(dom->des(&m_color.G));
      RC_CHECK_RETURN(dom->des(&m_color.B));
    }
  else
    {
      m_color = ref->m_color;
    }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->des(&m_bkcolor.R));
      RC_CHECK_RETURN(dom->des(&m_bkcolor.G));
      RC_CHECK_RETURN(dom->des(&m_bkcolor.B));
    }
  else
    {
      m_bkcolor = ref->m_bkcolor;
    }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_width)); }
  else
    { m_width = ref->m_width; }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      int sty;
      RC_CHECK_RETURN(dom->des(&sty));
      m_line = static_cast<templatedStyle>(sty);
    }
  else
    {
      m_line = ref->m_line;
    }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      int sty;
      RC_CHECK_RETURN(dom->des(&sty));
      m_brush = static_cast<templatedStyle>(sty);
    }
  else
    {
      m_brush = ref->m_brush;
    }
  set();
  return 0;
}

bool FillStyle::operator ==(const FillStyle &src) const
{
  return (LineStyle::operator ==(src) &&
      (m_bkcolor == src.m_bkcolor &&
      m_brush == src.m_brush));
}

////////////////////////////////////////////////////////////////////////////////
// Text Style Template                                                        //
////////////////////////////////////////////////////////////////////////////////

TextStyle::TextStyle(const std::string &symbol,
                     const templatedColor & color,
                     const std::string &fontName,
                     int sizePt,
                     bool bold,
                     bool italic,
                     bool underline) :
    StyleBase(symbol),
    m_color(color),
    m_fontName(fontName),
    m_sizePt(sizePt),
    m_bold(bold),
    m_italic(italic),
    m_underline(underline)
{
  set();
}

TextStyle::TextStyle(const std::string &symbol, const QPen &penIns, const QFont &fontIns) :
    StyleBase(symbol),
    m_color(penIns.color()),
    m_fontName(fontIns.family().toStdString()),
    m_sizePt(fontIns.pointSize()),
    m_bold(fontIns.bold()),
    m_italic(fontIns.italic()),
    m_underline(fontIns.underline()),
    m_penIns(penIns),
    m_fontIns(fontIns)
{
}

void TextStyle::set()
{
  m_fontIns = QFont(m_fontName.c_str(), m_sizePt);
  m_fontIns.setBold(m_bold);
  m_fontIns.setItalic(m_italic);
  m_fontIns.setUnderline(m_underline);
  m_penIns = QPen(QColor(m_color.R, m_color.G, m_color.B));
}

int TextStyle::serialize(persistence::PersistenceDom *dom)
{
  bool custom;
  RC_CHECK_RETURN(dom->ser(m_symbol));

  const TextStyle* ref = StyleTemplates::getInstance()->getTextStyleRef(m_symbol);

  custom = !(this->m_color == ref->m_color);
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->ser(m_color.R));
      RC_CHECK_RETURN(dom->ser(m_color.G));
      RC_CHECK_RETURN(dom->ser(m_color.B));
    }

  custom = this->m_fontName.compare(ref->m_fontName) != 0;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_fontName)); }

  custom = this->m_sizePt != ref->m_sizePt;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_sizePt)); }

  custom = this->m_bold != ref->m_bold;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_bold)); }

  custom = this->m_italic != ref->m_italic;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_italic)); }

  custom = this->m_underline != ref->m_underline;
  RC_CHECK_RETURN(dom->ser(custom));
  if (custom)
    { RC_CHECK_RETURN(dom->ser(m_underline)); }
  return 0;
}

int TextStyle::deserialize(persistence::PersistenceDom *dom)
{
  bool custom;
  RC_CHECK_RETURN(dom->des(&m_symbol));

  const TextStyle* ref = StyleTemplates::getInstance()->getTextStyleRef(m_symbol);

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    {
      RC_CHECK_RETURN(dom->des(&m_color.R));
      RC_CHECK_RETURN(dom->des(&m_color.G));
      RC_CHECK_RETURN(dom->des(&m_color.B));
    }
  else
    {
      m_color = ref->m_color;
    }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_fontName)); }
  else
    { m_fontName = ref->m_fontName; }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_sizePt)); }
  else
    { m_sizePt = ref->m_sizePt; }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_bold)); }
  else
    { m_bold = ref->m_bold; }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_italic)); }
  else
    { m_italic = ref->m_italic; }

  RC_CHECK_RETURN(dom->des(&custom));
  if (custom)
    { RC_CHECK_RETURN(dom->des(&m_underline)); }
  else
    { m_underline = ref->m_underline; }
  set();
  return 0;
}

bool TextStyle::operator==(const TextStyle &src) const
{
  return (m_color == src.m_color &&
      m_fontName == src.m_fontName &&
      m_sizePt == src.m_sizePt &&
      m_bold == src.m_bold &&
      m_italic == src.m_italic &&
      m_underline == src.m_underline);
}

////////////////////////////////////////////////////////////////////////////////
// Style Templates                                                            //
////////////////////////////////////////////////////////////////////////////////

StyleTemplates *StyleTemplates::m_instance = 0l;

StyleTemplates::StyleTemplates()
{
  m_lineStyles["none"]      = new LineStyle("none", templatedColor(0, 0, 0), 1, LINE_SOLID);
  m_lineStyles["wire"]      = new LineStyle("wire", templatedColor(0, 128, 255), 1, LINE_SOLID);
  m_lineStyles["component"] = new LineStyle("component", templatedColor(128, 0, 0), 1, LINE_SOLID);
  m_lineStyles["pin"]       = new LineStyle("pin", templatedColor(128, 0, 0), 1, LINE_SOLID);

  m_fillStyles["none"]      = new FillStyle("none", templatedColor(0, 0, 0), templatedColor(0, 0, 0), 1, LINE_SOLID, BRUSH_NONE);
  m_fillStyles["component"] = new FillStyle("component", templatedColor(128, 0, 0), templatedColor(0, 0, 0), 1, LINE_SOLID, BRUSH_NONE);

  m_textStyles["none"]      = new TextStyle("none", templatedColor(0, 0, 0), "Times New Roman", 8, false, false, false);
  m_textStyles["component"] = new TextStyle("component", templatedColor(128, 0, 0), "Times New Roman", 8, false, false, false);
  m_textStyles["pin"]       = new TextStyle("pin", templatedColor(128, 0, 0), "Times New Roman", 8, false, false, false);
  m_textStyles["symbol"]    = new TextStyle("symbol", templatedColor(0, 132, 132), "Times New Roman", 8, true, false, false);
}

StyleTemplates::~StyleTemplates()
{
  for(std::map<std::string, LineStyle *>::iterator it=m_lineStyles.begin(); it != m_lineStyles.end(); it++)
    {delete it->second;}
  for(std::map<std::string, FillStyle *>::iterator it=m_fillStyles.begin(); it != m_fillStyles.end(); it++)
    {delete it->second;}
  for(std::map<std::string, TextStyle *>::iterator it=m_textStyles.begin(); it != m_textStyles.end(); it++)
    {delete it->second;}
}

const LineStyle *StyleTemplates::getLineStyleRef(const std::string &symbol)
{
  const LineStyle *line = m_lineStyles[symbol];
  return line ? line : m_lineStyles["none"];
}
const FillStyle *StyleTemplates::getFillStyleRef(const std::string &symbol)
{
  const FillStyle *fill = m_fillStyles[symbol];
  return fill ? fill : m_fillStyles["none"];
}
const TextStyle *StyleTemplates::getTextStyleRef(const std::string &symbol)
{
  const TextStyle *text = m_textStyles[symbol];
  return text ? text : m_textStyles["none"];
}

LineStyle StyleTemplates::copyLineStyle(const std::string &symbol)
{return *getLineStyleRef(symbol);}
FillStyle StyleTemplates::copyFillStyle(const std::string &symbol)
{return *getFillStyleRef(symbol);}
TextStyle StyleTemplates::copyTextStyle(const std::string &symbol)
{return *getTextStyleRef(symbol);}

const char *StyleTemplates::getTemplatedStyleName(templatedStyle style)
{
  switch (style)
  {
    case LINE_NONE:
    case BRUSH_NONE:
      return "None";
    case LINE_SOLID:
    case BRUSH_SOLID:
      return "Solid";
    case LINE_DASH:
      return "Dash";
    case LINE_DOT:
      return "Dot";
    case LINE_DASH_DOT:
      return "Dash Dot";
    case LINE_DASH_DOTDOT:
      return "Dash Dot dot";
    case BRUSH_HOR:
      return "Horizon";
    case BRUSH_VER:
      return "Vertical";
    case BRUSH_CROSS:
      return "Cross";
    default:
      assert(0);
      return "Unknown";
  }
}


} // namespace schematic
