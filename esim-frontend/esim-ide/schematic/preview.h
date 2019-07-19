#ifndef SCHEMATIC_PREVIEW_H_
#define SCHEMATIC_PREVIEW_H_

#include <QWidget>

namespace schematic
{

class Schematic;

class PreviewElement : public QWidget
{
  Q_OBJECT
public:
  PreviewElement(QWidget *parent = 0l);
  ~PreviewElement();

  inline Schematic *document() const { return m_document; }

protected:
  void resizeEvent(QResizeEvent * event);
  void paintEvent(QPaintEvent *event);

private:
  Schematic *m_document;
signals:
  void resize(int width, int height);
};

class PreviewSchematic : public QWidget
{
  Q_OBJECT
 public:
  PreviewSchematic(Schematic *document, QWidget *parent = 0l);
   ~PreviewSchematic();

   QSize sizeHint() const;
 protected:
   void resizeEvent(QResizeEvent * event);
   void paintEvent(QPaintEvent *event);

 private:
   Schematic *m_document;
   int m_thresholdX, m_thresholdY;
   float m_viewW, m_viewH;
 signals:
   void resize(int width, int height);
};

} // namespace schematic

#endif // SCHEMATIC_PREVIEW_H_
