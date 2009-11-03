/*
 * FASTAFile.cpp
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen, lnau
 */

#include "../include/FASTAFile.h"
#include "../include/SequenceString.h"

FASTAFile::FASTAFile(const char * file_name)
{
	using namespace std;

	cout << "Reading FASTA file: " << file_name << "... ";

    const unsigned int LINE_SIZE = 1024;
    char line[LINE_SIZE];
	string header;
	//string seq;
	SequenceString *seq = new SequenceString; // TODO: move memory alloc 

	ifstream fin(file_name);
    if (!fin) {
		cout << "Error: Unable to open FASTA file\n";
		exit(1);  // terminate process for now - replace later with Exception
    } else {
		cout << "File successfully opened\n";
    }

    // TODO: put this in while-loop so that multiple seqs can be read
    fin.getline(line, LINE_SIZE);  // first eat header
	header = std::string(line);
    while (fin.getline(line, LINE_SIZE)) {
		*seq += line;
    }

    // TODO: make me more efficient!  // convert entire string to uppercase
    for (uint32_t i = 0; i < seq->size(); i++) {  
		(*seq)[i] = toupper((*seq)[i]);
    }

	FASTASequence *faseq1 = new FASTASequence(seq, header);

	sequences.push_back(faseq1);

    ///seqs += '$';  // TODO: add terminating character! 
    ///num_seqs = 1;  // TODO: make this variable!
    fin.close();  // close the input file
}

FASTAFile::~FASTAFile()
{
	// TODO Auto-generated destructor stub
}

FASTASequence *FASTAFile::getSeq(int i)
{
	return this->sequences[i];
}

unsigned int FASTAFile::getNumSeqs()
{
	return this->sequences.size();
}




