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

#ifndef RMES_ALPHABET_H
#define RMES_ALPHABET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cctype>
#include <map>
#include <string>
#include <vector>

typedef short Base;

namespace rmes {

  class Alphabet {

  public:
    Alphabet(const std::string &);
    Alphabet(const std::vector<std::string> &, const std::string &,
	     const char, const Base =-1);
    inline short size() const;
    inline short bits() const;
    inline long factor() const;
    inline short mask() const;
    inline char joker() const;
    inline Base base(const char);
    inline char character(const Base);
    inline Base interrupt() const;
    inline Base invalid() const;
    long buildNumber(const long, const Base &);
    virtual ~Alphabet();
   

    static Alphabet *alphabet;

  private:

    void build(const std::vector<std::string> &, const std::string &,
	       const char, const Base);

    
    short _size;
    short _bits;
    long _factor;
    short _mask;
    char _joker;
    Base _interrupt;
    Base _invalid;
    std::map<Base,char> _charMap;
    std::map<char,Base> _baseMap;

  };

  inline short 
  Alphabet::size() const
  {
    return _size;
  }

  inline short 
  Alphabet::bits() const
  {
    return _bits;
  }

  inline long 
  Alphabet::factor() const
  {
    return _factor;
  }

  inline short 
  Alphabet::mask() const
  {
    return _mask;
  }

  inline char
  Alphabet::joker() const
  {
    return _joker;
  }

  inline Base
  Alphabet::interrupt() const
  {
    return _interrupt;
  }

  inline Base
  Alphabet::invalid() const
  {
    return _invalid;
  }

  inline char 
  Alphabet::character(const Base b) 
  {
    if (_charMap.find(b) != _charMap.end())
      return _charMap[b];
    else
      return _invalid;
  }

  inline Base 
  Alphabet::base(const char c) 
  {
    if (_baseMap.find(c) != _baseMap.end())
      return _baseMap[c];
    else
      return _invalid;
  }


  class BaseToCharMapper {
  public:
    BaseToCharMapper(std::vector<char> *);
    void operator()(const Base);
  private:
    std::vector<char> *_charVec;
  };

  class CharToBaseMapper {
  public:
    CharToBaseMapper(std::vector<Base> *);
    void operator()(const char);
  private:
    std::vector<Base> *_baseVec;
  };

  class BaseToStringMapper {
  public:
    BaseToStringMapper(std::string *);
    void operator()(const Base);
  private:
    std::string *_string;
  };

  long
  buildNumber(const long, const Base &);

};



#endif
