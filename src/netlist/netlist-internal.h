/*
 *  OpenDSIM (Opensource Digital Circuit Simulation)
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

#ifndef NETLIST_INTERNAL_H_
#define NETLIST_INTERNAL_H_

#include <dsim/scheme-intr.h>

typedef enum
{
  err_nlst = 0,
  err_nlst_version_mismatch,
  err_nlst_expected_a_time,
  err_nlst_invalid_token
} nlst_err_t;

/* netlist-throw.c */
int netlist_throw( int errcode );

/* netlist-op.c */
int nlst_op_netlist( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out );
int nlst_op_timescale( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out );

#endif //!defined(NETLIST_INTERNAL_H_)
