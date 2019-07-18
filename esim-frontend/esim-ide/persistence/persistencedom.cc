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

#include "persistencedom.h"

#include "persistencejson.h"
#include "errtrace.h"
#include <cstring>

namespace persistence
{

PersistenceDom::PersistenceDom(const char *className) :
  m_className(className),
  m_root(cJSON_CreateArray()),
  m_readPos(0l)
{
}

PersistenceDom::~PersistenceDom()
{
  cJSON_Delete(m_root);
}

int PersistenceDom::toStream(std::ostream &stream)
{
  char *buff = cJSON_Print(m_root);
  stream.write(buff, std::strlen(buff));
  delete [] buff;
  return 0;
}

int PersistenceDom::fromString(std::string &string)
{
  m_readPos = 0l;
  cJSON_Delete(m_root);

  m_root = cJSON_Parse(string.c_str());
  if (!m_root)
    { return -RC_DB_BROKEN; }
  if (cJSON_Array != m_root->type)
    {
      cJSON_Delete(m_root);
      return -RC_DB_BROKEN;
    }
  m_readPos = m_root->child;
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Serialization Interface                                                    //
////////////////////////////////////////////////////////////////////////////////

// int
int PersistenceDom::ser(int intval) {
  cJSON_AddItemToArray(m_root, cJSON_CreateNumber(intval));
  return 0;
}
int PersistenceDom::ser(const std::vector<int> & intvals) {
  cJSON *array = cJSON_CreateArray();
  for(size_t i=0; i < intvals.size(); i++)
    {
      cJSON_AddItemToArray(array, cJSON_CreateNumber(intvals[i]));
    }
  cJSON_AddItemToArray(m_root, array);
  return 0;
}

// bool
int PersistenceDom::ser(bool boolval) {
  cJSON_AddItemToArray(m_root, cJSON_CreateBool(boolval));
  return 0;
}
int PersistenceDom::ser(const std::vector<bool> & boolvals) {
  cJSON *array = cJSON_CreateArray();
  for(size_t i=0; i < boolvals.size(); i++)
    {
      cJSON_AddItemToArray(array, cJSON_CreateBool(boolvals[i]));
    }
  cJSON_AddItemToArray(m_root, array);
  return 0;
}

// float
int PersistenceDom::ser(float floatval) {
  return ser(static_cast<double>(floatval));
}
int PersistenceDom::ser(const std::vector<float> & floatvals) {
  cJSON *array = cJSON_CreateArray();
  for(size_t i=0; i < floatvals.size(); i++)
    {
      cJSON_AddItemToArray(array, cJSON_CreateNumber(static_cast<double>(floatvals[i])));
    }
  cJSON_AddItemToArray(m_root, array);
  return 0;
}

// double
int PersistenceDom::ser(double doublelval) {
  cJSON_AddItemToArray(m_root, cJSON_CreateNumber(doublelval));
  return 0;
}
int PersistenceDom::ser(const std::vector<double> & doublelvals) {
  cJSON *array = cJSON_CreateArray();
  for(size_t i=0; i < doublelvals.size(); i++)
    {
      cJSON_AddItemToArray(array, cJSON_CreateNumber(doublelvals[i]));
    }
  cJSON_AddItemToArray(m_root, array);
  return 0;
}

// string
int PersistenceDom::ser(const char *cstrval) {
  cJSON_AddItemToArray(m_root, cJSON_CreateString(cstrval));
  return 0;
}
int PersistenceDom::ser(const std::string & strval) {
  cJSON_AddItemToArray(m_root, cJSON_CreateString(strval.c_str()));
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// De serialization Interface                                                  //
////////////////////////////////////////////////////////////////////////////////

#define LEAF_CHECK(_type) { if (!m_readPos) return -RC_DB_BROKEN; NODE_CHECK_TYPE(m_readPos, _type) }
#define LEAF_CHECK2() { if (!m_readPos) return -RC_DB_BROKEN; }
#define LEAF_NEXT() { if (m_readPos) m_readPos = m_readPos->next; }
#define NODE_CHECK_TYPE(_node, _type) { if (_type != _node->type) return -RC_TYPE_MISMATCH; }

/**
 * Foreach implement for json array. Example:
 * cJSON *array = (target array);
 * FOREACH(array, node)
 *   {
 *     std::cout << node->valuestring;
 *   }
 */
#define foreach_array(_array, _var) for(cJSON *_var = _array->child; _var; _var = _var->next )

// int
int PersistenceDom::des(int *intval) {
  LEAF_CHECK(cJSON_Number)
    {
      *intval = m_readPos->valueint;
    }
  LEAF_NEXT()
  return 0;
}
int PersistenceDom::des(std::vector<int> *intvals) {
  LEAF_CHECK(cJSON_Array)
    {
      intvals->clear();
      foreach_array(m_readPos, node)
        {
          NODE_CHECK_TYPE(node, cJSON_Number)
          intvals->push_back(node->valueint);
        }
    }
  LEAF_NEXT()
  return 0;
}

// uchar
int PersistenceDom::des(unsigned char *ucharval) {
  LEAF_CHECK(cJSON_Number)
    {
      if ( m_readPos->valueint < 0 || m_readPos->valueint > 255 )
        return -RC_NUMBER_OVERFLOW;
      *ucharval = static_cast<unsigned char>(m_readPos->valueint);
    }
  LEAF_NEXT()
  return 0;
}
int PersistenceDom::des(std::vector<unsigned char> *ucharvals) {
  LEAF_CHECK(cJSON_Array)
    {
      ucharvals->clear();
      foreach_array(m_readPos, node)
        {
          NODE_CHECK_TYPE(node, cJSON_Number)
          if ( node->valueint < 0 || node->valueint > 255 )
            return -RC_NUMBER_OVERFLOW;
          ucharvals->push_back(static_cast<unsigned char>(node->valueint));
        }
    }
  LEAF_NEXT()
  return 0;
}

// bool
int PersistenceDom::des(bool *boolval) {
  LEAF_CHECK2()
  if (cJSON_True == m_readPos->type)
    *boolval = true;
  else if (cJSON_False == m_readPos->type)
    *boolval = false;
  else
    return -RC_TYPE_MISMATCH;
  LEAF_NEXT()
  return 0;
}
int PersistenceDom::des(std::vector<bool> *boolvals) {
  LEAF_CHECK(cJSON_Array)
    {
      boolvals->clear();
      foreach_array(m_readPos, node)
        {
          if (cJSON_True == node->type)
            boolvals->push_back(true);
          else if (cJSON_False == node->type)
            boolvals->push_back(false);
          else
            return -RC_TYPE_MISMATCH;
        }
    }
  LEAF_NEXT()
  return 0;
}

// float
int PersistenceDom::des(float *floatval) {
  LEAF_CHECK(cJSON_Number)
    {
      *floatval = static_cast<float>(m_readPos->valuedouble);
    }
  LEAF_NEXT()
  return 0;
}
int PersistenceDom::des(std::vector<float> *floatvals) {
  LEAF_CHECK(cJSON_Array)
    {
      floatvals->clear();
      foreach_array(m_readPos, node)
        {
          NODE_CHECK_TYPE(node, cJSON_Number)
          floatvals->push_back(static_cast<float>(node->valuedouble));
        }
    }
  LEAF_NEXT()
  return 0;
}

// double
int PersistenceDom::des(double *doublelval) {
  LEAF_CHECK(cJSON_Number)
    {
      *doublelval = m_readPos->valuedouble;
    }
  LEAF_NEXT()
  return 0;
}
int PersistenceDom::des(std::vector<double> *doublelval) {
  LEAF_CHECK(cJSON_Array)
    {
      doublelval->clear();
      foreach_array(m_readPos, node)
        {
          NODE_CHECK_TYPE(node, cJSON_Number)
          doublelval->push_back(node->valuedouble);
        }
    }
  LEAF_NEXT()
  return 0;
}

// string
int PersistenceDom::des(char *cstrval, std::size_t maxlen) {
  LEAF_CHECK(cJSON_String)
    {
      std::size_t len = std::strlen(m_readPos->valuestring);
      std::strncpy(cstrval, m_readPos->valuestring, len > maxlen ? maxlen : len);
    }
  LEAF_NEXT()
  return 0;
}

int PersistenceDom::des(std::string *strval) {
  LEAF_CHECK(cJSON_String)
    {
      strval->assign(m_readPos->valuestring);
    }
  LEAF_NEXT()
  return 0;
}

} // namespace persistence
