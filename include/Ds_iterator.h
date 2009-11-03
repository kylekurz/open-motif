/*
**********************************************
// File:	Ds_iterator.h
//
// Author:	Kyle Kurz
//
// Date:	10.28.09
//
// Purpose:	Define an abstract iterator class for use
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
 
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <omp.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>

#include "Data_structure.h"
#include "OWEF_args.h"

#ifndef KKURZ_DS_ITERATOR
#define KKURZ_DS_ITERATOR

using namespace std;

class ds_iterator
{
	public:		
		
		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		ds_iterator();

		//initialized constructor
		ds_iterator(owef_args *from_input, data *structure, int wordlength);
		
		//standard destructor
		virtual ~ds_iterator()=0;

		//iterator information
		virtual bool has_next()=0;
		virtual string next()=0;

	protected:
		//pointer to the list of input parameters
		owef_args *list;
};

#endif
