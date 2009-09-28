/*
**********************************************
// File:	Module_discovery.h
//
// Author:	Kyle Kurz and Xiaoyu Liang
//
// Date:	7.20.09
//
// Purpose:	Define an abstract class for use
//		with various module discovery models
//		in the Open Word Enumeration
//		Framework
**********************************************

Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau, Xiaoyu Liang

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
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "Data_structure.h"
#include "Scores.h"
#include "OWEF_args.h"
#include "Word_scoring.h"

#ifndef KKURZ_MODULE_DISCOVERY
#define KKURZ_MODULE_DISCOVERY

class module_discovery
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		module_discovery();
		
		//standard destructor
		virtual ~module_discovery()=0;
		
		virtual void report_modules(data *structure, word_scoring *model)=0;
		virtual void find_position(vector<string> &seqs, vector<pair<string, scores> > &words, vector<vector<vector<int> > > &positions)=0;
		
	protected:
		//pointer to the list of input parameters
		owef_args *list;
};

#endif
