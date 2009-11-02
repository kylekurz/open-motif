/*
 **********************************************
 // File:	Motif.h
 //
 // Author:	Jens Lichtenberg, Rami Alouran
 //
 // Date:	09.10.09
 //
 // Purpose:	Define an abstract class for use
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

#include "Data_structure.h"
#include "Scores.h"
#include "OWEF_args.h"

#include <vector>

#ifndef JL_MOTIF
#define JL_MOTIF

using namespace std;

class motif
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		motif();
		
		//standard destructor
		virtual ~motif()=0;
		
		//function to compute the count value of a motif
		virtual vector<string> get_elements_of_motif()=0;
		
		//function to compute the seqs value of a motif
		virtual vector<vector<double> > get_pwm()=0;
		
	protected:
		
		vector <scores*> words;
		
		vector<vector<double> > pwm;
		
		//pointer to the list of input parameters
		owef_args *list;
};

#endif
