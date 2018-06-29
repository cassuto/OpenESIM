/** @file
 * Lower bound and upper bound
 */

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
#include <math.h>
#include <dsim/misc.h>

#include <dsim/bound.h>


/* find first element not before value, the series must be ordered */
const unsigned long *
  lower_bound(const unsigned long *first, const unsigned long *last, unsigned long value)
{
  const unsigned long *it;
  unsigned long count, step;
  count = abs(last - first);
  while (count > 0)
    {
      it = first; step = count/2; it += step;
      if (*it < value)
        { first = ++it; count -= step+1; }
      else count = step;
    }
  return first;
}

/* find first element not after value, the series must be ordered */
const unsigned long *
  upper_bound (const unsigned long *first, const unsigned long *last, unsigned long value)
{
  const unsigned long *it;
  unsigned long count, step;
  count = abs(last - first);
  while (count>0)
    {
      it = first; step = count/2; it += step;
      if (!(value < *it))
        { first = ++it; count -= step+1;  }
      else count=step;
    }
  return first;
}
