/*
**********************************************
// File:	Rt_iterator.h
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

#include "Ds_iterator.h"
#include "Radix_trie.h"

#ifndef KKURZ_RT_ITERATOR
#define KKURZ_RT_ITERATOR

using namespace std;

class rt_iterator : public ds_iterator
{
	public:		
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		rt_iterator();

		//initialized constructor
		rt_iterator(owef_args *from_input, data *structure, int wordlength);
		
		//standard destructor
		~rt_iterator();

		//iterator information
		bool has_next();
		string next();

	protected:
		radix_trie *pointer;
		int wl, next_branch, words_reported;
		string last_word;
		radix_trie_node *last_loc;
		
};

#endif
