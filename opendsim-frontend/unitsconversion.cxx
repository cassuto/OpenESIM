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

#include <cmath>
#include "unitsconversion.h"

namespace dsim
{

double UnitsConversion::toNumber( const QString &n )
{
  QString num( n.trimmed() );
  int unitOffset = -1;
  int length = num.length();
  double val;
  ds_scheme_unit_t unt = SCHEME_UNIT_ONE;

  for( int i=0; i<length; i++ )
    {
      QChar ch = num.at( i );
      if( ch.isLetter() && ch.toLower().toLatin1() != 'e' ) { unitOffset = i; break; }
    }
  if( unitOffset == 0 ) return 0;
  if( unitOffset > 0 )
    {
      QString unit( num.right( length - unitOffset ).trimmed() );
      switch( unit.at(0).toLatin1() )
      {
        case 'F': case 'f': unt = SCHEME_UNIT_F; break;
        case 'P': case 'p': unt = SCHEME_UNIT_P; break;
        case 'N': case 'n': unt = SCHEME_UNIT_N; break;
        case 'U': case 'u': unt = SCHEME_UNIT_U; break;
        case 'm':           unt = SCHEME_UNIT_MIL; break;
        case 'K': case 'k': unt = SCHEME_UNIT_K; break;
        case 'M':           unt = SCHEME_UNIT_MEG; break;
        case 'G': case 'g': unt = SCHEME_UNIT_G; break;
        case 'T': case 't': unt = SCHEME_UNIT_T; break;
        case 'D': case 'd':
          {
            if( unit.length()<2 ) break;
            switch( unit.at(1).toLatin1() )
            {
              case 'B':
              case 'b':
                unt = SCHEME_UNIT_DB; break;
              default:
                unt = SCHEME_UNIT_ONE;
            }
            break;
          }
        default:
          unt = SCHEME_UNIT_ONE;
      }
    }

  val = num.left( unitOffset ).trimmed().toDouble();

  return lispex_convert_unit( unt, val );
}

template <class numT>
  inline QString toStringInner( numT num )
    {
      int level = num > 0 ? (int)log10( num )+1 : (int)log10( num );

      ds_scheme_unit_t unit = SCHEME_UNIT_ONE; // select an unit automatically
      if( level >= 12 )
        unit = SCHEME_UNIT_T;
      else if( level >= 9 )
        unit = SCHEME_UNIT_G;
      else if( level >= 6 )
        unit = SCHEME_UNIT_MEG;
      else if( level >= 3 )
        unit = SCHEME_UNIT_K;
      else if( level >= 0 )
        unit = SCHEME_UNIT_ONE;
      else if( level >= -3 )
        unit = SCHEME_UNIT_MIL;
      else if( level >= -6 )
        unit = SCHEME_UNIT_U;
      else if( level >= -9 )
        unit = SCHEME_UNIT_N;
      else if( level >= -12 )
        unit = SCHEME_UNIT_P;
      else /*if( level >= -15 )*/
        unit = SCHEME_UNIT_F;

      num /= lispex_unit_coeff( unit );

      return QString( "%1%2" ).arg( num ).arg( lispex_unit_disp( unit ) );
    }

QString UnitsConversion::unified( const QString &num )
{ return toString( toNumber( num ) ); }

QString UnitsConversion::toString( float num )
{ return toStringInner( double( num ) ); }

QString UnitsConversion::toString( double num )
{ return toStringInner( num ); }

QString UnitsConversion::toString( int num )
{ return toStringInner( num ); }

}
