/*
 * SuffixTree.h
 *
 *  Created on: Oct 23, 2009
 *      Author: Lev A Neiman - lev.neiman@gmail.com
 */

#ifndef SUFFIXTREE_H_
#define SUFFIXTREE_H_

#include <vector>
#include <stdio.h>
#include <string>
#include <queue>
#include <stack>
#include <iostream>
#include <list>
#include <map>

#include "Data_structure.h"
//#include "SuffixTreeIterator.h"

//#define USE_CUSTOM_NEW

const bool SUFFIX_TREE_DEBUG_ON=true;

#define d_print(x) std::cout<<#x<<" = "<<(x)<<std::endl;

typedef unsigned int index_type;



inline int SuffixTreeCompOffset( char a )
{
	switch (a)
	{
		case 'A':
			return 0;
			break;
		case 'C':
			return 1;
			break;
		case 'G':
			return 2;
			break;
		case 'T':
			return 3;
			break;
		case '$':
			return 4;
			break;
	}
	return -1;
}

const int NUM_CHAR = 5;
const index_type NOT_LEAF = (index_type) ((unsigned long long) 1 << 32) - 1;

//unsigned long long SNODE_COUNT = 0;

struct SuffixTreeNode
{
		/**
		 * instead of storing string, store indices into text
		 * vector for beginning and end of string that is edge of
		 * this node.
		 */
		index_type str_a, str_b;

		/**
		 * start of suffix.  only makes sense for leaf nodes.
		 */
		index_type t;
		index_type num_leafs;
		SuffixTreeNode ** children;
		SuffixTreeNode()
		{
			init();
		}

		void init()
		{
			//++SNODE_COUNT;
			children = new SuffixTreeNode*[NUM_CHAR];
			num_leafs = NOT_LEAF;
			for (int i = 0; i < 5; ++i)
			{
				children[i] = NULL;
			}

		}

		bool isLeaf()
		{
			/*
			 for (int i = 0; i < NUM_CHAR; ++i)
			 {
			 if (children[i] != NULL)
			 return false;
			 }
			 return true;
			 */
			return this->t != NOT_LEAF;
		}

		index_type leafNodeCount()
		{
			using namespace std;
			if (this->num_leafs != NOT_LEAF)
			{
				return this->num_leafs;
			}
			index_type ret = 0;
			if (isLeaf())
				ret = 1;
			else
			{
				for (int i = 0; i < 5; ++i)
				{
					if (children[i] != NULL)
					{
						ret += children[i]->leafNodeCount();
					}
				}
			}
			this->num_leafs = ret;
			return ret;
		}

		index_type leafNodeCount_iterative()
		{
			using namespace std;
			index_type count = 0;
			queue<SuffixTreeNode*> q;
			q.push( this );
			while (!q.empty())
			{
				SuffixTreeNode * t = q.front();
				q.pop();
				if (t->isLeaf())
					++count;
				else
				{
					for (int i = 0; i < 5; ++i)
					{
						if (t->children[i] != NULL)
							q.push( t->children[i] );
					}
				}
			}
			return count;
		}
};

class SuffixTreeIterator;

class SuffixTree: public data
{
	public:
		SuffixTree();
		SuffixTree(owef_args *list);
		virtual ~SuffixTree();

		void readFile( const char * file_name );
		void buildSTree();
		void genRandom( int length );
		void writeTextToFile( const char * file_name );

		index_type naiveCount( std::string needle );
		index_type count( std::string needle );

		void printNode( SuffixTreeNode * node, int rec );

		//	std::vector<char> text;
		char * text;
		index_type text_size;

		friend class SuffixTreeIterator;

		/**
		 * Virtual classes from data class.
		 */

		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************

		//function to get word count
		virtual int get_count( string motif )
		{
			return count(motif);
		}
		//function to get sequence count
		virtual int get_seqs( string motif )
		{
			return get_count(motif)==0?0:1;
		}
		//function to get the statistics for a word
		virtual scores* get_stats( string motif )
		{
			if( scorez.find(motif)==scorez.end())return NULL;
			return scorez[motif];
		}
		//function to return all words that match a regular expression
		virtual void get_regex_matches( vector<string> &matches, string regex )
		{

		}


		//************************************************************
		//Modifiers
		//All modifiers return their new value by default.  This may
		//be ignored based on developer preference.
		//************************************************************

		//function to set the statistics of a word in the trie
		virtual int set_stats( string motif, scores *new_stats )
		{
			scorez[motif]=new_stats;
			return 1;
		}

		//************************************************************
		//General Purpose Functions
		//May do any number of operations, should still be implemented
		//for each data structure incorporated.
		//************************************************************

		//function to iterate through the words contained in the data structure
		virtual string get_next_word( int length );
		//function to get a block of words to process (eliminate contention between threads
		virtual void get_next_word_block( vector<string> &block, int length, int count )
		{

		}
		//function to output information
		virtual void output()
		{

		}
		//function to reset last_word variables
		virtual void reset()
		{

		}

	private:
		SuffixTreeNode * root;
		void insertSuffix( index_type a );
		SuffixTreeNode * getNode( unsigned long long a, unsigned long long b );
		SuffixTreeNode * NewSNode();

		std::map<int,SuffixTreeIterator*> iterators;
		std::map<std::string,scores*> scorez;
};

#endif /* SUFFIXTREE_H_ */
