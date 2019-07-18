/******************************************************************************
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
 ******************************************************************************/

#include "persistence.h"
#include "persistencedom.h"
#include "errtrace.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>

namespace persistence
{

Persistence::Persistence(persistenceType type, int vermajor, int verminor) :
    m_type(type),
    m_vermajor(vermajor),
    m_verminor(verminor)
{
}
Persistence::~Persistence()
{
  clear();
}

void Persistence::clear()
{
  for(std::size_t i=0; i < m_persistences.size(); i++)
    {
      delete m_persistences[i];
    }
  m_persistences.clear();
}

PersistenceDom *Persistence::append(const char *className)
{
  PersistenceDom *persistence = new PersistenceDom(className);
  m_persistences.push_back(persistence);
  return persistence;
}

int Persistence::save(std::ostream &entry)
{
  int rc;
  /*
   * Write file header (magic + version)
   */
  const char *magic;
  switch(m_type)
  {
    case PERSISTENCE_SCH:
      magic = "sch"; break;
    case PERSISTENCE_SSYM:
      magic = "ssym"; break;
    default:
      return -RC_FAILED;
  }
  entry << "ESIM " << magic << std::endl;
  entry << "V " << m_vermajor << " " << m_verminor << std::endl;

  /*
   * Write all the children DOMs to the file.
   */
  for(std::size_t i=0; i < m_persistences.size(); i++)
    {
      entry << m_persistences[i]->getClassName() << std::endl;
      entry << "%" << std::endl;

      if (RC_FAILURE(rc = m_persistences[i]->toStream(entry)))
        return rc;

      entry << std::endl << "%" << std::endl;
    }
  return 0;
}

enum perstate
{
  STATE_CLASSNAME,
  STATE_CLASSNAME_GOT,
  STATE_BUFF
};

int Persistence::open(std::istream &stream, PersistenceCallbackBase &callback, void *opaque)
{
  int rc = 0;
  enum perstate state = STATE_CLASSNAME;
  std::string className;
  std::string token;
  std::string buff;

  /*
   * Validate magic of file
   */
  stream >> token;
  if (token.compare("ESIM"))
    {rc = -RC_INVALID_MAGIC; goto err; }

  stream >> token;
  persistenceType type;
  if (token.compare("sch") == 0)
    type = PERSISTENCE_SCH;
  else if (token.compare("ssym") == 0)
    type = PERSISTENCE_SSYM;
  if (type != m_type)
    {rc = -RC_INVALID_FILE_TYPE; goto err; }

  /*
   * Validate version
   */
  stream >> token;
  if (token.compare("V"))
    {rc = -RC_INVALID_FILE_VERSION; goto err; }

  int vermajor, verminor;
  stream >> vermajor >> verminor;
  if (vermajor > m_vermajor )
    {rc = -RC_INVALID_FILE_VERSION; goto err; }
  if (vermajor == m_vermajor && verminor > m_verminor )
    {rc = -RC_INVALID_FILE_VERSION; goto err; }

  this->clear();

  while (std::getline(stream, token))
    {
      if (token.size() > 1)
        {
          switch(state)
          {
            case STATE_CLASSNAME:
              className = token;
              state = STATE_CLASSNAME_GOT;
              break;
            case STATE_BUFF:
              buff.append(token);
              break;
            default:
              rc = RC_DB_BROKEN;
              goto err;
          }
        }
      else if (token[0] == '%' && token[1] == 0)
        {
          switch(state)
          {
            case STATE_CLASSNAME_GOT:
              buff.clear();
              state = STATE_BUFF;
              break;
            case STATE_BUFF:
              {
                /*
                 * Call the handler.
                 */
                PersistenceDom *dom = this->append(className.c_str());
                if (RC_FAILURE(rc = dom->fromString(buff)))
                  goto err;
                if (RC_FAILURE(rc = callback(className.c_str(), dom, opaque)))
                  goto err;

                state = STATE_CLASSNAME; // BUFF_GOT
                break;
              }
            default:
              { rc = -RC_DB_BROKEN; goto err; }
          }
        }
    }
err:
  return rc;
}


} /* namespace persistence */
