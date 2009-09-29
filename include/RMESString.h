/**
    Copyright 2006, 2007, 2008 Sophie Schbath & Mark Hoebeke.
    
    Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
    Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley
    
    This file is part of RMES

    RMES is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    RMES is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with RMES; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**/

#ifndef RMES_STRING_H
#define RMES_STRING_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>

//#include <Alphabet.h>
#include "Alphabet.h"

namespace rmes {

  /**
   * \brief Number based string class.
   *
   * The String class represents strings of Base tokens. It is an abstract
   * class destined to be derived in subclasses like Word or Sequence.
   * 
   **/
  class String {
  public:
    /**
     * \return the number of Base tokens composing this String.
     **/
    virtual long length() const = 0;

    /**
     * \return the position (starting from 0) where the String begins.
     **/
    virtual long begin() const = 0;

    /**
     * \return the position (always < length()) where the String ends.
     **/
    virtual long end() const = 0;

    /**
     * Returns a number representation of a substring of this String.
     * The substring includes Base tokens of the interval [s,e].
     *
     * \param s position of the first Base of the substring (>=0),
     * \param e position of the last Base of the substring ( < length()),
     * \return a number representation of the substring of this String
     * 
     **/
    virtual long substring(const long s, const long e) const = 0;

    /**
     * Returns the Base token location at position p in this String.
     *
     * \param p the position of the Base token to be returned.
     * \return the Base token at position p.
     **/
    virtual Base operator[](const long p) const= 0;

    /**
     * Returns the Base token at the first() position of this String.
     *
     * \return the Base token at the first() position of this String.
     */
    virtual Base first() const = 0;

    /**
     * Returns the Base token at the last() position of this String.
     *
     * \return the Base token at the last() position of this String.
     */
    virtual Base last() const = 0;

    virtual ~String();
  };
};

#endif
