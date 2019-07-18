/******************************************************************************
 * OpenESIM (Electrical Engineering Simulator)                                *
 * Copyright (C) 2019, ESIM team. <diyer175@hotmail.com>                      *
 *                                                                            *
 * This project is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public License(GPL)    *
 * as published by the Free Software Foundation; either version 2.1           *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This project is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 ******************************************************************************/

#include "schematic.h"
#include "elementarc.h"
#include "elementchord.h"
#include "elementcircle.h"
#include "elementellipse.h"
#include "elementline.h"
#include "elementpin.h"
#include "elementrect.h"
#include "elementtext.h"
#include "elementorigin.h"
#include "errtrace.h"
#include <cstring>
#include <fstream>

namespace schematic {

Element *Schematic::appendElement(const char *classname)
{
  Element *instance = 0l;
  if (std::strcmp(classname, "arc") == 0)
    { instance = new ElementArc(); }
  else if (std::strcmp(classname, "chord") == 0)
    { instance = new ElementChord(); }
  else if (std::strcmp(classname, "circle") == 0)
    { instance = new ElementCircle(); }
  else if (std::strcmp(classname, "ellipse") == 0)
    { instance = new ElementEllipse(); }
  else if (std::strcmp(classname, "line") == 0)
    { instance = new ElementLine(); }
  else if (std::strcmp(classname, "pin") == 0)
    { instance = new ElementPin(); }
  else if (std::strcmp(classname, "rect") == 0)
    { instance = new ElementRect(); }
  else if (std::strcmp(classname, "text") == 0)
    { instance = new ElementText(); }
  else if (std::strcmp(classname, "origin") == 0)
    { instance = new ElementOrigin(); }
  else
    { return 0l; }
  attachElement(instance); // append to element list
  return instance;
}

void Schematic::test()
{
#if 1
  ElementLine *line = static_cast<ElementLine *>(this->appendElement("line"));
  line->locate(locateSetPos, 4 * sizeof(int), 120, 120, 200, 120);
  
  ElementText *text = static_cast<ElementText *>(this->appendElement("text"));
  text->locate(locateSetPos, 2 * sizeof(int), 128, 128);
  text->setText("Hello world 0");
  text->setTextStyle(TextStyle("",templatedColor(0, 0, 0), "Times New Roman", 8, false, false, false));
  text->locate(locateSetRotation, sizeof(int), 0);
  
  text = static_cast<ElementText *>(this->appendElement("text"));
  text->locate(locateSetPos, 2 * sizeof(int),128, 128);
  text->setText("Hello world 90");
  text->setTextStyle(TextStyle("",templatedColor(0, 0, 0), "Times New Roman", 8, false, false, false));
  text->locate(locateSetRotation, sizeof(int), 90);
  
  text = static_cast<ElementText *>(this->appendElement("text"));
  text->locate(locateSetPos, 2 * sizeof(int),128, 128);
  text->setText("Hello world 180");
  text->setTextStyle(TextStyle("",templatedColor(0, 0, 0), "Times New Roman", 8, false, false, false));
  text->locate(locateSetRotation, sizeof(int), 180);
  
  text = static_cast<ElementText *>(this->appendElement("text"));
  text->locate(locateSetPos, 2 * sizeof(int),128, 128);
  text->setText("Hello world 270");
  text->setTextStyle(TextStyle("",templatedColor(0, 0, 0), "Times New Roman", 8, false, false, false));
  text->locate(locateSetRotation, sizeof(int), 270);
  
  ElementRect *rect = static_cast<ElementRect *>(this->appendElement("rect"));
  rect->locate(locateSetPos, 4 * sizeof(int), 760, 300, 500, 500);
  
  ElementEllipse *ellipse = static_cast<ElementEllipse *>(this->appendElement("ellipse"));
  ellipse->locate(locateSetPos, 4 * sizeof(int), 400, 400, 600, 600);
  
  ElementArc *arc = static_cast<ElementArc *>(this->appendElement("arc"));
  arc->locate(locateSetPos, 5 * sizeof(int), 200,200, 10, 30, -270*16);

  ElementChord *chord = static_cast<ElementChord *>(this->appendElement("chord"));
  chord->locate(locateSetPos, 5 * sizeof(int), 300, 300, 0, -270*16);
  
  line = static_cast<ElementLine *>(this->appendElement("line"));
  line->locate(locateSetPos, 4 * sizeof(int), 120, 120, 100, 400);

  ElementPin *pin = static_cast<ElementPin *>(this->appendElement("pin"));
  pin->locate(locateSetPos, 2 * sizeof(int), 100, 400);
  pin->locate(locateSetRotation, sizeof(int), 0);
  
  pin = static_cast<ElementPin *>(this->appendElement("pin"));
  pin->locate(locateSetPos, 2 * sizeof(int), 100, 400);
  pin->locate(locateSetRotation, sizeof(int), 90);
  
  pin = static_cast<ElementPin *>(this->appendElement("pin"));
  pin->locate(locateSetPos, 2 * sizeof(int), 100, 400);
  pin->locate(locateSetRotation, sizeof(int), 180);
  
  pin = static_cast<ElementPin *>(this->appendElement("pin"));
  pin->locate(locateSetPos, 2 * sizeof(int), 100, 400);
  pin->locate(locateSetRotation, sizeof(int), 270);

  std::fstream stream("openesim.sch");
  if (stream.is_open())
    {
      std::cout << "rc = " << this->save(stream) << std::endl;
    }
#endif

#if 1
  std::fstream ostream("openesim.sch");
  if (ostream.is_open())
    {
      std::cout << "rc = " << this->open(ostream) << std::endl;
    }
#endif
}

} // namespace schematic
