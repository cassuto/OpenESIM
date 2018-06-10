/*
 *  OpenDSIM (Opensource Digital Circuit Simulation)
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

#ifndef DSIM_LIST_H_
#define DSIM_LIST_H_

#include <dsim/magic.h>

typedef struct ds_list_node_s
{
  struct ds_list_node_s *next;
  struct ds_list_node_s *prev;
} ds_list_node_t;

/* must be placed at the beginning of the definition of a structure */
#define DS_LIST_NODE() ds_list_node_t __ds_list0__;
/* Get the list_node pointer safely */
#define DS_LIST_GET(_elem) ((ds_list_node_t*)_elem)

#endif //!defined(DSIM_LIST_H_)
