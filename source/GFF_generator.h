// produce .gff files to be used as input to the Composite Motif Generation
// 9/10/09
// lee nau

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
