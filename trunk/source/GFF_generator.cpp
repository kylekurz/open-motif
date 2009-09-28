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

