
#ifndef ELEMENTBASECAST_HXX_H_
#define ELEMENTBASECAST_HXX_H_

#include <QtWidgets>
#include <cstring>

namespace dsim
{

class ElementBase;

enum { ElementBaseKey = 1000 };

static inline ElementBase *elementbase_cast( QGraphicsItem *graphicsItem )
{
  return static_cast<ElementBase *>( (graphicsItem->data(ElementBaseKey).value<void *>()) );
}

template <class T>
  static void elementbase_register_cast( T *element )
    {
      (static_cast<QGraphicsItem *>(element))->setData( ElementBaseKey, QVariant::fromValue( static_cast<void *>(static_cast<ElementBase *>(element)) ) );
    }

template <class T>
  static inline T element_cast( ElementBase *base )
  { return 0l; }

/*
 * The following cast functions may be quite expensive but worth using for the case that
 * we don't sure the type of a pointer to ElementBase and try to convert.
 * When type mismatching the cast will return a null pointer.
 */
#define DECLARE_ELEMENT_CAST(_type, _classname) \
  template <> \
    inline _type *element_cast( ElementBase *base ) \
      { \
        if( 0l==base ) return 0l; \
        if( 0==std::strcmp( base->classname(), _classname ) ) \
          return static_cast<_type *>(base); \
        else \
          return 0l; \
      }

}

#endif
