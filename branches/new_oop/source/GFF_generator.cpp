
/*
**********************************************
// File:        GFF_generator.cpp

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


#include "../include/GFF_generator.h"


using namespace std;

GFFGenerator::GFFGenerator()
{
	//no-op
}

// return a single string instead of vector?
vector<string> GFFGenerator::GenerateGFF() {
    vector<string> gff_file;
/*
    for (int i = 0; i <	motif.num_motifs; i++) {
	gff_file += motif.seqname;
	gff_file += motif.source;
	gff_file += motif.feature;
	gff_file += motif.start;
	gff_file += motif.end;
	gff_file += motif.score;
	gff_file += motif.strand;
	gff_file += motif.frame;
	gff_file += motif.attribute;  // gff version 2
	gff_file += "\n";
    }
*/
    return gff_file;
}

