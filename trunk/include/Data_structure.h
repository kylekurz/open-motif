/*
**********************************************
// File:	Data_structure.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define an abstract class for use
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

#include "Scores.h"
#include "OWEF_args.h"

#ifndef KKURZ_DATA_STRUCTURE
#define KKURZ_DATA_STRUCTURE

using namespace std;

//change this array as we add characters to the supported alphabet
static const int branch_array[26] = {0,4,1,4,4,4,2,4,4,4,4,4,4,4,4,4,4,4,4,3,4,4,4,4,4,4};
static const int reverse_branch[26] = {0,2,6,19,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13};

class data
{
	public:		
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		data();
		
		//standard destructor
		virtual ~data()=0;
		
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************
		
		//function to get word count
		virtual int get_count(string motif)=0;
		//function to get sequence count
		virtual int get_seqs(string motif)=0;
		//function to get the statistics for a word
		virtual scores* get_stats(string motif)=0;
		//function to return all words that match a regular expression
		virtual vector<string> get_regex_matches(string regex)=0;
		//function to get total count of a regex
		//virtual pair<string, int> get_regex_counts(string motif)=0;
		//function to get the sequences from the input file
		virtual vector<string> get_seq_file()=0;
    
		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************

		//function to set the statistics of a word in the trie
		virtual int set_stats(string motif, scores *new_stats)=0;
		
		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implemented
		//for each data structure incorporated.
		//************************************************************
		
		//function to iterate through the words contained in the data structure
		virtual string get_next_word(int length)=0;
		//function to get a block of words to process (eliminate contention between threads
		virtual vector<string> get_next_word_block(int length, int count)=0;
		//function to output information
		virtual void output()=0;
		//function to reset last_word variables
		virtual void reset()=0;
	protected:
		//pointer to the list of input parameters
		owef_args *list;
};

#endif
