//**********************************************
// File:	Radix_trie.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a radix trie class for use
//		with the WordSeeker pipeline.
//**********************************************
/*
Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley
*/

#ifdef KKURZ_MPI
#include <mpi.h>
#endif

#include "Alphabet.h"
#include "Data_structure.h"
#include "DNAAlphabet.h"
#include "Radix_trie_leaf.h"

#ifndef KKURZ_RADIX_TRIE
#define KKURZ_RADIX_TRIE

using namespace rmes;

#define ALPH (Alphabet::alphabet->size()+1)

using namespace std;

class radix_trie : public data
{
	friend class rt_iterator;

	public:
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		radix_trie();
		
		//empty constructor
		radix_trie(owef_args *from_input, int x);
		
		//initialized constructor
		radix_trie(owef_args *from_input);
		
		//standard destructor
		~radix_trie();
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************
		
		//function to get word count
		int get_count(string motif);
		//function to get sequence count
		int get_seqs(string motif);
		//function to get the statistics stored in a leaf node from the trie
		scores* get_stats(string motif);
		//function to return all words that match a regular expression
		void get_regex_matches(vector<string> &matches, string regex);
		//function to get all of the sequences from the input file
		void get_seq_file(vector<string> &sequences);
		//function to get a block of words
		void get_next_word_block(vector<string> &block, int length, int count);
		//function to iterate through the words contained in the data structure
		string get_next_word(int length);
		//function to iterate through the words contained in the data structure from a temporary root
		string get_next_word(radix_trie_node *temp_root, int length);
		//function to output information
		void output();
		//function to reset last_word variables
		void reset();
		//function to reset last_iterator variables
		void reset_iterator(int thread);
		
		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.
		//************************************************************
		//function to set count of a string
		int set_count(string motif, int count);
		//function to set the count in a trie member
		int trie_set(radix_trie_node *node, char *s, int length, int count);
		//function to increment count
		int inc_count(string motif);	
		//function to set the statistics of a word in the trie
		int set_stats(string motif, scores *new_stats);
		//function to set the statistics of a word in the trie
		int trie_stats(radix_trie_node *node, char *s, int length, scores *new_stats);	
		
	private:
		
		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implmented
		//for each data structure incorporated.
		//************************************************************
		
		
		//function to increment seqs
		int inc_seqs(string motif);
		//function to count all the words (called from the constructor)
		void count_words();
		//function to mutate a string to the next physical permutation
		void mutate(string &previous_word);
			
		//************************************************************
		//Radix Trie functions
		//These functions are specific to this implementation of the
		//WordSeeker data structure
		//************************************************************
		
		//add a node to the trie
		void trie_add (radix_trie_node * &node, char *s, int length, int level);

		//get the information about what is stored at a node
		int trie_get (radix_trie_node * &node, char *s, int length, int level);
		
		//get the stats from a leaf
		scores* trie_find_stats (radix_trie_node * &node, char *s, int length);

		//find a word in the trie
		int trie_find (radix_trie_node * &node, char *s, int length);

		//function to get the sequence count of a word in the radix trie (no increment)
		int trie_find_s (radix_trie_node * &node, char *s, int length);

		//delete the trie
		void trie_clear (radix_trie_node * root);

		//output to a file so we can use it in the next step
		//void output(ofstream &ofile, radix_trie_node *ptr, string word, int length);

		//when missing words and enumerate are true, enumerate all words that include 'X' character
		void enumerate_string(ofstream &ofile, vector<string> words);

		//search for missing words in the dataset
		void miss_search(radix_trie_node *root, ofstream &ofile, int length_rem, string word);
		
		//function to locate the appropriate branch from a character
		int locate_branch(char x);
		
		//convert strings to uppercase
		//string convert(string check);
		//string unconvert(string check);
		string to_upper(const string & word );
		
		//functions to get the thread information from the system, regardless of whether we have OMP or not
		int thread_num();
		int num_procs();
				
		//***********************************************************
		//Radix Trie variables
		//These variables are specific to this implementation of the
		//WordSeeker data structure
		//***********************************************************
		
		//root of a trie. used to access all of the words.
		radix_trie_node *root;
		
		//queue for use during get_next_word search
		vector<vector<string> > breadth_search;
		
		//store the last reported word
		vector<string> last_word;
		vector<radix_trie_node *> last_loc;
		vector<vector<string> > last_ext;
		vector<vector<radix_trie_node *> > last_ext_loc;
		int next_branch;
		vector<int> next_branch_iterator;	
		bool flag;
};

#endif
