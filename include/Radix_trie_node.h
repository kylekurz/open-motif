//******************************************************
// File:	Radix_Trie_Node.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a radix trie node structure
//		for use with the WordSeeker pipeline.
//*****************************************************

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#ifndef KKURZ_RADIX_TRIE_NODE
#define KKURZ_RADIX_TRIE_NODE

using namespace std;

//class for a node (has 1 NULL pointer if leaf, possibly 4 NULL pointers if branch)
class radix_trie_node
{
	public:
		//standard constructor
		radix_trie_node();
		//function to set branch pointer to something else
		radix_trie_node **new_pointer_array (void);
		//public variables
		int data;
		int num_seq;
		int last_seq;
		radix_trie_node **branch;	// we only need 4 elements: A, C, G, T
								// actually need 5, including 'n'
};

#endif
