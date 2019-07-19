#ifndef SCHEMATICVIEW_H
#define SCHEMATICVIEW_H

#include <QScrollArea>
#include <QOpenGLWidget>
#include <QObject>
#include <QString>
#include <vector>
#include <sstream>

class QMenu;
class QAction;
class QUndoStack;

namespace persistence {
  class PersistenceDom;
}

namespace schematic {

enum EditMode
{
  EDIT_MODE_SELECT = 0,
  EDIT_MODE_INSERT_ELEMENT
};

class Schematic;
class Element;

class SchematicViewport;
typedef void (SchematicViewport::*pfnMouseAction)(QMouseEvent *, int, int);
typedef void (SchematicViewport::*pfnKeyAction)(QKeyEvent *);

class SchematicViewport : public QOpenGLWidget
{
  Q_OBJECT
public:
  SchematicViewport(Schematic *document, QWidget *parent);
  inline Schematic *document() const { return m_document; }
  inline QUndoStack *undoStack() const { return m_undoStack; }

  void switchMode(EditMode mode, const char *classname=0l);
  void scale(float factor);
  void setSize(int w, int h);
  void setSize(int type);
  void changed(bool update = false);

  void keyPressEvent(QKeyEvent *event);
protected:
  void adjSize(int w=-1, int h=-1);
  void beginEdit(const std::vector<Element *> &targets);
  void beginEdit(Element *element);
  void endEdit(const QString &command, const std::vector<Element *> &targets);
  void endEdit(const QString &command, Element *element);
  void endInsert(Element *target);
  void endDelete(Element *target);
  void setModified(bool val);

  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void leaveEvent(QEvent *event);

  void mousePressSelect(QMouseEvent *event, int logicalX, int logicalY);
  void mouseDbclickSelect(QMouseEvent *event, int logicalX, int logicalY);
  void mouseMoveSelect(QMouseEvent *event, int logicalX, int logicalY);
  void mouseReleaseSelect(QMouseEvent *event, int logicalX, int logicalY);
  void keyPressSelect(QKeyEvent *event);

  void mouseMoveElementMoving(QMouseEvent *event, int x, int y);
  void mouseReleaseElementMoving(QMouseEvent *event, int x, int y);

  void finishElementInsertion(QMouseEvent *event);
  void giveupElementInsertion();
  void mousePressInsertElement(QMouseEvent *event, int logicalX, int logicalY);
  void mouseLeaveInsertElement(QMouseEvent *event, int logicalX, int logicalY);
  void keyPressInsertElement(QKeyEvent *event);
  void keyPressGiveupElementInsertion(QKeyEvent *event);
  void mouseMoveInsert_1P1R(QMouseEvent *event, int logicalX, int logicalY);
  void mouseMoveInsert_2P1D(QMouseEvent *event, int logicalX, int logicalY);
  void mousePress2Insert_2P1D(QMouseEvent *event, int logicalX, int logicalY);
  void mouseMove2Insert_2P1D(QMouseEvent *event, int logicalX, int logicalY);
  void mousePress3Insert_2P1D(QMouseEvent *event, int logicalX, int logicalY);
  void mouseMoveInsert_4P(QMouseEvent *event, int logicalX, int logicalY);
  void mousePress2Insert_4P(QMouseEvent *event, int logicalX, int logicalY);
  void mouseMoveInsert_1P(QMouseEvent *event, int logicalX, int logicalY);
  void mousePress2Insert_1P(QMouseEvent *event, int logicalX, int logicalY);
  void mouseReleaseInsert_1P(QMouseEvent *event, int logicalX, int logicalY);

  int m_initDistance;

  int m_mx1, m_my1;
  int m_mx2, m_my2;
  int m_tx1, m_ty1;
  std::vector<Element *> m_currentElements;
  Element *m_currentElement;
  std::string m_currentClassname;

  inline void setMousePressAction(pfnMouseAction pfn) { m_mousePressAction = pfn; }
  inline void setMouseDbclickAction(pfnMouseAction pfn) { m_mouseDbclickAction = pfn; }
  inline void setMouseMoveAction(pfnMouseAction pfn) { m_mouseMoveAction = pfn; }
  inline void setMouseReleaseAction(pfnMouseAction pfn) { m_mouseReleaseAction = pfn; }
  inline void setMouseLeaveAction(pfnMouseAction pfn) { m_mouseLeaveAction = pfn; }
  inline void setKeyPressAction(pfnKeyAction pfn) { m_keyPressAction = pfn; }

private:
  Schematic *m_document;
  EditMode m_mode;
  pfnMouseAction m_mousePressAction;
  pfnMouseAction m_mouseMoveAction;
  pfnMouseAction m_mouseReleaseAction;
  pfnMouseAction m_mouseDbclickAction;
  pfnMouseAction m_mouseLeaveAction;
  pfnKeyAction m_keyPressAction;

  bool m_modified;
  std::stringstream m_undoPrevStream;
  std::stringstream m_undoNextStream;
  QUndoStack *m_undoStack;

private:
  QMenu *m_assureSelectionMenu;
  int m_assureSelectId;

private slots:
  void slotAssureSelectionMenuTriggered(QAction *action);
signals:
  void modeChanged(int mode, const char *classname);
  void schematicChanged();
};

class SchematicView : public QScrollArea
{
public:
  SchematicView(bool symbolMode, QWidget *parent = 0l);
  ~SchematicView();

  void test();

  void keyPressEvent(QKeyEvent *event);

  inline Schematic *document() const { return m_viewport->document(); }
  inline SchematicViewport *view() const { return m_viewport; }
  inline QUndoStack *undoStack() const { return m_viewport->undoStack(); }

private:
  Schematic *m_document;
  SchematicViewport *m_viewport;
};

} // namespace

#endif // SCHEMATICVIEW_H
