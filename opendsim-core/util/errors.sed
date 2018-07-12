## @file
# Util - Get status descriptors from error.h.
#

#
#  OpenDSIM (A/D mixed circuit simulator)
#  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
#
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License(GPL)
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#

# Locate the market
/gen{{/,/}}gen/{

# if (#define) goto defines
/^[[:space:]]*#[[:space:]]*define/b defines

# if (/**) goto description
/\/\*\*/b description

}

# delete the other unused
d
b end


##
# Convert the defines
:defines
s/^[[:space:]]*#[[:space:]]*define[[:space:]]*\([[:alnum:]_]*\)[[:space:]]*\(.*\)[[:space:]]*$/    "\1",\n     \1 }, /
b end

##
# Convert descriptive comments. /** desc */
:description
# a burst. how to do N until end of comment?
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
/\*\//!N
# anything with @{ and @} is skipped
/@[\{\}]/d

# Fix double spaces
s/[[:space:]][[:space:]]/ /g

# Fix \# sequences (doxygen needs them, we don't).
s/\\#/#/g

# insert punctuation.
s/\([^.[:space:]]\)[[:space:]]*\*\//\1. \*\//

# convert /** short. more
s/[[:space:]]*\/\*\*[[:space:]]*/  { NULL, \"/
s/  { NULL, \"\([^.!?"]*[.!?][.!?]*\)/  { \"\1\",\n    \"\1/

# terminate the string
s/[[:space:]]*\*\//\"\,/

# translate empty lines into new-lines (only one, please).
s/[[:space:]]*[[:space:]]\*[[:space:]][[:space:]]*\*[[:space:]][[:space:]]*/\\n/g

# remove asterics.
s/[[:space:]]*[[:space:]]\*[[:space:]][[:space:]]*/ /g
b end


# next expression
:end
