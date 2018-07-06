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

#ifndef DOM_POOL_H_
#define DOM_POOL_H_

#include <dsim/list.h>

namespace dsim
{

class DomDataset;

class DomPool
{
public:
  DomPool();

  int addDOM( DomDataset *dom );
  void deleteDOMs();

private:
  list_t m_domList; /* voidptr_node */
};

}

#endif //!defined(DOM_POOL_H_)

