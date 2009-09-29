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

#ifndef RMES_WORD_H
#define RMES_WORD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <vector>

//#include <Counter.h>
//#include <RMESString.h>
//#include <Alphabet.h>
#include "RMESString.h"
#include "Alphabet.h"

namespace rmes {
  /**
   * Class representing words on which statistical computations can be 
   * performed.
   *
   * Word instances hold number representations of sequences of Base tokens.
   * Each tokens takes AlphabetBits bits in the number representation.
   * Hence, the lenth of representable Word instances is given by :
   * (sizeof(long)*8/AlphabetBits)/2.
   * Each Word is supposed to contain only valid Base tokens (no Z, nor 
   * INVALID tokens).
   * For using the conjugate() method, the complementary Base token of 
   * any Base token must be obtained by taken its bitwise NOT value.
   **/
  class Word : public String {
  public:

    /**
     * Word constructor with length argument. 
     * The Word is initialized with the number 0.
     *
     * \param l the number of Base tokens in this word.
     **/
    Word(const long l);

    /**
     * Word constructor with length and number arguments.
     *
     * \param l the number of Base tokens in this word.
     * \param n the number representation of the sequence of Base tokens.
     **/
    Word(const long l, const long n);

    /**
     * Word constructor from a string of characters.
     *
     * The length of the Word will be the number of characters in the string.
     *
     * \param s string filled with characters corresponding to valid Base
     * tokens.
     **/
    Word(const std::string &);
    Word(const Word &);
    Word & operator=(const Word &);
    virtual ~Word();
    
    /**
     * \return the number of Base tokens in this Word.
     */
    virtual long length() const;

    /**
     * \return the position of the first Base token in this Word.
     **/
    virtual long begin() const;

    /**
     * \return the position of the last Base token in this Word.
     **/
    virtual long end() const;

    /**
     * Returns the number representation of the subword composed of
     * the Base tokens of the interval [s,e].
     *
     * \param s position of the first Base token of the substring.
     * \param e position of the last Base token of the substring.
     * \return the number representation of the subword of this Word
     * located in [s,e].
     **/
    virtual long substring(const long s, const long e) const;

    /**
     * \return the Base token located at position p in this Word.
     **/
    virtual Base operator[](const long p) const; 

    /**
     * Returns the number representation of the subword composed of
     * the length-1 first letters of this word.
     **/
    virtual long prefix() const;


    /**
     * Returns the number representation of the subword composed of the
     * length-1 last letters if this word.
     *
     **/
    virtual long suffix() const;

    /**
     * \return the first Base token of this Word.
     **/
    virtual Base first() const;

    /**
     * \return the last Base tolen of this Word.
     **/
    virtual Base last() const;


    /**
     * \return the number representation of this Word.
     **/
    virtual  long number() const ;

    /**
     * Sets the number representation of this Word to the value
     * given as argument.
     * This method does <b>not</b> change the Word length.
     *
     * \param n the number representation to initialize this Word with.
     **/
    virtual void setNumber(const long n);

    /**
     * Returns de conjugate (complementary) Word of this Word.
     *
     * In order to yield correct results, it is necessary that each Base token
     * has a conjugate (complementary) Base token which is its binary 
     * complement.
     */
    virtual Word conjugate() const ;

    /**
     * \returns true if this Word is a palindrome and false otherwise.
     **/
    virtual bool isPalindrome() const ;

    /**
     * If this Word overlaps itself by a given number of Base tokens,
     * this method returns the Word with the overlapping prefix or suffix.
     * If no overlap is found a NULL pointer is returned.
     *
     *\param d the number of
     * overlapping Base tokens. If d>0, the left overlap (d Base
     * tokens) is prefixed to this Word, if d < 0, the right overlap
     * (d Base tokens) is suffixed to this Word.  \returns the Word
     * prefixed or suffixed with its overlap.
     * \return the overlapping Word or NULL if no overlap is found.
     */
    virtual Word *overlaps(const short d) const;

    /**
     * Performs the same overlap detection as the single argument
     * overlaps() method except that the overlap is not between the
     * word and itself but between this Word instance and the Word
     * instance given a argument.
     *
     * \param w the Word for which an overlap with this Word instance
     * is to be checked.
     * \d the number of overlapping Base tokens.
     * \return the overlapping Word or NULL if no overlap is found.
     **/
    virtual Word *overlaps(const Word & w, const short d) const;

    virtual std::vector<short> princPeriod(const short m);
 
    //virtual double getkstar();

    friend std::ostream & operator<<(std::ostream &, const Word &);
    friend std::istream & operator>>(std::istream &, Word &);
    friend bool operator==(const Word &, const Word &);
    friend bool operator!=(const Word &, const Word &);

  private :
    Word();

    long _length;
    long _number;
  };

};

#endif
