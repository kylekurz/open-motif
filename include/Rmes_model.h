/*
**********************************************
// File:        Rmes_model.h
// Purpose:     Define a class for calculating scoring based on RMES model .
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



#include <algorithm>
#include <cstdlib>  
#include <fstream>  
#include <iostream> 
#include <memory>
#include <pthread.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h>   
#include <vector>  

#include "Counter.h"
#include "Word.h"
#include "Data_structure.h"
#include "OWEF_args.h"
#include "Scores.h"
#include "Word_scoring.h"



#ifndef RMES_MODEL
#define RMES_MODEL



class rmes_model :  public word_scoring 
{
	public:
	    	//constructor
	    	rmes_model();
	    	//boring constructor
	    	rmes_model(owef_args *input_list, data *structure, int x);
	    	//initialized constructor
	      	rmes_model(owef_args *input_list,data *structure);
	    	//destructor
      		~rmes_model();
    		//compute the ratio of (N-E)/sqrt(v)
      		double compute_ratio(double expect,double variance, int word_count);
      
    
		//condAsExpect
		double  condAsExpect(const rmes::Word  &w, const short m, data *structure);
		//condAsVar with data*
		double condAsVar(const rmes::Word &w, const short m, data *structure);
		//condAsVar   using rmes_counter derived class
		double condAsVar(const rmes::Word &w, const short m, const rmes::Counter &c);   
		//function to compute all scores for a motif
		void compute_scores(scores *word, string &motif, data *structure, int &order);
		//function to compute the E value of a motif
		double compute_E(string &motif, data *structure, int &order);
		//function to compute the Es value of a motif
		double compute_Es(string &motif, data *structure, int &order);
		//function to compute the Oln value of a motif
     		double compute_Oln(string &motif, data *structure, int &order);
		//function to compute the Sln value of a motif
     		double compute_Sln(string &motif, data *structure, int &order);
		//function to compute the pval value of a motif
     		double compute_pval(string &motif, data *structure);
		//function to compute the rank value of a motif  
     		int compute_rank(string &motif, data *structure); 
		//function to compute the count value of a motif  
     		int compute_count(string &motif, data *structure);
		//function to compute the seqs value of a motif  
		int compute_seqs(string &motif, data *structure);                                                    
		//function to give clustering access to seeds
     		vector<pair<string, scores> > get_seeds();  
  
  
  	protected:
    		owef_args *list;      
};


#endif


