#ifndef UNDO_COMMANDS_H_
#define UNDO_COMMANDS_H_

#include <string>
#include <vector>
#include <QtWidgets/QUndoCommand>
#include <QtCore/QString>

namespace schematic
{

class SchematicViewport;
class Element;

class SchematicUndoEditCommand : public QUndoCommand
{
public:
  SchematicUndoEditCommand(const QString &command,
                           SchematicViewport *viewport,
                           const std::vector<Element *> &elements,
                           const std::string &prevStream, const std::string &nextStream);
  void undo();
  void redo();

protected:
  SchematicViewport *m_viewport;
  std::vector<Element *> m_elements;
  std::string m_prevStream;
  std::string m_nextStream;
};

class SchematicUndoInsertCommand : public QUndoCommand
{
public:
  SchematicUndoInsertCommand(const QString &command,
                           SchematicViewport *viewport,
                           Element *instance,
                           const std::string &prevStream);
  ~SchematicUndoInsertCommand();

  void undo();
  void redo();

protected:
  SchematicViewport *m_viewport;
  Element *m_instance;
  std::string m_prevStream;
  bool m_elementAttached;
};

class SchematicUndoDeleteCommand : public QUndoCommand
{
public:
  SchematicUndoDeleteCommand(const QString &command,
                           SchematicViewport *viewport,
                           Element *instance,
                           const std::string &prevStream);
  ~SchematicUndoDeleteCommand();

  void undo();
  void redo();

protected:
  SchematicViewport *m_viewport;
  Element *m_instance;
  std::string m_prevStream;
  bool m_canRelease;
};

} // namespace schematic

#endif // UNDO_COMMANDS_H_
