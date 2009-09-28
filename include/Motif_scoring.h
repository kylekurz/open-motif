/*
 **********************************************
 // File:	Motif_scoring.h
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

#include "Motif.h"
#include "Scores.h"
#include "OWEF_args.h"

#ifndef JL_MOTIF_SCORING
#define JL_MOTIF_SCORING

using namespace std;

class motif_scoring
{
	public:

		//************************************************************
		//Constructors/Destructors
		//************************************************************
		
		//standard constructor
		motif_scoring();
		
		//standard destructor
		virtual ~motif_scoring()=0;
		
		//function to compute all scores of a word
		virtual void compute_scores(motif &to_compute)=0;
		
		//function to compute the count value of a motif
		virtual int compute_count(motif &to_compute, data *structure)=0;
		
		//function to compute the seqs value of a motif
		virtual int compute_seqs(motif &to_compute, data *structure)=0;
		
	protected:
		//pointer to the list of input parameters
		owef_args *list;
};


#endif



