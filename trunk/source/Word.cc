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
#include <cmath>
#include <iterator>
#include <memory>
#include <numeric>
using namespace std;

//#include <Alphabet.h>
//#include <Word.h>
#include "Alphabet.h"
#include "Word.h"

namespace rmes {

  Word::Word() : _length(-1), _number(-1)
  {
  }

  Word::Word(const long l) : _length(l), _number(0)
  {
  }

  Word::Word(const long l, const long number) : _length(l), _number(number)
  {
  }


  Word::Word(const string &s) : _length(s.length())
  {
    vector<Base> letters;
    for_each(s.begin(),s.end(),CharToBaseMapper(&letters));
    _number=accumulate(letters.begin(),letters.end(),0,buildNumber);
  }


  void
  Word::setNumber(const long number) 
  {
    _number=number;
  }

  Word::Word(const Word &w) 
  {
    _length=w._length;
    _number=w._number;
  } 

  Word &
  Word::operator=(const Word &w) 
  {
    if (this != &w) {
      _length=w._length;
      _number=w._number;
    }
    return *this;
  }

  Word::~Word()
  {
  }

  long
  Word::length() const
  {
    return _length;
  }

  long
  Word::begin() const
  {
    return 0;
  }

  long
  Word::end() const
  {
    return _length-1;
  }

  long
  Word::substring(const long start, const long end) const
  {
    if (start>end)
      return 0;

    long mask=Alphabet::alphabet->mask();
    for (int index=0;index<end-start;index++)
      mask=(mask<<Alphabet::alphabet->bits())|Alphabet::alphabet->mask();

    return ((_number>>(Alphabet::alphabet->bits()*(_length-1-end)))&mask);

  }

  long
  Word::prefix() const
  {
    return _number>>Alphabet::alphabet->bits();
  }

  long
  Word::suffix() const
  {
    long mask=Alphabet::alphabet->mask();
    mask=mask<<(Alphabet::alphabet->bits()*(_length-1));
    mask=~mask;

    return _number & mask;
  }

  Base 
  Word::operator[](const long index) const
  {
    assert(index>=0);
    assert(index<_length);
    long shiftedNumber=_number>>(Alphabet::alphabet->bits()*(_length-1-index));
    return (Base)(shiftedNumber&Alphabet::alphabet->mask());
  }

  Base 
  Word::first() const
  {
    long shiftedNumber=_number>>(Alphabet::alphabet->bits()*(_length-1));
    return (Base)(shiftedNumber&Alphabet::alphabet->mask());
  }

  Base 
  Word::last() const
  {
    return (Base)(_number&Alphabet::alphabet->mask());
  }

  Word
  Word::conjugate() const
  {
    long mask=0;
    long cnumber=0;
    for (short i=1;i<=_length;i++) {
      cnumber=(cnumber<<(Alphabet::alphabet->bits()))+(*this)[_length-i];
      mask=(mask<<Alphabet::alphabet->bits())|Alphabet::alphabet->mask();
    }

    cnumber=(~cnumber)&mask;
    
    return Word(_length,cnumber);
  }

  long
  Word::number() const
  {
    return _number;
  }


  bool
  Word::isPalindrome() const
  {
    bool palindrome=true;
    for (short i=0;i<_length/2 && palindrome;i++)
      if ((*this)[i] != (*this)[_length-1-i])
	palindrome=false;

    return palindrome;
  }


  Word *
  Word::overlaps(const short d) const
  {
    return overlaps(*this,d);
  }

  Word *
  Word::overlaps(const Word &w, const short d) const
  {
    Word *ow=NULL;

    const Word *leftWord=this;
    const Word *rightWord=&w;
    short absd=d;

    bool overlap=true;

    if (d<0) {
      leftWord=&w;
      rightWord=this;
      absd=-d;
    }

    if (absd < rightWord->length()) {
      
      for (short baseIndex=0; baseIndex < leftWord->length() - absd &&
	     baseIndex < rightWord->length(); baseIndex++)
	if ((*leftWord)[baseIndex+absd] != (*rightWord)[baseIndex]) {
	  overlap=false;
	  break;
	}
      
      if (overlap) {
	short overlapLength=leftWord->length();
	long overlapNumber=leftWord->number();
	if (leftWord->length() - absd < rightWord->length()) {
	  overlapLength=absd+rightWord->length();
	  for (short baseIndex=leftWord->length()-absd;
	       baseIndex< rightWord->length(); baseIndex++)
	    overlapNumber=overlapNumber*Alphabet::alphabet->factor()+(*rightWord)[baseIndex];
	}
	ow=new Word(overlapLength,overlapNumber);
      }
    }
    
    return ow;
  }

  ostream &
  operator<<(ostream &os, const Word &w)
  {
    
    string wordString;
    
    vector<Base> letters;
    letters.resize(w._length);
    for (short i=0; i<w._length;i++) {
      letters[i]=w[i];
    }
    

    for_each(letters.begin(),letters.end(),
	     BaseToStringMapper(&wordString));
    
    return os << wordString;
  }
  
  istream &
  operator>>(istream &is, Word &w)
  {
    
    string wordString;
    vector<Base> letters;

    is >> wordString;
    
    w._length=wordString.length();
    letters.clear();
    for_each(wordString.begin(),wordString.end(),CharToBaseMapper(&letters));
    w._number=accumulate(letters.begin(),letters.end(),0,buildNumber);
    
    return is;
  }
  
  bool
  operator==(const Word &w1, const Word &w2)
  {
    return (w1._number == w2._number && w1._length==w2._length);
  }
  
  bool
  operator!=(const Word &w1, const Word &w2)
  {
    return (w1._number != w2._number || w1._length!=w2._length);
  }

  vector<short>
  Word::princPeriod(const short m){
    short vectLength=0;
    vector<short> result;
    result.resize(this->length()-1);
    fill(result.begin(),result.end(),0);
    vector<short>::iterator it=result.begin();

    for (short d=1; (d < this->length())&&(d < this->length()-m+1); d++) {
      auto_ptr<Word> ow1(this->overlaps(d));
       if (ow1.get()){
	 if (d>1){
	   bool princ=true;
	   for(short l=1;l<d;l++){
	     auto_ptr<Word> ow2(this->overlaps(l));
	     auto_ptr<Word> ow3(this->overlaps(d-l));
	     if(ow2.get() && ow3.get())
	       princ=false;
	   }
	   if(princ==true){
	     *it=d;
	     it++;
	     vectLength++;
	   }
	 }
	 else{*it=1;it++;vectLength++;}
       }    
    }
   
    vector<short> result2;
    result2.resize(vectLength);
    vector<short>::iterator it2=result2.begin();
    vector<short>::iterator it1=result.begin();
    while (it2 !=result2.end()){
      *it2=*it1;
      it1++;it2++;
    }
    return(result2);
  }

 //  double
//   Word::getkstar(){
//       short princPeriodMin=length();
//     vector<short> vect=princPeriod();
//     for(vector<short>::iterator it=vect.begin(); it != vect.end();it++){
//       if(*it<princPeriodMin){princPeriodMin=*it;}
//     }
//     double result=length()/((double) princPeriodMin);
//     return(result);
//   }

};

