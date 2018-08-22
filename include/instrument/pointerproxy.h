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

#ifndef POINTERPROXY_H_
#define POINTERPROXY_H_

template <class T>
  class PointerProxy
  {
  public:
    explicit PointerProxy( T *ptr ) : m_ptr( ptr ) {}
    PointerProxy( const PointerProxy &dev ) : m_ptr( dev.m_ptr ) {}

    inline void set( T *ptr )
      { m_ptr = ptr; }
    inline bool valid()
      { return m_ptr; }
    inline void erase()
      { m_ptr = 0l; }
    inline T *get() const
      { return m_ptr; }

  private:
    T *m_ptr;
  };

#endif
