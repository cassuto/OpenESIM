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

#ifndef CIRC_PIN_H_
#define CIRC_PIN_H_

#include <dsim/types.h>
#include <model/logic_signal.h>
#include <dsim/cdecl.h>

C_DECLS

typedef enum
{
  PIN_TYPE_ANALOG = 0,
  PIN_TYPE_DIGITAL
} pin_type_t;

typedef struct circ_pin_s
{
  bool connected;
  int index;
  pin_type_t type;
  struct circ_node_s *node, *node_comp;
} circ_pin_t;

circ_pin_t *circ_pin_create( void );
int circ_pin_set_node( circ_pin_t *pin, struct circ_node_s *node );
int circ_pin_set_nodecomp( circ_pin_t *pin, struct circ_node_s *node );
double circ_pin_get_volt( circ_pin_t *pin );
logic_state_t circ_pin_get_state( circ_pin_t *pin );
void circ_pin_free( circ_pin_t *pin );

static inline void
circ_pin_set_index( circ_pin_t *pin, int index )
{
  pin->index = index;
}
static inline void
circ_pin_set_type( circ_pin_t *pin, pin_type_t type )
{
  pin->type = type;
}

#define circ_pin_set_state(pin, state) \
  ({ \
    int _rc_ = 0; \
    if( pin->connected ) \
      { \
        _rc_ = circ_node_set_state( pin->node, state ); \
      } \
    (_rc_); \
  })

END_C_DECLS

#endif //!defined(CIRC_PIN_H_)
