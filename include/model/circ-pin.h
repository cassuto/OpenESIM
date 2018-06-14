/*
 *  OpenDSIM (Opensource Circuit Simulator)
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

#ifndef CIRC_PIN_H_
#define CIRC_PIN_H_

#include <dsim/types.h>

typedef struct circ_pin_s
{
  bool connected;
  int index;
} circ_pin_t;

circ_pin_t *circ_pin_create( void );
void circ_pin_free( circ_pin_t *pin );

static inline void
circ_pin_set_index( circ_pin_t *pin, int index )
{
  pin->index = index;
}

#endif //!defined(CIRC_PIN_H_)
