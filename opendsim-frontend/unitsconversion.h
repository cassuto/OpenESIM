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

#ifndef UNITSCONVERSION_H_
#define UNITSCONVERSION_H_

#include <QString>
#include <dsim/scheme-intr.h>

namespace dsim
{

class UnitsConversion
{
public:
  static double toNumber( const QString &num );
  static QString toString( float num );
  static QString toString( double num );
  static QString toString( int num );
  static QString unified( const QString &num );
};

}

#endif
