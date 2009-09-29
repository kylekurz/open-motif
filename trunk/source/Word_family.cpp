
/*
**********************************************
// File:        Word_family.cpp
// Purpose:     The one with all the word families.
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
GNU General Public License for more details.You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Word_family.h"

#include <iostream>

//create_family(string w);

using namespace std;
using namespace rmes;

word_family::word_family()
{
	//no op
}

word_family::~word_family()
{
	//no op
}

word_family::word_family(owef_args *input_list,data *structure, rmes_model *model)
{
	cout<<"Creating Word Families"<<endl;
	list=input_list;
	
	int num_files = (list->maxlength - list->minlength)+1;
	//cout << "num files: " << num_files << endl;
	for(int i=0; i<num_files; i++)
	{
		cout<<"list num:"<<list->num_words[i+list->minlength-1]<<endl;
  	
  	for(int j=0; j<list->num_words[i+list->minlength-1]; j++)
		{
			cout << i+list->minlength << endl;
	    string next_word = structure->get_next_word(i+list->minlength);
	    cout<<"next word"<<next_word<<endl;
	    for (int i=1; i<static_cast<int> (next_word.length()-1); i++) 
			{
				char x = next_word[i];
				next_word[i] = 'N';
				
	    	create_family(next_word,structure, model, list->order);
	    	
	    	next_word[i] = x;
	    }
  	}
  }
}


string word_family::create_family(string w,data *structure, rmes_model *model, int order) 
{
	if(w.length() > 2)
	{
		cout << "Word " << w << endl;
		//w = "ANA";
		
		vector<string> temp = structure->get_regex_matches(w);
		cout << "size " << temp.size() << endl;
		for(int p=0; p < static_cast<int>(temp.size()); p++)
		{
			cout << "Exploded Word: " << temp[p] << " Expected: " << model->condAsExpect(temp[p], order, structure) << endl;
		}
  } 	
	return "AAA";
}











