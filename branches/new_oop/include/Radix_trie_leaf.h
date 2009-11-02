//******************************************************
// File:	Radix_trie_leaf.h
//
// Author:	Kyle Kurz
//
// Date:	10.15.09
//
// Purpose:	Define a radix trie leaf structure
//		for use with the WordSeeker pipeline.
//*****************************************************
/*
Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley
*/

#include "Radix_trie_node.h"

#ifndef KKURZ_RADIX_TRIE_LEAF
#define KKURZ_RADIX_TRIE_LEAF

using namespace rmes;

#define ALPH (Alphabet::alphabet->size()+1)

using namespace std;

//class for a node (has 1 NULL pointer if leaf, possibly 4 NULL pointers if branch)
class radix_trie_leaf : public radix_trie_node
{
	public:
		radix_trie_leaf();
		scores *storage;
};

#endif
