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

//#define USE_CUSTOM_NEW

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

unsigned long long SNODE_COUNT = 0;

struct SNode
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
		SNode ** children;
		SNode()
		{
			init();
		}

		void init()
		{
			++SNODE_COUNT;
			children = new SNode*[NUM_CHAR];
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
			queue<SNode*> q;
			q.push( this );
			while (!q.empty())
			{
				SNode * t = q.front();
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

class SuffixTree
{
	public:
		SuffixTree();
		virtual ~SuffixTree();

		void readFile( const char * file_name );
		void buildSTree();
		void genRandom( int length );
		void writeTextToFile( const char * file_name );

		index_type naiveCount( std::string needle );
		index_type count( std::string needle );

		void printNode( SNode * node, int rec );

		//	std::vector<char> text;
		char * text;
		index_type text_size;

		friend class SuffixTreeIterator;

	private:
		SNode * root;

		void insertSuffix( index_type a );
		SNode * getNode( unsigned long long a, unsigned long long b );

		SNode * NewSNode();
};



#endif /* SUFFIXTREE_H_ */
