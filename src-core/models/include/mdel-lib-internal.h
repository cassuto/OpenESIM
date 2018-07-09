/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyright (C) 2016, The first Middle School in Yongsheng Lijiang China
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

#ifndef MDEL_LIB_INTERNAL_H_
#define MDEL_LIB_INTERNAL_H_

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include <dsim/types.h>
#include <dsim/error.h>
#include <dsim/memory.h>
#include <dsim/circuit.h>
#include <model/circ-pin.h>
#include <model/circ-node.h>
#include <model/circ-element.h>
#include <model/model.h>

/* model-logic.c */
typedef struct model_logic_s
{
  int inputs_count;
  int outputs_count;

  double input_vh;
  double input_vl;
  double output_vh;
  double output_vl;

  double input_imp;
  double output_imp;

  bool inv_inputs;
  bool inverted;
  bool clock;
  bool out_enable;
  bool in_enable;

  circ_pin_t *pin_clock;
  circ_pin_t *pin_out_enable;
  circ_pin_t *pin_in_enable;

  circ_element_t **out_vsrc;
  circ_element_t **in_vsrc;
  bool           *in_state;
  bool           *out_inverted;
  bool           *in_inverted;
  bool clock_pin_inverted;

  circ_element_t *element;
} model_logic_t;

model_logic_t *model_logic_create( void );
int model_logic_init( model_logic_t *dev_logic );
void logic_reset( model_logic_t *dev_logic );
int logic_create_in_pins( model_logic_t *dev_logic, circuit_t *circuit, int n );
int logic_create_out_pins( model_logic_t *dev_logic, circuit_t *circuit, int n );
bool logic_out_enabled( model_logic_t *dev_logic );
void logic_set_out_enabled( model_logic_t *dev_logic, bool enabled );
bool logic_in_enabled( model_logic_t *dev_logic );
int logic_set_output( model_logic_t *dev_logic, int index, bool state );
int logic_set_output_vh( model_logic_t *dev_logic, double volt );
int logic_set_output_vl( model_logic_t *dev_logic, double volt );
int logic_set_output_imp( model_logic_t *dev_logic, double imp );
void logic_set_output_inverted( model_logic_t *dev_logic, int index, bool inverted );
bool logic_get_input( model_logic_t *dev_logic, int input );
int logic_set_input_vh( model_logic_t *dev_logic, double volt );
int logic_set_input_vl( model_logic_t *dev_logic, double volt );
int logic_set_input_imp( model_logic_t *dev_logic, double imp );
void logic_set_input_inverted( model_logic_t *dev_logic, int index, bool inverted );
logic_state_t logic_get_clk( model_logic_t *dev_logic );
void model_logic_free( model_logic_t *dev_logic );

static inline int
logic_inpin_count( model_logic_t *dev_logic )
{
  return dev_logic->inputs_count;
}

static inline circ_pin_t *
logic_inpin( model_logic_t *dev_logic, int index )
{
  return dev_logic->in_vsrc[index]->pin_vector[0];
}

static inline int
logic_outpin_count( model_logic_t *dev_logic )
{
  return dev_logic->outputs_count;
}

static inline circ_pin_t *
logic_outpin( model_logic_t *dev_logic, int index )
{
  return dev_logic->out_vsrc[index]->pin_vector[0];
}

/*
 * Internal Routines
 */

/* lib-pn.c */
double LIB_INT_FUNC(pn_current)( circ_element_t *element );

#endif //!defined(MDEL_LIB_INTERNAL_H_)

