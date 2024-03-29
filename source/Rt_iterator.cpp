/*
**********************************************
// File:	Rt_iterator.cpp
//
// Author:	Kyle Kurz
//
// Date:	11.02.09
//
// Purpose:	Define a radix trie iterator class for use
//		with various data structures and
//		scoring models in the Open Word
//		Enumeration Framework.
**********************************************

Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley


This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
			    
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "Rt_iterator.h"
	
//************************************************************
//Constructors/Destructors
//************************************************************
		
//standard constructor
rt_iterator::rt_iterator()
{
	//no-op
	pointer = NULL;
	list = NULL;
	next_branch = 0;
	last_word = "";
	last_loc = NULL;
}

rt_iterator::rt_iterator(owef_args *from_input, data *in_structure, int wordlength)
{
	list = from_input;
	wl = wordlength;
	pointer = (radix_trie *)in_structure;
	next_branch = 0;
	last_word = "";
	last_loc = NULL;
	words_reported = 0;
}
		
//standard destructor
rt_iterator::~rt_iterator()
{
	//no-op
}

//iterator information
bool rt_iterator::has_next()
{
	if(words_reported < list->num_words[wl-1])
		return 1;
	else
		return 0;
}

string rt_iterator::next()
{
	string ret_word = "";
	radix_trie_node *node = NULL;
	//if we haven't returned anything yet...
	if(last_loc == NULL && last_word.compare("") == 0) 
	{
		//if we have a trie here...
		if(pointer->root && pointer->root->branch)	
		{
			node = pointer->root;
			//hunt until we find a word long enough to be the "first"
			while(static_cast<int>(ret_word.length()) < wl) 
			{
				//if we can keep following this branch
				if(node->branch && node->branch[next_branch]) 
				{
					///char x = conversion[next_branch];
					char x = 'A' + reverse_branch[next_branch];  // ljn 10/5/2009
					ret_word += x;
					node = node->branch[next_branch];
					next_branch = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else 
				{
					next_branch++;
					if (next_branch >= ALPH) {
						while (next_branch >= ALPH) {
							char branch_id = ret_word[ret_word.length()-1];
							ret_word = ret_word.substr(0, ret_word.length()-1);
							next_branch = (pointer->locate_branch(branch_id)) + 1;
							node = pointer->root;
							for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
								if(node && node->branch && node->branch[pointer->locate_branch(ret_word[i])]) {
									node = node->branch[pointer->locate_branch(ret_word[i])];
								}
							}
							if(ret_word.length()==0 && next_branch >= ALPH)
							{
								last_loc = NULL;
								last_word.clear();
								last_word = "";
								return "";
							}
						}						
					}
				}
			}
			last_loc = node;
			last_word = ret_word;
			words_reported++;
			return ret_word;
		}
	}
	//otherwise we have something and can start from there...
	else {
		char branch_id = last_word[last_word.length()-1];
		ret_word = last_word.substr(0, last_word.length()-1);
		next_branch = pointer->locate_branch(branch_id) + 1;
		node = pointer->root;
		for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
			if(node && node->branch && node->branch[pointer->locate_branch(ret_word[i])]) {
				node = node->branch[pointer->locate_branch(ret_word[i])];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while(static_cast<int>(ret_word.length()) < wl) 
		{
			//if we can keep following this branch
			if(node && node->branch && node->branch[next_branch]) 
			{
				///char x = conversion[next_branch];
				char x = 'A' + reverse_branch[next_branch];  // ljn 10/5/2009
				ret_word += x;
				node = node->branch[next_branch];
				next_branch = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else 
			{
				next_branch++;
				if (next_branch >= ALPH) {
					char branch_id;
					while (next_branch >= ALPH) {
						branch_id = ret_word[ret_word.length()-1];					
						ret_word = ret_word.substr(0, ret_word.length()-1);
						next_branch = pointer->locate_branch(branch_id) + 1;
						node = pointer->root;
						for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
							if(node && node->branch && node->branch[pointer->locate_branch(ret_word[i])]) {
								node = node->branch[pointer->locate_branch(ret_word[i])];
							}
						}
						if(ret_word.compare("") == 0 && (next_branch >= ALPH || next_branch < 0))
						{
							last_loc = NULL;
							last_word.clear();
							last_word = "";
							return "";
						}
					}	
				}
			}
		}
	}	
	last_loc = node;
	last_word = ret_word;
	words_reported++;
	return ret_word;
}


