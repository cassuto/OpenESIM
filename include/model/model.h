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

#ifndef CIRC_MODEL_H_
#define CIRC_MODEL_H_

#include <dsim/types.h>
#include <dsim/cdecl.h>
#include <dsim/hashmap.h>
#include <model/logic_signal.h>
#include <dsim/cdecl.h>

C_DECLS

typedef struct circ_element_s circ_element_t;
typedef struct circuit_s circuit_t;

typedef int (*pfn_element_create)( circ_element_t *element );
typedef int (*pfn_element_init)( circ_element_t *element );
typedef int (*pfn_element_stamp)( circ_element_t *element );
typedef int (*pfn_element_vchanged)( circ_element_t *element );
typedef int (*pfn_element_event)( circ_element_t *element );
typedef int (*pfn_element_config)( circ_element_t *element, int op, ... );
typedef void (*pfn_element_reset)( circ_element_t *element );
typedef void (*pfn_element_uninit)( circ_element_t *element );
typedef int (*pfn_element_clock)( circ_element_t *element );

typedef enum
{
  MDEL_ANALOG,
  MDEL_DIGITAL,
  MDEL_AD
} mdel_type_t;

typedef struct asim_descriptor_s
{
  pfn_element_create    pfn_create;
  pfn_element_init      pfn_init;
  pfn_element_stamp     pfn_stamp;
  pfn_element_vchanged  pfn_vchanged;
  pfn_element_config    pfn_config;
  pfn_element_reset     pfn_reset;
  pfn_element_uninit    pfn_uninit;
} asim_descriptor_t;

typedef struct dsim_descriptor_s
{
  pfn_element_create    pfn_create;
  pfn_element_init      pfn_init;
  /* pass */ pfn_element_stamp     padding0;
  /* pass */ pfn_element_vchanged  padding1;
  pfn_element_config    pfn_config;
  pfn_element_reset     pfn_reset;
  pfn_element_uninit    pfn_uninit;
  pfn_element_clock     pfn_clock;
  pfn_element_event     pfn_event;
} dsim_descriptor_t;

typedef struct adsim_descriptor_s
{
  pfn_element_create    pfn_create;
  pfn_element_init      pfn_init;
  pfn_element_stamp     pfn_stamp;
  pfn_element_vchanged  pfn_vchanged;
  pfn_element_config    pfn_config;
  pfn_element_reset     pfn_reset;
  pfn_element_uninit    pfn_uninit;
  pfn_element_clock     pfn_clock;
  pfn_element_event     pfn_event;
} adsim_descriptor_t;

typedef struct circ_element_descriptor_s
{
  DS_HASHMAP_NODE();
  size_t                param_size;
  const char           *symbol_name;
  int                   pin_count;
  mdel_type_t           mdel_type;
  void                 *mdel;
} circ_element_descriptor_t;

#define DEFINE_PARAM(varname, elm, type) type *param = circ_element_get_param(elm, type);

#define LIB_FUNC(name) name
#define LIB_INT_FUNC(name) name

/* Get the node of pin that belongs to element */
#define PINNODE(element, index) ((element)->pin_vector[index]->node)

/* Get the volt of pin */
#define GET_VOLT(pin) (circ_pin_get_volt(pin))
/* Get the logic state of pin */
#define GET_STATE(pin) (circ_pin_get_state(pin))

/* configuration opcodes */
enum
{
  ELM_CONFIG_NOP = 0,
  /**
   * Get the number of parameters
   * @param int* out Where to store the result.
   */
  ELM_CONFIG_LIST_COUNT,

  /**
   * Get the prop list of parameter.
   * @param const model_variable_prop_t **prop Where to store the pointer of prop list.
   */
  ELM_CONFIG_LIST_PROP,

  /**
   * Get the value of parameter, which must be a double type.
   * @param int param_id The index of parameter, starts from 0.
   * @param double *out Where to store the result.
   */
  ELM_CONFIG_GET,

  /**
   * Set the value of parameter, which must be a double type.
   * @param int param_id The index of parameter, starts from 0.
   * @param double val The target value.
   */
  ELM_CONFIG_SET
};

typedef enum
{
  MDEL_VAR_UNKNOWN = 0,
  MDEL_VAR_DOUBLE,     /* double */
  MDEL_VAR_FLOAT,      /* float */
  MDEL_VAR_INTEGER,    /* int */
  MDEL_VAR_BOOL,       /* int (NOT bool type variable) */
  MDEL_VAR_STRPTR,     /* const char * */
  MDEL_VAR_VOIDPTR,    /* void * */
  MDEL_VAR_CONST_VOIDPTR /* const void * */
} model_variable_type_t;

typedef struct
{
  const char *symbol;
  const char *name;
  model_variable_type_t type;
} model_variable_prop_t;

int model_create_instance( circ_element_t *element, const char *symbol, const circ_element_descriptor_t DS_OUT **ppdesc, const void DS_OUT **ppmdel );
void model_destroy_instance( const circ_element_descriptor_t *desc, const void *mdel, circ_element_t *element );
void model_log( const char *format, ... );

extern double model_cero_doub;
extern double model_high_imp;
extern double model_logic_vh;
extern double model_logic_vl;
extern double model_logic_vth;
extern double model_logic_high_imp;

typedef enum
{
  MDEL_ERR = 0,
  MDEL_WARNING,
  MDEL_INFO,
  MDEL_TRACE,
  MDEL_DEBUG,
  MDEL_VERBOSE
} mdel_loglevel_t;

#define mdel_logtrace(level, params) do { model_log params ; } while(0)

END_C_DECLS

#endif //!defined(CIRC_MODEL_H_)
