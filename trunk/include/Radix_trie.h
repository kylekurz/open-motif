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

#include "Alphabet.h"
#include "Data_structure.h"
#include "DNAAlphabet.h"
#include "Radix_trie_node.h"

#ifdef KKURZ_MPI
#include <mpi.h>
#endif

#ifndef KKURZ_RADIX_TRIE
#define KKURZ_RADIX_TRIE

using namespace rmes;

#define ALPH (Alphabet::alphabet->size()+1)

using namespace std;

class radix_trie : public data
{
	public:
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		radix_trie();
		
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
		//function to return all words that match a regular expression
		vector<string> get_regex_matches(string regex);
		//function to get all of the sequences from the input file
		vector<string> get_seq_file();
		//function to iterate through the words contained in the data structure
		string get_next_word(int length);
		//function to iterate through the words contained in the data structure from a temporary root
		string get_next_word(radix_trie_node *temp_root, int length);
		//function to iterate through the entire subtree from a given word
		string get_next_extension(string seed, int length);
		//function to output information
		void output();
		//function to reset last_word variables
		void reset();
		
		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.
		//************************************************************
				
		
	private:
		
		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implmented
		//for each data structure incorporated.
		//************************************************************
		
		//function to increment count
		int inc_count(string motif);
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
		int trie_get (radix_trie_node * &node, char *s, int length);

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
		vector<string> last_ext;
		vector<radix_trie_node *> last_ext_loc;
		int next_branch;	
		bool flag;
};

#endif
