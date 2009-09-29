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

#include "OWEF_args.h"

using namespace std;

//standard constructor
owef_args::owef_args()
{
	//no-op
}

//initialized constructor
owef_args::owef_args(bool count_in, string seq_file_in, int word_length_in, int min_length_in, int min_seqs_in, int min_O_in, bool ancestral_filter_in, bool N_filter_in, bool missing_in, bool enumerate_in, bool record_seqs_in, bool score_in, int order_in, bool revcomp_in, bool pval_in, bool pthr_in, double pthresh_in, bool fam_in, string prefix_in, bool parallel_in, string log_file_in)
{
	//do counting?
	count = count_in;
	//filter for ancestral repeats?
	anc_filter=ancestral_filter_in;
	//minimum wordlength for search (usually set to 1)
	minlength = min_length_in;
	//maximumn wordlength for search (the length of reported motifs)
	maxlength = word_length_in;
	//filter n's out of input sequences?
	n_filter= N_filter_in;
	//list the missing words?
	missing = missing_in;
	//enumerate them?
	enumerate = enumerate_in;
	// record set of sequences?
	record_seqs = record_seqs_in;
	//input sequence filename
	seq_file = seq_file_in;
	//minimum number of sequences in which a word must occur
	min_seqs = min_seqs_in;
	//minimum number of times a word must occur
	min_O = min_O_in;
	//do scoring?
	score = score_in;
	//Markov order for scoring
	order = order_in;
	//do revcomp?
	revcomp = revcomp_in;
	//do pval?
	pval = pval_in;
	//set threshold?
	pthr = pthr_in;
	//threshold value
	pthresh = pthresh_in;
	//Do Familying
	fam = fam_in;
	//prefix for unique job id's
	prefix = prefix_in;
	//run the job in parallel?
	parallel = parallel_in;
	//file to log the filenames
	log_file = log_file_in;
	//total number of words found in the sequences
	for(int i=0; i<maxlength; i++)
		num_words.push_back(0);
	//sequence number being analyzed
	seq = 0;
	//number of characters in the input (total)
	total_input_length = 0;
	//rank for MPI (when used)
	rank = -1;
	//number of MPI tasks (when used)
	numtasks = -1;
	
		
}

//standard destructor
owef_args::~owef_args()
{
	//no-op
}

void owef_args::write_logs()
{
	#ifdef KKURZ_MPI
	if(rank == 0)
	{
		ofstream log(log_file.c_str(), ios::app);
		//do counting?
		log << "Count: " << count << endl;
		//filter for ancestral repeats?
		log << "Ancestral filter: " << anc_filter << endl;
		//minimum wordlength for search (usually set to 1)
		log << "Minimum wordlength: " << minlength << endl;
		//maximumn wordlength for search (the length of reported motifs)
		log << "Maximum wordlength: " << maxlength << endl;
		//filter n's out of input sequences?
		log << "N-filter: " << n_filter << endl;
		//list the missing words?
		log << "Missing words: " << missing << endl;
		//enumerate them?
		log << "Enumerate: " << enumerate << endl;
	  	// record sequences?
	  	log << "Record Set of Seqs: " << record_seqs << endl;
		//input sequence filename
		log << "Input file: " << seq_file << endl;
		//minimum number of sequences in which a word must occur
		log << "Minimum sequences: " << min_seqs << endl;
		//minimum number of times a word must occur
		log << "Minimum occurrences: " << min_O << endl;
		//do scoring?
		log << "Score: " << score  << endl;
		//Markov order for scoring
		log << "Markov order: " << order  << endl;
		//do revcomp?
		log << "Revcomp: " << revcomp << endl;
		//do pval?
		log << "P-values: " << pval << endl;
		//set threshold
		log << "P-thr: " << pthr << endl;
		//threshold value
		log << "P-thresh: " << pthresh << endl;
		//Family Stuff
		log << "Family: " << fam << endl;
		//run the job in parallel?
		log << "Parallel: " << parallel << endl;
		//prefix for unique job id's
		log << "Job ID: " << prefix << endl << endl;
		log.close();
	}
	#endif
	
	#ifndef KKURZ_MPI
	ofstream log(log_file.c_str(), ios::app);
	//do counting?
	log << "Count: " << count << endl;
	//filter for ancestral repeats?
	log << "Ancestral filter: " << anc_filter << endl;
	//minimum wordlength for search (usually set to 1)
	log << "Minimum wordlength: " << minlength << endl;
	//maximumn wordlength for search (the length of reported motifs)
	log << "Maximum wordlength: " << maxlength << endl;
	//filter n's out of input sequences?
	log << "N-filter: " << n_filter << endl;
	//list the missing words?
	log << "Missing words: " << missing << endl;
	//enumerate them?
	log << "Enumerate: " << enumerate << endl;
  	// record sequences?
  	log << "Record Set of Seqs: " << record_seqs << endl;
	//input sequence filename
	log << "Input file: " << seq_file << endl;
	//minimum number of sequences in which a word must occur
	log << "Minimum sequences: " << min_seqs << endl;
	//minimum number of times a word must occur
	log << "Minimum occurrences: " << min_O << endl;
	//do scoring?
	log << "Score: " << score  << endl;
	//Markov order for scoring
	log << "Markov order: " << order  << endl;
	//do revcomp?
	log << "Revcomp: " << revcomp << endl;
	//do pval?
	log << "P-values: " << pval << endl;
	//set threshold
	log << "P-thr: " << pthr << endl;
	//threshold value
	log << "P-thresh: " << pthresh << endl;
	//Family Stuff
	log << "Family: " << fam << endl;
	//run the job in parallel?
	log << "Parallel: " << parallel << endl;
	//prefix for unique job id's
	log << "Job ID: " << prefix << endl << endl;
	log.close();
	#endif
}

