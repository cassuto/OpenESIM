#ifndef SCHEMATIC_GRAPHIC_STYLE_DIALOG_H_
#define SCHEMATIC_GRAPHIC_STYLE_DIALOG_H_

#include "ui_schematic-graphicstyledialog.h"

#include "styletemplate.h"

namespace schematic
{

class PreviewElement;
class ElementRect;
class ElementText;

class SchematicGraphicsStyleDialog : public QDialog
{
  Q_OBJECT
public:
  SchematicGraphicsStyleDialog(const LineStyle &line, QWidget *parent = 0);
  SchematicGraphicsStyleDialog(const FillStyle &fill, QWidget *parent = 0);
  SchematicGraphicsStyleDialog(const TextStyle &text, QWidget *parent = 0);

  inline const LineStyle &lineStyle() const { return m_line; }
  inline const FillStyle &fillStyle() const { return m_fill; }
  inline const TextStyle &textStyle() const { return m_text; }

protected:
  void setupUi();
  void setupLine(const LineStyle &line);
  void setupFill(const FillStyle &fill);
  void setupText(const TextStyle &text);

private:
  Ui::SchematicGraphicsStyleDialog ui;
  LineStyle m_line, m_lineGlobal;
  bool m_editLine;
  FillStyle m_fill, m_fillGlobal;
  bool m_editFill;
  TextStyle m_text, m_textGlobal;
  bool m_editText;
  bool m_blockUpdating;
  PreviewElement *m_graphicsPreview;
  PreviewElement *m_textPreview;
  ElementRect *m_previewRect;
  ElementText *m_previewText;

private slots:
  void onLineColorPress();
  void onFillColorPress();
  void onFillBkcolorPress();
  void onTextColorPress();
  void onAccept();
  void setLine();
  void setFill();
  void setText();
  void updateGlobals();
  void onGraphicsPreviewResize(int width, int height);
  void onTextPreviewResize(int width, int height);
  void onTemplateChanged(int index);
};

} // namespace schematic

#endif // SCHEMATIC_GRAPHIC_STYLE_DIALOG_H_
