#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <vector>
#include <cassert>
#include <cstdarg>
#include <cstring>
#include <string>
#include "typedefs.h"
#include "decorator.h"

namespace schematic
{

/**
 * @brief Indicate how to size the element. In form of bitmask.
 *       Each flag could be set simultaneously. Please assure that
 *       there is at least one flag named 'S0_*' set.
 */
enum ElementLocationType {
  S0_POS_2P = 1,       // setPos(x1, y1, x2, y2)
  S0_POS_1P = 1<<2,    // setPos(x1, y1)
  S0_POS_4P = 1<<3,    // setPos(x1, y1, x2, y2, x3, y3, x4, y4)
  S0_POS_1P1R = 1<<4,  // setPos(x1, y1, radius)
  S0_POS_2P1D  = 1<<5,  // setPos(stringX1, stringY1, stringX2, stringY2, distance)
  S1_ROTATION_1A = 1<<6, // setRotation(a1)
};

enum LocateOpcode {
  locateSetPos,
  locateGetPos,
  locateSetArc,
  locateGetArc,
  locateSetRotation,
  locateGetRotation,
};

enum AnchorType
{
  AnchorNone,
  AnchorPoint,
  AnchorCircle,
  AnchorArc,
};

class Schematic;
class RenderDevice;

class Element
{
public:
  Element();
  virtual ~Element();

  /**
   * @brief Persistence interface - class name used for persistence
   */
  virtual const char *classname() const=0;

  /**
   * @brief Persistence interface - serialize the element.
   * Must be called by derived classes.
   * @return status code.
   */
  virtual int serialize(persistence::PersistenceDom *dom);

  /**
   * @brief Persistence interface - deserialize the element.
   * Must be called by derived classes.
   * @return status code.
   */
  virtual int deserialize(persistence::PersistenceDom *dom);

  /**
   * @brief Get location type. see @r ElementLocationType for details.
   */
  virtual int locationType() const=0;

  virtual void locate(int opcode, int paramSize, ...)=0;

  /**
   * @brief Render interface
   */
  virtual void render(RenderDevice *device)=0;

  /**
   * @brief Get bounding rectangle of this element.
   */
  virtual void bounding(int *x1, int *y1, int *x2, int *y2) const=0;

  /**
   * @brief Show attribute configuration dialog
   */
  virtual void config() {}

public:
  /**
   * @brief Get instance of this element, validating classname in DBEUG.
   */
  template <typename T>
    T *instance(const char *classname)
     {
       assert (std::strcmp(this->classname(), classname) == 0);
       return static_cast<T *>(this);
     }

  inline void setSelected(bool selected) { m_selected = selected; }

  inline bool selected() const { return m_selected; }

  inline void setModifiable(bool val) { m_modifiable = val; }

  inline bool isModifiable() const { return m_modifiable; }

  void origin(int *x, int *y);

  void getLocationMsg(std::string *out);

  void moveDelta(int dx, int dy);

  void getAnchors(int *type, int *num);
  void moveAnchor(int type, int point, int cx, int cy, int dx, int dy);
  void renderAnchors(RenderDevice *device, Schematic *document);

protected:
  /*
   * Persistence properties.
   * Write to stream in serialization.
   * Note that all the derived classes should call Element::serialize() and Element::deserialize()
   * when persist to keep consistent of these variables with document stream.
   */

  /*
   * Dynamic properties (no need to be saved in document stream)
   */
  bool m_selected;
  bool m_modifiable;

  friend class Schematic;
};

} // namespace schematic

#ifdef NDEBUG
# define ASSERT_PARAMS_RETURN(exp) do { if (!(exp)) return; } while(0)
#else
# define ASSERT_PARAMS_RETURN(exp) do { assert(exp); } while(0)
#endif

#endif // ELEMENT_H_
