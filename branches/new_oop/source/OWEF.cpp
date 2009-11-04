/*
**********************************************
// File:	OWEF.cpp
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

#include "OWEF.h"


using namespace std;

//standard constructor
owef::owef()
{
	//no-op
}

//initialized constructor
owef::owef(owef_args *from_input)
{
	list = from_input;
	//int t_num;
	//pthread_t threads[NUM_THREADS];
	//are we counting?
	clock_t start, end;
	double duration;
	start = clock();
	if(list->count)
	{
		//structure = new radix_trie(list);
		structure = new SuffixTree(list);
	}
	end = clock();
	duration = (double)(end-start)/CLOCKS_PER_SEC;
	printf("Enumeration finished in: %f seconds\n", duration);
	//are we scoring?
	start = clock();
	if(list->score){
		cout<<"Scoring rmes"<<endl;
		model = new rmes_model(list, structure);
		
  	}
	end = clock();
	duration = (double)(end-start)/CLOCKS_PER_SEC;
	printf("Scoring finished in: %f seconds\n", duration);
	start = clock();
	if(list->fam)
	{
		cout<<"Word Familying"<<endl;
		family = new word_family(list, structure, model);
		
  	}
	end = clock();
	#ifdef _OPENMP
	double nprocs = (double)omp_get_num_procs();
	#else
	double nprocs = 1;
	#endif
	duration = (double)(end-start)/CLOCKS_PER_SEC;
	duration = duration/nprocs;
	printf("Familying finished in: %f seconds\n", duration);
}
		
//standard destructor		
owef::~owef()
{
	delete structure;
	delete model;
	delete style;
}

//************************************************************
//Accessors
//All accessors simply return the value of their associated
//variable.  No modifications allowed.
//************************************************************

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
string owef::get_next_word(int length)
{
	return (structure->get_next_word(length));
}

//output all the information
void owef::output()
{
	structure->output();
}
