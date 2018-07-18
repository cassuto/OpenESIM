
#ifndef ELEMENTBASECAST_HXX_H_
#define ELEMENTBASECAST_HXX_H_

#include <QtWidgets>

namespace dsim
{

class ElementBase;

enum { ElementBaseKey = 1000 };

static ElementBase *elementbase_cast( QGraphicsItem *graphicsItem )
{
  return static_cast<ElementBase *>( (graphicsItem->data(ElementBaseKey).value<void *>()) );
}

template <class T>
  static void elementbase_register_cast( T *element )
    {
      (static_cast<QGraphicsItem *>(element))->setData( ElementBaseKey, QVariant::fromValue( static_cast<void *>(static_cast<ElementBase *>(element)) ) );
    }

}

#endif
