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

#ifndef LISPDATASET_H_
# include "lispdataset.h"
#endif

#ifndef LISPDATASETFUNCTIONS_H_
#define LISPDATASETFUNCTIONS_H_

namespace dsim
{

template<typename T>
  int LispDataset::ser( const T &val )
    {
      using namespace std;

      stringstream ss;
      ss << val;
      *m_outstream << ss.str() << " ";
      return m_outstream->good() ? 0 : -DS_WRITE_FILE;
    }

}

#endif
