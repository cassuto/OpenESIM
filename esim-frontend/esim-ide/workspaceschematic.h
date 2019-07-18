#ifndef WORKSPACE_SCHEMATIC_H_
#define WORKSPACE_SCHEMATIC_H_

#include "workspace.h"

namespace schematic
{
  class SchematicView;
}

namespace esim
{

class WorkspaceSchematic : public WorkspaceFixed
{
  Q_OBJECT
public:
  WorkspaceSchematic(MainWindow *mainWindow, bool symbolMode, const char *filename);

  ESD_OVERRIDE QWidget *view() const;

  ESD_OVERRIDE int open();

  ESD_OVERRIDE int save();

  ESD_OVERRIDE int close();

  ESD_OVERRIDE bool modified();

  inline bool symbolMode() const { return m_symbolMode; }

protected:
  bool m_symbolMode;
  schematic::SchematicView *m_schematicView;
  QAction *m_actionSelection,
          *m_actionInsertPin,
          *m_actionInsertDevice,
          *m_actionInsertLine,
          *m_actionInsertRect,
          *m_actionInsertCircle,
          *m_actionInsertArc,
          *m_actionInsertChord,
          *m_actionInsertText,
          *m_actionInsertOrigin;
  float m_scaleMax;
  float m_scaleMin;
  float m_scaleStep;

private slots:
  void slotSelection(bool checked);
  void slotInsertPin(bool checked);
  void slotInsertDevice(bool checked);
  void slotInsertLine(bool checked);
  void slotInsertRect(bool checked);
  void slotInsertCircle(bool checked);
  void slotInsertArc(bool checked);
  void slotInsertChord(bool checked);
  void slotInsertText(bool checked);
  void slotInsertOrigin(bool checked);
  void slotModeChanged(int mode, const char *classname);
  void slotZoomCenter(bool);
  void slotZoomIn(bool);
  void slotZoomOut(bool);
  void slotZoomFit(bool);
};

} // namespace esim


#endif // WORKSPACE_SCHEMATIC_H_
