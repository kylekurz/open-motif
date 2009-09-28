/*
**********************************************
// File:	Edit_cluster.h
//
// Author:	Kyle Kurz
//
// Date:	4.22.09
//
// Purpose:	Define an Edit cluster class
**********************************************
Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

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

#include "Cluster_method.h"

#ifndef KKURZ_EDIT_CLUSTER
#define KKURZ_EDIT_CLUSTER

using namespace std;

class edit_cluster : public cluster
{
	public:
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		edit_cluster();
		
		//initialized constructor
		edit_cluster(owef_args *from_input, data *structure, word_scoring *model);
		
		//standard destructor
		~edit_cluster();
		
	private:
		
		//***********************************************************
		//Modifiers
		//***********************************************************
		
		//function to create the clusters from the word list
		void build_clusters(data *structure, word_scoring *model);
		
		//private functions (edit distance specific)
		int edit_dist(const string & s1, const string & s2);
		
		//minimum of 3 numbers
		int min3(int a, int b, int c);
};

#endif
