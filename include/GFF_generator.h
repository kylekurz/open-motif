/*
**********************************************
// File:        GFF_generator.h
// Purpose:     produce .gff files to be used as input to the Composite Motif Generation
// Author:	Lee Nau
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

#ifndef GFF_GENERATOR
#define GFF_GENERATOR

#include <string>
#include <vector>

using namespace std;

class GFFGenerator {
    public:
	GFFGenerator();
	vector<string> GenerateGFF();

    private:
	vector<string> motifs; // set of input motifs to be stored in gff 
	vector<string> gff;  // output string
};

#endif // GFF_GENERATOR
