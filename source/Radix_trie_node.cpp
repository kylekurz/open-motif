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



#include "../include/Radix_trie_node.h"


using namespace std;

radix_trie_node::radix_trie_node()
{
	data = 0;
	num_seq = 0;
	last_seq = 0;
	branch = NULL;
}

radix_trie_node **radix_trie_node::new_pointer_array (void)
{
	radix_trie_node **new_node;
	new_node = (radix_trie_node **) malloc (sizeof (radix_trie_node *)*5);
	return new_node;
}
