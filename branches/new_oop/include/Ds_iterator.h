/*
**********************************************
// File:	Ds_iterator.h
//
// Author:	Kyle Kurz
//
// Date:	10.28.09
//
// Purpose:	Define an abstract iterator class for use
//		with various data structures and
//		scoring models in the Open Word
//		Enumeration Framework.
**********************************************

Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
			    
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
 
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>

#include "OWEF_args.h"

#ifndef KKURZ_DS_ITERATOR
#define KKURZ_DS_ITERATOR

using namespace std;

class ds_iterator
{
	public:		
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		ds_iterator();
		
		//standard destructor
		virtual ~ds_iterator()=0;
		
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************
		
		//function to get word count
		virtual int get_count()=0;
		//function to get sequence count
		virtual int get_seqs()=0;
		//function to get the statistics for a word
		virtual scores* get_stats()=0;
    
		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************

		//function to set the statistics of a word in the trie
		virtual int set_stats(scores *new_stats)=0;
		
		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implemented
		//for each data structure incorporated.
		//************************************************************
		
		//function to iterate through the words contained in the data structure
		virtual string get_next_word(int length)=0;
		//function to get a block of words to process (eliminate contention between threads
		virtual void get_next_word_block(vector<string> &block, int length, int count)=0;
	protected:
		//pointer to the list of input parameters
		owef_args *list;
};

#endif
