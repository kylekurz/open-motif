/*
**********************************************
// File:	Hamming_cluster.h
//
// Author:	Kyle Kurz
//
// Date:	4.22.09
//
// Purpose:	Define an Hamming cluster class
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

#ifndef KKURZ_HAMMING_CLUSTER
#define KKURZ_HAMMING_CLUSTER

using namespace std;

class hamming_cluster : public cluster
{
	public:
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		hamming_cluster();
		
		//initialized constructor
		hamming_cluster(owef_args *from_input, data *structure, word_scoring *model);
		
		//standard destructor
		~hamming_cluster();
		
	private:
		
		//***********************************************************
		//Modifiers
		//***********************************************************
		
		//function to create the clusters from the word list
		void build_clusters(data *structure, word_scoring *model);
		
		//private functions (hamming distance specific)
		int hamming_dist(const string & s1, const string & s2);
		
};

#endif
