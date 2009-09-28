/*
**********************************************
// File:	Cluster.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define an abstract class for use
//		with various data structures and
//		scoring models in the Open Word
//		Enumeration Framework.
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

#include <vector>
#include <string.h>
#include "OWEF_args.h"

#ifndef KKURZ_CLUSTER
#define KKURZ_CLUSTER

using namespace std;

class cluster
{
	public:		
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		cluster();
		
		//standard destructor
		virtual ~cluster()=0;
		
		//************************************************************
		//Accessors
		//All accessors simply return the value of their associated
		//variable.  No modifications allowed.
		//************************************************************
		
		// Gets the next cluster from the list of clusters
		virtual vector<string> get_next_cluster()=0;
		
		// Get total number of clusters
		virtual int get_num_clusters()=0;
		
	protected:
		//pointer to the list of input parameters
		owef_args *list;
};

#endif
