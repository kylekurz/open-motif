/*

 **********************************************
 // File:	Data_structure.cpp
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
#include <fstream>
#include <iostream>
#include <string>
#include <list>

/*
//standard constructor
data::data()
{
	//no-op
}
*/

//standard destructor
data::~data()
{
	//no-op
}

void data::get_seq_file( vector<string> &sequences )
{
	using namespace std;
	ifstream in;
	in.open( list->seq_file.c_str() );
	if (in.fail())
	{
		cerr << "FAILED TO OPEN INPUT FILE" << endl;
		exit( -1 );
	}
	//read in the file, keeping tags separate from the sequences
	while (!in.eof())
	{
		string tag;
		if (in.peek() == '>' || in.peek() == ' ')
		{
			string t;
			getline( in, t );
			continue;
		}
		else if (!in.eof())
		{
			string t;
			while (in.peek() != '>' && !in.eof())
			{
				getline( in, t );
				tag.append( t );
			}
			sequences.push_back( tag );
		}
		tag.clear();
	}
	in.close();
	return;
}
