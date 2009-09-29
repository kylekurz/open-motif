/**
    Copyright 2006 Sophie Schbath & Mark Hoebeke.
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

#ifndef RMES_COUNTER_H
#define RMES_COUNTER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>

typedef std::vector<long> longvect;
typedef std::vector<short> shortvect;

#include "RMESString.h"

namespace rmes {
  class Counter {

  public :
    Counter();
    Counter(const short lmin , const short lmax );
    virtual ~Counter();
    
    virtual short minLength() const;
    virtual short maxLength() const;
    
    virtual void countWords(const String &);
    virtual long *wordCounts(const short l);
    virtual long wordCount(const short l, const long w) const;
    virtual long maxCount(const short l) ;
    virtual long minCount(const short l) ;
    virtual long numWords(const short l) const ;
    virtual void countTrains(const String &);

    friend std::ostream & operator<<(std::ostream &, const Counter &);

  protected :
    short _lmin;
    short _lmax;
    mutable std::vector<longvect> _counts;
    std::vector<long> _maxcounts;
    std::vector<long> _mincounts;
  };
};

#endif
