#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include <iostream>
#include <vector>

namespace persistence
{

/* The cJSON structure: */
class cJSON {
public:
  cJSON() :
    next(0l), prev(0l),
    child(0l),
    type(0),
    valuestring(0l),
    valueint(0),
    valuedouble(0),
    string(0l)
  {
  }
  cJSON(int type, char *valuestring, int valueint, double valuedouble) :
    next(0l), prev(0l),
    child(0l),
    type(type),
    valuestring(valuestring),
    valueint(valueint),
    valuedouble(valuedouble),
    string(0l)
  {
  }

  class cJSON *next,*prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
  class cJSON *child;		/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

  int type;					/* The type of the item, as above. */

  char *valuestring;			/* The item's string, if type==cJSON_String */
  int valueint;				/* The item's number, if type==cJSON_Number */
  double valuedouble;			/* The item's number, if type==cJSON_Number */

  char *string;				/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
};

class PersistenceDom : public cJSON {
public:
  PersistenceDom(const char *className);
  ~PersistenceDom();

public:
  int toStream(std::ostream &stream);
  int fromString(std::string &string);
  const std::string &getClassName() { return m_className; }

protected:
  inline void addLeaf(cJSON *leaf);

public:
  int ser(int intval);
  int ser(const std::vector<int> & intvals);
  int ser(bool boolval);
  int ser(const std::vector<bool> & boolvals);
  int ser(float floatval);
  int ser(const std::vector<float> & floatvals);
  int ser(double doublelval);
  int ser(const std::vector<double> & doublelvals);
  int ser(const char *cstrval);
  int ser(const std::string & strval);

public:
  int des(int *intval);
  int des(std::vector<int> *intvals);
  int des(unsigned char *ucharval);
  int des(std::vector<unsigned char> *ucharvals);
  int des(bool *boolval);
  int des(std::vector<bool> *boolvals);
  int des(float *floatval);
  int des(std::vector<float> *floatvals);
  int des(double *doublelval);
  int des(std::vector<double> *doublelvals);
  int des(char *cstrval, std::size_t maxlen);
  int des(std::string *strval);

private:
  std::string m_className;
  cJSON *m_root;
  cJSON *m_readPos;
};

} // namespace persistence

#endif // PERSISTENCE_H
