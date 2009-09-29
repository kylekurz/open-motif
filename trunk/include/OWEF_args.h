/*
**********************************************
// File:	OWEF_args.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a class for storing Open
//		Word Enumeration Framework
//		input variables.
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
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#ifndef KKURZ_OWEF_ARGS
#define KKURZ_OWEF_ARGS

using namespace std;

class owef_args
{
	public:
				
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		owef_args();
		
		//initialized constructor
		owef_args(bool count, string seq_file, int word_length, int min_length, int min_seqs, int min_O, bool ancestral_filter, bool N_filter, bool missing, bool enumerate, bool record_seqs, bool score, int order, bool revcomp, bool pval, bool pthr, double pthresh, bool fam, string prefix, bool parallel, string log_file);
		
		void write_logs();
		
		//standard destructor
		~owef_args();
		
		//************************************************************
		//Input parameters
		//Make all input parameters available to classes based off
		//the wordseeker class
		//************************************************************
		//do counting stage?
		bool count;
		//filter for ancestral repeats?
		bool anc_filter;
		//minimum wordlength for search (usually set to 1)
		int minlength;
		//maximumn wordlength for search (the length of reported motifs)
		int maxlength;
		//filter n's out of input sequences?
		bool n_filter;
		//list the  missing words?
		bool missing;
		//enumerate them?
		bool enumerate;
    		// record sets of seqs?
    		bool record_seqs;
		//total number of words found in the sequences
		vector<long int> num_words;
		//sequence number being analyzed
		int seq;
		//input sequence filename
		string seq_file;
		//minimum number of sequences in which a word must occur
		int min_seqs;
		//minimum number of times a word must occur
		int min_O;
		//Markov order for scoring
		int order;
		//do scoring?
		bool score;
		//do revcomp?
		bool revcomp;
		//do pval?
		bool pval;
		//set threshold?
		bool pthr;
		//threshold value
		double pthresh;
		//Family Stuff
		bool fam;
		//prefix for unique job id's
		string prefix;
		//run the job with parallel code?
		bool parallel;
		//file to log the filenames of the job
		string log_file;
		//number of characters in the dataset (total)
		long total_input_length;
		//store data about each sequence we find (length)
		vector<int> background_seqs;	
		//rank for MPI (when used)
		int rank;
		//number of MPI tasks (when used)
		int numtasks;
		//boolean for module discovery
		bool discover;
		
		//module discovery variables
		bool discovery;
		bool fixed;
		int dimensions;
		bool self_compare;
		bool distributed;
		bool preserve_order;
		bool density;
		bool overlap_words ;
};
	
#endif
