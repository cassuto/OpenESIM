#ifndef PERSISTENCE_H_
#define PERSISTENCE_H_

#include <vector>
#include <iostream>

namespace persistence
{

class PersistenceDom;

# define RUNNABLE_CALLBACK PersistenceCallback
# define RUNNABLE_CALLBACK_BASE PersistenceCallbackBase
# define CALLBACK_RETURN int
# define CALLBACK_PARAM_DECLS (const char *filename, PersistenceDom *dom, void *opaque)
# define CALLBACK_PARAMS (filename, dom, opaque)
#include "runnablecallback.h"

enum persistenceType
{
  PERSISTENCE_SCH = 0,
  PERSISTENCE_SSYM
};

class Persistence
{
public:
  Persistence(persistenceType type, int vermajor, int verminor);
  ~Persistence();

public:
  void clear();
  PersistenceDom *append(const char *className);
  inline const std::vector<PersistenceDom *> & doms() const { return m_persistences; }

  int save(std::ostream &stream);
  int open(std::istream &stream, PersistenceCallbackBase &callback, void *opaque = 0l);

private:
  persistenceType m_type;
  int m_vermajor;
  int m_verminor;
  std::vector<PersistenceDom *> m_persistences;
};

} /* namespace persistence */

#endif /* PERSISTENCE_H_ */
