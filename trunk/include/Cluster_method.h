/*
**********************************************
// File:	Cluster_method.h
//
// Author:	Kyle Kurz
//
// Date:	4.22.09
//
// Purpose:	Define a class for use
//		in clustering in the Open
//		Word Enumeration Framework.
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

 
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <time.h>
#include <vector>

#include "Data_structure.h"
#include "OWEF_args.h"
#include "Scores.h"
#include "Scores_ws.h"
#include "Word_scoring.h"

#ifndef KKURZ_CLUSTER_METHOD
#define KKURZ_CLUSTER_METHOD

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
		//Modifiers
		//************************************************************
		
		virtual void build_clusters(data *structure, word_scoring *model)=0;
		
	protected:
		
		//function to create motif logos for clusters
		void create_logos(data *structure, word_scoring *model);
		//
		double score_cluster(vector<string> cluster, data *structure, word_scoring *model);
		//
		void output_reg_exp(ostream & out_file, const vector<vector<float> > & pwm);
		//
		void output_pwm(ostream & out_file, const vector<vector<float> > & pwm);
		//
		vector<vector<float> > compute_pwm(vector<string> cluster_data, data *structure);
		//
		void create_motif_logo(const vector<vector<float> > & pwm, const string & filename);\
		//
		//bool desc_order(const pair<double, vector <word_data> > & op1, const pair<double, vector <word_data> > & op2);

		//pointer to the list of input parameters
		owef_args *list;
		vector<vector<string> > clusters;
};

#endif
