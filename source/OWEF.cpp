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

Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

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

#include "../include/OWEF.h"

#define NUM_THREADS 4

using namespace std;

extern "C" void *ThreadStartup2(void *);

//structure for passing to parallel processor
typedef struct args{
	int thr_id;
	owef *job;
}args;

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
		structure = new radix_trie(list);
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
	//are we doing module discovery?
	//start = clock();
	/*if(list->discover)
	{
		style = new module_enumeration(list, structure, model);
	}
	end = clock();
	duration = (double)(end-start)/CLOCKS_PER_SEC;
	printf("Module enumeration finished in: %f seconds\n", duration);
	//do the rest in parallel
	for(t_num=0; t_num<NUM_THREADS; t_num++)
	{
		//cout << "creating thread: " << t_num << endl;
		//thread_seqs[t_num] = 0;
		args *p = new args;
		p->thr_id = t_num;
		//p->thread = prefix[t_num];
		//p->sequence = tag;
		p->job = this;
		pthread_create(&threads[t_num], NULL, ThreadStartup2, (void *)p);
	}
	for(t_num=0; t_num<4; t_num++)
		pthread_join(threads[t_num], NULL);
	 */
}
		
//standard destructor		
owef::~owef()
{
	delete structure;
	delete model;
	//delete method;
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

void *owef::process(void *foo)
{
	/*clock_t start, end;
	double duration;
	args *p = (args *)foo;
	if(p->thr_id == 1)
	{
		//are we clustering?
		start = clock();
		if(list->cluster)
		{
			if(list->type == 'e')
				method = new edit_cluster(list, structure, model);
			if(list->type == 'h')
				method = new hamming_cluster(list, structure, model);
		}
		end = clock();
		duration = (double)(end-start)/CLOCKS_PER_SEC;
		printf("Word Clustering finished in: %f seconds\n", duration);
	}
	else if(p->thr_id == 2)
	{
		//are we doing word distribution?
		start = clock();
		if(list->word_distribution)
		{
			type = new word_distribution(list, structure, model);
		}
		end = clock();
		duration = (double)(end-start)/CLOCKS_PER_SEC;
		printf("Word distribution finished in: %f seconds\n", duration);
	}
	else if(p->thr_id == 3)
	{
		//are we doing cooccurrence analysis?
		start = clock();
		if(list->cooccur)
		{
			analysis = new cooccurrence(list, structure, model);
		}
		end = clock();
		duration = (double)(end-start)/CLOCKS_PER_SEC;
		printf("Cooccurrence analysis finished in: %f seconds\n", duration);
	}
	else if(p->thr_id == 4)
	{
		start = clock();
		if(list->sequence_clustering)
		{
			tool = new sequence_clustering(list, structure, model);
		}
		end = clock();
		duration = (double)(end-start)/CLOCKS_PER_SEC;
		printf("Sequence clustering finished in: %f seconds\n", duration);
	}*/
	return NULL;
}

void *ThreadStartup2(void *_Object)
{
	args *p = (args *)_Object;
	return p->job->process(_Object);
}
