/**
    Copyright 2006 Sophie Schbath & Mark Hoebeke.

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

#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;

#include "../include/Counter.h"
#include "../include/Alphabet.h"
#include "../include/Word.h"
namespace rmes {

  Counter::Counter()
  {
  
  }
  
  Counter::Counter(const short lmin, const short lmax) : _lmin(lmin),
							 _lmax(lmax)
  {
    _counts.resize(lmax+1);
    _mincounts.resize(lmax+1);
    _maxcounts.resize(lmax+1);
    for (int l=lmin;l<=lmax;l++) {
      _mincounts[l]=-1;
      _maxcounts[l]=-1;
      _counts[l].resize(1<<(l*Alphabet::alphabet->bits()));
    }
  }


  short
  Counter::minLength() const
  {
    return _lmin;
  }

  short
  Counter::maxLength() const
  {
    return _lmax;
  }

  void
  Counter::countWords(const String &s)
  {
    for (short l=_lmin; l<=_lmax;l++) {
      fill(_counts[l].begin(),_counts[l].end(),0);
      for (long start=0;start<=s.length()-l;start++) {
	long wordnumber=s.substring(start,start+l-1);
	if (wordnumber>=0) 
	  _counts[l][wordnumber]++;
      }
    }
    
  }

  long *
  Counter::wordCounts(const short l)
  {
    return &(_counts[l][0]);
  }

  long
  Counter::wordCount(const short l, const long w) const
  {
    
    return _counts[l][w];
  }

  long
  Counter::maxCount(const short l)
  {
    /*
     * Yuck ! max_element returns an iterator on
     * the containter elements. 
     * The real value is obtained by getting its contents with '*'.;
     */
    if (_maxcounts[l]<0)
      _maxcounts[l]=*(max_element(_counts[l].begin(),_counts[l].end()));
    return _maxcounts[l];
  }

  long
  Counter::minCount(const short l)
  {
    /*
     * Yuck ! min_element returns an iterator on
     * the containter elements. 
     * The real value is obtained by getting its contents with '*'.;
     */
    if (_mincounts[l]<0)
      _mincounts[l]=*(min_element(_counts[l].begin(),_counts[l].end()));
    return _mincounts[l];
  }

  long
  Counter::numWords(const short l) const
  {
    return _counts[l].size();
  }

  void
  Counter::countTrains(const String &s)
  {
    
  }

  Counter::~Counter()
  {
  }

  ostream &
  operator<<(ostream &os, const Counter &c)
  {
    return os;
  }
};
