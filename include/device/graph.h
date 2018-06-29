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

#ifndef CIRC_GRAPH_H_
#define CIRC_GRAPH_H_

#include <dsim/types.h>

#if !defined(__cplusplus)
#error cplusplus only
#endif

typedef uint32_t graph_color;

class IGraph
{
public:
  IGraph();
  virtual ~IGraph() {}

  virtual int init(int x, int y, float scale);
  virtual void mapAbsolutePos(int, int, int&, int&);
  virtual void point(int, int);
  virtual void line (int, int, int, int);
  virtual void rect (int, int, int, int);
  virtual void rectD(int, int, int, int);
  virtual void roundRect(int, int, int, int);
  virtual void ellipse(int, int, int, int);
  virtual void arc(int, int, int, int, int, int);
  virtual int  text(const char*, int, int, int *Height=0);
  virtual int  textMapped(const char*, int, int, int *Height=0);
  virtual void fillRect(int, int, int, int, const graph_color&);
  virtual void eraseRect(int, int, int, int);
  virtual void drawResizeRect(int, int);
};

#endif //!defined(CIRC_GRAPH_H_)
