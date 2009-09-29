/*
**********************************************
// File:        Word_family.h
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
#include "Rmes_model.h"



#ifndef WORD_FAMILY
#define WORD_FAMILY



class word_family
{
  public:
    //constructor
      word_family();
    //initialized constructor
      word_family(owef_args *input_list,data *structure, rmes_model *model);
    //destructor
      ~word_family();

    //Make Family
      string create_family(string w,data *structure, rmes_model *model, int order);

  protected:
    owef_args *list;      
};


#endif


