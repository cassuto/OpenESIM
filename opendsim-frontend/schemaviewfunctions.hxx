/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef SCHEMAVIEW_H_
# include "schemaview.h"
#endif

#ifndef SCHEMAVIEWFUNCTIONS_H_
#define SCHEMAVIEWFUNCTIONS_H_

#include "elementbase.h"

namespace dsim
{

// T must be a subclass of ElementGraphItem<>
template <typename T>
  bool SchemaView::keyPressRotate( QKeyEvent *event, T *element )
  {
    if( event->key() == Qt::Key_Space )
      {
        if( element )
          {
            if( m_hintDirect < ELEM_BOTTOM )
              ++m_hintDirect;
            else
              m_hintDirect = 0;
            element->setDirect( (ElemDirect)m_hintDirect );
            return false;
          }
      }
    return true;
  }

}

#endif
