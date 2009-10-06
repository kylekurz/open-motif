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
/*
Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley
*/

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "Alphabet.h"
#include "DNAAlphabet.h"

#ifndef KKURZ_RADIX_TRIE_NODE
#define KKURZ_RADIX_TRIE_NODE

using namespace rmes;

#define ALPH (Alphabet::alphabet->size()+1)

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
