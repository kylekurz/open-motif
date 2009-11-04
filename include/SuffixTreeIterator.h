/*
 * SuffixTreeIterator.h
 *
 *  Created on: Nov 2, 2009
 *      Author: Lev A Neiman - lev.neiman@gmail.com
 */

#ifndef SUFFIXTREEITERATOR_H_
#define SUFFIXTREEITERATOR_H_

#include "SuffixTree.h"
#include "Ds_iterator.h"



class SuffixTreeIterator: public ds_iterator
{
	public:
		SuffixTreeIterator(){}
		SuffixTreeIterator( SuffixTree * stree, index_type word_length );
		virtual ~SuffixTreeIterator();

		bool hasNext();
		std::string next();

		index_type word_count()
		{
			return string_words.size();
		}

		/**
		 * VIRTUAL METHODz
		 */

		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************

		//function to get word count
		virtual int get_count()
		{
			return -1;
		}
		//function to get sequence count
		virtual int get_seqs()
		{
			return -1;
		}
		//function to get the statistics for a word
		virtual scores* get_stats()
		{
			return NULL;
		}

		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************

		//function to set the statistics of a word in the trie
		virtual int set_stats( scores *new_stats )
		{
			return -1;
		}

		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implemented
		//for each data structure incorporated.
		//************************************************************

		//function to iterate through the words contained in the data structure
		virtual string get_next_word( int length )
		{
			return next();
		}
		//function to get a block of words to process (eliminate contention between threads
		virtual void get_next_word_block( vector<string> &block, int length, int count )
		{

		}

		std::vector<std::string> string_words;

	private:
		SuffixTree * stree;
		std::queue<std::pair<SuffixTreeNode*, index_type> > q;
		std::list<SuffixTreeNode *> curr_path;
		index_type word_length;
		//std::vector<SequenceString*> words;

		//	void populate(SNode * node, std::vector<SequenceString*> & words,  index_type rec);
		void populate_string( SuffixTreeNode * node, std::vector<std::string> & string_words, std::string curr_word, index_type rec );
		index_type counter;

};

#endif /* SUFFIXTREEITERATOR_H_ */
