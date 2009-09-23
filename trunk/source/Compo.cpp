#include "Compo.h"

#include <vector>
#include <string>

Compo::Compo() {
}

vector<string> Compo::Compo_Generate() {
    vector<string> motifs = gff.GenerateGFF();
    
    for (int i = 0; i < motifs.size(); i++) {
	motifs[i] += motifs.seqID;
	motifs[i] += motifs.frequencies;
    }

    return motifs;
}

