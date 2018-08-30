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

#ifndef LOGIC_SIGNAL_H_
#define LOGIC_SIGNAL_H_


typedef enum
{
  SIG_LOW = 0,
  SIG_RISE,
  SIG_HIGH,
  SIG_FALL,
  SIG_FLOAT
} logic_state_t;

#define logic_high(state) ( state == SIG_HIGH || state == SIG_RISE )
#define logic_low(state) ( state == SIG_LOW || state == SIG_FALL )
#define logic_reversed(state) \
  ({ \
      logic_state_t _resed; \
      switch( state ) \
      { \
        case SIG_LOW: _resed = SIG_HIGH; break; \
        case SIG_RISE: _resed = SIG_FALL; break; \
        case SIG_HIGH: _resed = SIG_LOW; break; \
        case SIG_FALL: _resed = SIG_RISE; break; \
        case SIG_FLOAT: _resed = SIG_FLOAT; break; \
      } \
      (_resed); \
    })


#endif //!defined(LOGIC_SIGNAL_H_)

