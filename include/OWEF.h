/*
**********************************************
// File:	OWEF.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a class for use
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

#ifdef KKURZ_MPI
#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>
#endif
 
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
#include <time.h>
#include <vector>

#include "Data_structure.h"
#include "Ds_iterator.h"
#include "Module_discovery.h"
#include "OWEF_args.h"
#include "Radix_trie.h"
#include "Rmes_model.h"
#include "Rt_iterator.h"
#include "Scores.h"
#include "Word_family.h"
#include "Word_scoring.h"




#ifndef KKURZ_OWEF
#define KKURZ_OWEF

using namespace std;

class owef
{
	public:
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		owef();
		
		//initialized constructor
		owef(owef_args *from_input);
		
		//standard destructor
		~owef();
		
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.  If the data structure
		//does not store the information, throw an error with (-1)
		//************************************************************
		
		//function to get word count
		int get_count(string &motif);
		//function to get sequence count
		int get_seqs(string &motif);
		
		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************
		
		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implmented
		//for each data structure incorporated.
		//************************************************************
		
		//function to iterate through the words contained in the data structure
		string get_next_word(int length);
		
		//output all of the information currently stored about the input
		void output();
		
	private:
		//cluster *method;
		data *structure;
		module_discovery *style;
		owef_args *list;
		word_scoring *model;
		word_family *family;
};

#endif
