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

#ifndef DSIM_NETLIST_PARSER_H_
#define DSIM_NETLIST_PARSER_H_

#include <dsim/types.h>
#include <dsim/scheme-intr.h>

#define NETLIST_VERSION (1.0f)

typedef struct ds_netlist_parser_s
{
  struct ds_scheme_s *sc;
  bool began;
  ds_scheme_time_t timescale;
  ds_scheme_time_t timescale_pr;
}ds_netlist_parser_t;

ds_netlist_parser_t *netlist_parser_create( struct ds_scheme_s *sc );
netlist_parser_release( ds_netlist_parser_t *nlst );

#endif //!defined(DSIM_NETLIST_PARSER_H_)
