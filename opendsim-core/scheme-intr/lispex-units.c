/** @file
 * Lisp ext. - Units Conversion
 */

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

#include <math.h>
#include "lisp-internal.h"

static double coeffs[] =
  {
    1.0,        /* SCHEME_UNIT_ONE */
    1.00e-15,   /* SCHEME_UNIT_F */
    1.00e-12,   /* SCHEME_UNIT_P */
    1.00e-09,   /* SCHEME_UNIT_N */
    1.00e-06,   /* SCHEME_UNIT_U */
    1.00e-03,   /* SCHEME_UNIT_MIL */
    1.00e+03,   /* SCHEME_UNIT_K */
    1.00e+06,   /* SCHEME_UNIT_MEG */
    1.00e+09,   /* SCHEME_UNIT_G */
    1.00e+12,   /* SCHEME_UNIT_T */
    1.00e+15,   /* SCHEME_UNIT_P */
    1.00e+18,   /* SCHEME_UNIT_E */
    1.00e+21,   /* SCHEME_UNIT_Z */
    1.00e+24,   /* SCHEME_UNIT_Y */
  };

double
lispex_convert_unit( ds_scheme_unit_t unit, double src )
{
  if( (int)unit >= SCHEME_UNIT_ONE && (int)unit <= SCHEME_UNIT_Y )
    {
      return src * coeffs[(int)unit];
    }
  else if( unit == SCHEME_UNIT_DB )
    {
      return 20 * log10( src );
    }
  else
    return src;
}

const char *
lispex_unit_disp( ds_scheme_unit_t unit )
{
  switch( unit )
  {
    case SCHEME_UNIT_ONE:   return "";
    case SCHEME_UNIT_F:     return "f";
    case SCHEME_UNIT_P:     return "p";
    case SCHEME_UNIT_N:     return "n";
    case SCHEME_UNIT_U:     return "u";
    case SCHEME_UNIT_MIL:   return "m";
    case SCHEME_UNIT_K:     return "k";
    case SCHEME_UNIT_MEG:   return "M";
    case SCHEME_UNIT_G:     return "G";
    case SCHEME_UNIT_T:     return "T";
    case SCHEME_UNIT_PETA:  return "P";
    case SCHEME_UNIT_E:     return "E";
    case SCHEME_UNIT_Z:     return "Z";
    case SCHEME_UNIT_Y:     return "Y";
    case SCHEME_UNIT_DB:    return "dB";
  }
  return "?";
}

double lispex_unit_coeff( ds_scheme_unit_t unit )
{
  if( (int)unit >= SCHEME_UNIT_ONE && (int)unit <= SCHEME_UNIT_T )
    {
      return coeffs[(int)unit];
    }
  else
    return 1.0;
}
