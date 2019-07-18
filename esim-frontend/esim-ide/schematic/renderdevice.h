/* -*-c++-*- */
#ifndef RENDERDEVICE_H
#define RENDERDEVICE_H

class QPainter;

namespace schematic {

class Schematic;
class LineStyle;
class FillStyle;
class TextStyle;

class RenderDevice
{
public:
  RenderDevice(Schematic *doc, QPainter *painter);

  LineStyle setLineStyle(const LineStyle &line);
  FillStyle setFillStyle(const FillStyle &fill);
  TextStyle setTextStyle(const TextStyle &text);

  void point(int x, int y);
  void line(int x1, int y1, int x2, int y2);
  void rect(int x, int y, int dx, int dy);
  void ellipse(int x, int y, int dx, int dy);
  void arc(int x, int y, int dx, int dy, int startAngle, int spanAngle);
  void chord(int x, int y, int dx, int dy, int startAngle, int spanAngle);
  int text(const char *text, int x, int y, int *height=0l, int angle=0);

  static void getTextSize(const char *text, const TextStyle &style, int *width, int *height);

public:
  bool GraphAntiAliasing;
  bool TextAntiAliasing;
  QPainter *painter;
  int lineSpacing;
  float scale;
  float fontScale;
private:
  Schematic *m_doc;
};

} // namespace

#endif // RENDERDEVICE_H
