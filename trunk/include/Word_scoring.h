/*
**********************************************
// File:	Word_scoring.h
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define an abstract class for use
//		with various word scoring models
//		in the Open Word Enumeration
//		Framework
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
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

#include "Data_structure.h"
#include "Scores.h"
#include "OWEF_args.h"
//#include "Word.h"

#ifndef KKURZ_WORD_SCORING
#define KKURZ_WORD_SCORING

using namespace std;

class word_scoring
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		word_scoring();
		
		//standard destructor
		virtual ~word_scoring()=0;
		
		//function to compute all scores of a word
		virtual void compute_scores(scores *word, string &motif, data *structure, int &order)=0;
		virtual double quantile(const double p)=0;
		//funtion to provide clustering with seeds
		virtual void get_seeds(vector<pair<string, scores> > &seeds)=0;
		
	protected:
		//pointer to the list of input parameters
		owef_args *list;
		vector<pair<string, scores> > top_words[3];
};

#endif
