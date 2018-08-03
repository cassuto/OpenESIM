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

#include <frontend/error.h>
#include "lispdataset.h"
#include "templatestyle.h"
#include "templatecustom.h"

namespace dsim
{

TemplateCustom::TemplateCustom()
              : line( LINE_SOLID ), brush( BRUSH_NONE ), width( 1.0 ), size( 0 ), bold( false )
              , italic( false )
{
  color.r = color.g = color.b =
  bkcolor.r = bkcolor.g = bkcolor.b = -1;
  usebkcolor = true;

  setCustomAll( false );
}

int TemplateCustom::serialize( LispDataset *dataset )
{
  int rc = 0;

  rc = dataset->beginNode(true, "cust");                    UPDATE_RC(rc);

  if( lineCustom )
    {
      rc = dataset->beginNode(true, "ln");                  UPDATE_RC(rc);
      rc = dataset->ser( int(line) );                       UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( brushCustom )
    {
      rc = dataset->beginNode(true, "br");                  UPDATE_RC(rc);
      rc = dataset->ser( int(brush) );                      UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( widthCustom )
    {
      rc = dataset->beginNode(true, "wh");                  UPDATE_RC(rc);
      rc = dataset->ser( width );                           UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( colorCustom )
    {
      rc = dataset->beginNode(true, "cl");                  UPDATE_RC(rc);
      rc = dataset->ser( color.r );                         UPDATE_RC(rc);
      rc = dataset->ser( color.g );                         UPDATE_RC(rc);
      rc = dataset->ser( color.b );                         UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( usebkcolorCustom )
    {
      rc = dataset->beginNode(true, "ub");                  UPDATE_RC(rc);
      rc = dataset->ser( usebkcolor );                      UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( bkcolorCustom )
    {
      rc = dataset->beginNode(true, "bk");                  UPDATE_RC(rc);
      rc = dataset->ser( bkcolor.r );                       UPDATE_RC(rc);
      rc = dataset->ser( bkcolor.g );                       UPDATE_RC(rc);
      rc = dataset->ser( bkcolor.b );                       UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( sizeCustom )
    {
      rc = dataset->beginNode(true, "si");                  UPDATE_RC(rc);
      rc = dataset->ser( size );                            UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( boldCustom )
    {
      rc = dataset->beginNode(true, "bd");                  UPDATE_RC(rc);
      rc = dataset->ser( bold );                            UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }
  if( italicCustom )
    {
      rc = dataset->beginNode(true, "it");                  UPDATE_RC(rc);
      rc = dataset->ser( italic );                          UPDATE_RC(rc);
      rc = dataset->endNode(true);                          UPDATE_RC(rc);
    }

  rc = dataset->endNode(true);                              UPDATE_RC(rc);
  return rc;
}

int TemplateCustom::deserialize( LispDataset *dataset )
{
  std::string symbol;
  int i;
  DomEntry entry;

  int rc = dataset->des( entry, "cust" );                   UPDATE_RC(rc);

  for( entry.begin(); entry.valid(); ++entry )
    {
      dataset->deserializePush( entry.child() );
        {
          LispDataset::AutoPop autoPop( dataset );

          rc = dataset->des( symbol, /*symbol*/true );      UPDATE_RC(rc);

          if( 0==symbol.compare( "ln" ) )
            {
              rc = dataset->des( i );                       UPDATE_RC(rc);
              line = (LineStyle)i;
              lineCustom = true;
            }
          else if( 0==symbol.compare( "br" ) )
            {
              rc = dataset->des( i );                       UPDATE_RC(rc);
              brush = (BrushStyle)i;
              brushCustom = true;
            }
          else if( 0==symbol.compare( "wh" ) )
            {
              rc = dataset->des( width );                   UPDATE_RC(rc);
              widthCustom = true;
            }
          else if( 0==symbol.compare( "cl" ) )
            {
              rc = dataset->des( color.r );                 UPDATE_RC(rc);
              rc = dataset->des( color.g );                 UPDATE_RC(rc);
              rc = dataset->des( color.b );                 UPDATE_RC(rc);
              colorCustom = true;
            }
          else if( 0==symbol.compare( "ub" ) )
            {
              rc = dataset->des( usebkcolor );              UPDATE_RC(rc);
              usebkcolorCustom = true;
            }
          else if( 0==symbol.compare( "bk" ) )
            {
              rc = dataset->des( bkcolor.r );                UPDATE_RC(rc);
              rc = dataset->des( bkcolor.g );                UPDATE_RC(rc);
              rc = dataset->des( bkcolor.b );                UPDATE_RC(rc);
              bkcolorCustom = true;
            }
          else if( 0==symbol.compare( "si" ) )
            {
              rc = dataset->des( size );                    UPDATE_RC(rc);
              sizeCustom = true;
            }
          else if( 0==symbol.compare( "bd" ) )
            {
              rc = dataset->des( bold );                    UPDATE_RC(rc);
              boldCustom = true;
            }
          else if( 0==symbol.compare( "it" ) )
            {
              rc = dataset->des( italic );                  UPDATE_RC(rc);
              italicCustom = true;
            }
          else
            {
              return -DS_INVALID_CUSTOM_TEMPLATE;
            }
        } // auto pop
    }
  return 0;
}

void TemplateCustom::setCustomAll( bool custom )
{
  lineCustom = brushCustom = widthCustom = colorCustom = usebkcolorCustom = bkcolorCustom =
  sizeCustom = boldCustom = italicCustom = custom;
}

bool TemplateCustom::custom() const
{
  return lineCustom || brushCustom || widthCustom || colorCustom || usebkcolorCustom || bkcolorCustom ||
      sizeCustom || boldCustom || italicCustom;
}

void TemplateCustom::apply( StyleItem *style ) const
{
  if( lineCustom )
    {
      style->line = line;
    }
  if( brushCustom )
    {
      style->brush = brush;
    }
  if( widthCustom )
    {
      style->width = width;
    }
  if( colorCustom )
    {
      style->color = color;
    }
  if( usebkcolorCustom )
    {
      style->usebkcolor = usebkcolor;
    }
  if( bkcolorCustom )
    {
      style->bkcolor = bkcolor;
    }
  if( sizeCustom )
    {
      style->size = size;
    }
  if( boldCustom )
    {
      style->bold = bold;
    }
  if( italicCustom )
    {
      style->italic = italic;
    }
}

}
