//******************************************************
// File:	Radix_trie_leaf.h
//
// Author:	Kyle Kurz
//
// Date:	10.15.09
//
// Purpose:	Define a radix trie leaf structure
//		for use with the Open Motif framework.
//*****************************************************
/*
Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley
*/



#include "Radix_trie_leaf.h"


using namespace std;

radix_trie_leaf::radix_trie_leaf()
{
	data = 0;
	num_seq = 0;
	last_seq = 0;
	branch = NULL;
	storage = NULL;
}
