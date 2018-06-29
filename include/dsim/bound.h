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
#ifndef DSIM_BOUND_H_
#define DSIM_BOUND_H_

#include <dsim/cdecl.h>

C_DECLS

const unsigned long *lower_bound(const unsigned long *first, const unsigned long *last, unsigned long value);
const unsigned long *upper_bound (const unsigned long *first, const unsigned long *last, unsigned long value);

END_C_DECLS

#endif //!defined(DSIM_BOUND_H_)
