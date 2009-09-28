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

#include "DNAAlphabet.h"

#include <string>
using namespace std;

namespace rmes {

  vector<string> DNAAlphabet::NucleotideChars = 
                                          DNAAlphabet::buildNucleotideVector();

  string DNAAlphabet::InterruptChars("RrYyWwSsMmKkHhBbVvDdNn.Zz");

  char DNAAlphabet::JokerChar='n';

  DNAAlphabet::DNAAlphabet() : Alphabet(DNAAlphabet::NucleotideChars,
					DNAAlphabet::InterruptChars,
					DNAAlphabet::JokerChar)
  {
  }

  DNAAlphabet::~DNAAlphabet()
  {
  }
  
  vector<string>
  DNAAlphabet::buildNucleotideVector()
  {
    vector<string> nucvec;
    nucvec.push_back("aA");
    nucvec.push_back("gG");
    nucvec.push_back("cC");
    nucvec.push_back("tT");

    return nucvec;
  }

  Alphabet *Alphabet::alphabet=new DNAAlphabet();
    
};

