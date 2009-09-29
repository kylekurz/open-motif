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
		//function to compute the E value of a motif
		//virtual double compute_E(string &motif, data *structure, int &order)=0;
		//function to compute the Es value of a motif
		//virtual double compute_Es(string &motif, data *structure, int &order)=0;
		//function to compute the Oln value of a motif
		//virtual double compute_Oln(string &motif, data *structure, int &order)=0;
		//function to compute the Sln value of a motif
		//virtual double compute_Sln(string &motif, data *structure, int &order)=0;
		//function to compute the pval value of a motif
		//virtual double compute_pval(string &motif, data *structure)=0;
		//function to compute the rank value of a motif
		//virtual int compute_rank(string &motif, data *structure)=0;
		//function to compute the count value of a motif
		//virtual int compute_count(string &motif, data *structure)=0;
		//function to compute the seqs value of a motif
		//virtual int compute_seqs(string &motif, data *structure)=0;
		//funtion to provide clustering with seeds
		virtual vector<pair<string, scores> > get_seeds()=0;
		
	protected:
		//pointer to the list of input parameters
		owef_args *list;
		vector<pair<string, scores> > top_words[3];
};

#endif
