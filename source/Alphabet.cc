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

//#include <Alphabet.h>
#include "Alphabet.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <string>
using namespace std;

#ifndef HAVE_LOG2
extern "C" {
  double log2(double);
}
#endif

namespace rmes {

  
  Alphabet::Alphabet(const string &alphabetString)
  {
    unsigned int  pos=alphabetString.find_first_of(':');
    string characters;
    string interruptchars;
    char joker='\0';
    if (pos != string::npos && pos >0) {
      characters=alphabetString.substr(0,pos);
      unsigned int pos2=alphabetString.find(':',pos+1);
      if (pos2 != string::npos && pos2>pos) {
      	interruptchars=alphabetString.substr(pos+1,(pos2-pos-1));
	if (pos2+1 < alphabetString.length())
	  joker=alphabetString[pos2+1];
      }
      else
	interruptchars=alphabetString.substr(pos+1);
    } else
      characters=alphabetString;

    vector<string> validchars;
    for (string::const_iterator it=characters.begin();
	 it != characters.end();it++)
      validchars.push_back(string(&(*it),1));

    build(validchars,interruptchars,joker,-1);
    
  }

  Alphabet::Alphabet(const vector<string> &validchars,
		     const string &interruptchars,
		     const char joker,
		     const Base invalid)
  {
    build(validchars,interruptchars,joker,invalid);
  }

  void
  Alphabet::build(const vector<string> &validchars,
		  const string &interruptchars,
		  const char joker,
		  const Base invalid)
  {
    _joker=joker;
    _invalid=invalid;
    short baseIndex=0;
    for (vector<string>::const_iterator it1=validchars.begin();
	 it1 != validchars.end(); it1++) {
      _charMap[baseIndex]=(*it1)[0];
      for (string::const_iterator it2=it1->begin();
	   it2 != it1->end(); it2++) {
	_baseMap[*it2]=baseIndex;
      }
      baseIndex++;
    }
    _size=baseIndex;
    _bits=int(ceil(log2(_size)));
    _factor=1<<_bits;
    _mask=_factor-1;

    for (string::const_iterator it=interruptchars.begin();
	 it != interruptchars.end(); it++)
      _baseMap[*it]=baseIndex;
    _interrupt=baseIndex;
  }

  Alphabet::~Alphabet()
  {
  }


  CharToBaseMapper::CharToBaseMapper(vector<Base> *baseVec) :
    _baseVec(baseVec)
  {
  }

  void
  CharToBaseMapper::operator()(const char c)
  {
    _baseVec->push_back(Alphabet::alphabet->base(c));
  }

  BaseToCharMapper::BaseToCharMapper(vector<char> *charVec) :
    _charVec(charVec)
  {
  }
  
  void
  BaseToCharMapper::operator()(const Base b)
  {
    _charVec->push_back(Alphabet::alphabet->character(b));

  }

  BaseToStringMapper::BaseToStringMapper(string *s) : _string(s)
  {
  }

  void
  BaseToStringMapper::operator()(const Base b)
  {
    _string->push_back(Alphabet::alphabet->character(b));
  }

  long
  buildNumber(const long n, const Base &b)
  {
    if (b == Alphabet::alphabet->invalid() || b == Alphabet::alphabet->interrupt())
      cerr << "Ooops !" <<b<<":: "<<n*Alphabet::alphabet->factor()+(int)b<< endl;
    return (n*Alphabet::alphabet->factor()+(int)b);
  }
};

