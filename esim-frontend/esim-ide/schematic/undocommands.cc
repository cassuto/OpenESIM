#include "undocommands.h"
#include "schematicview.h"
#include "schematic.h"
#include "errtrace.h"
#include "element.h"
#include <sstream>

namespace schematic
{

////////////////////////////////////////////////////////////////////////////////
// Command Record for Schematic element's editing                             //
////////////////////////////////////////////////////////////////////////////////

SchematicUndoEditCommand::SchematicUndoEditCommand(const QString &command,
                           SchematicViewport *viewport,
                           const std::vector<Element *> &elements,
                           const std::string &prevStream, const std::string &nextStream) :
    QUndoCommand(command),
    m_viewport(viewport),
    m_elements(elements), // copy
    m_prevStream(prevStream), // copy
    m_nextStream(nextStream) // copy
{
}

void SchematicUndoEditCommand::undo()
{
  std::stringstream ss(m_prevStream);
  m_viewport->document()->importElements(ss, m_elements);
  m_viewport->update();
}

void SchematicUndoEditCommand::redo()
{
  std::stringstream ss(m_nextStream);
  m_viewport->document()->importElements(ss, m_elements);
  m_viewport->update();
}

////////////////////////////////////////////////////////////////////////////////
// Command Record for Schematic element's inserting                           //
////////////////////////////////////////////////////////////////////////////////

SchematicUndoInsertCommand::SchematicUndoInsertCommand(const QString &command,
                           SchematicViewport *viewport,
                           Element *instance,
                           const std::string &prevStream) :
    QUndoCommand(command),
    m_viewport(viewport),
    m_instance(instance),
    m_prevStream(prevStream), // copy
    m_elementAttached(true)
{
}

SchematicUndoInsertCommand::~SchematicUndoInsertCommand()
{
  if (m_elementAttached)
    {
      m_viewport->document()->removeElement(m_instance);
      m_viewport->update();
    }
}

void SchematicUndoInsertCommand::undo()
{
  if (m_elementAttached)
    {
      m_viewport->document()->detachElement(m_instance);
      m_viewport->update();
      m_elementAttached = false;
    }
}

void SchematicUndoInsertCommand::redo()
{
  if (!m_elementAttached)
    {
      m_viewport->document()->attachElement(m_instance);
      m_viewport->update();
      m_elementAttached = true;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Command Record for Schematic element's deleting                            //
////////////////////////////////////////////////////////////////////////////////

SchematicUndoDeleteCommand::SchematicUndoDeleteCommand(const QString &command,
                           SchematicViewport *viewport,
                           Element *instance,
                           const std::string &prevStream) :
    QUndoCommand(command),
    m_viewport(viewport),
    m_instance(instance),
    m_prevStream(prevStream), // copy
    m_canRelease(true)
{
  m_viewport->document()->detachElement(m_instance);
  m_viewport->update();
}

SchematicUndoDeleteCommand::~SchematicUndoDeleteCommand()
{
  if (m_canRelease)
    {
      delete m_instance;
    }
}

void SchematicUndoDeleteCommand::undo()
{
  if (m_canRelease)
    {
      m_viewport->document()->attachElement(m_instance);
      m_viewport->update();
      m_canRelease = false;
    }
}

void SchematicUndoDeleteCommand::redo()
{
  if (!m_canRelease)
    {
      m_viewport->document()->detachElement(m_instance);
      m_viewport->update();
      m_canRelease = true;
    }
}

} // namespace schematic
