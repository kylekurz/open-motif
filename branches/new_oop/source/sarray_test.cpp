// Lee Nau, new test main for suffix array

#include <iostream>
#include <string>
#include <ctime>
#include <stdint.h>

//#include "Data_structure.cpp"
//#include "SuffixArray.cpp"
//#include "divsufsort.cpp"

#include "../include/SuffixArray.h"

using namespace std;

int main(int argc, char *argv[])
{
	const char *file_name = "bin/fakeDNA.fasta";
	//const char *file_name = "bin/Ecoli_Genome.fasta";
    const uint8_t word_length = 8;
    ///const uint8_t markov_order = 6;

	SuffixArray SA(file_name);

	SA.printArray();

	string word = "AC";
	cout << "Word: " << word << " occurs: " << SA.get_count(word) << " times\n";

	for (uint32_t i = 0; i < word_length /*SA.arraySize()*/; i++) {
		cout << "# Words of Length: " << i << "\t" << SA.numWords(i) << endl;
	}

	// enumerate words of a certain length
	for (uint32_t i = 0; i < SA.numWords(word_length); i++) {
		cout << "next word: " << SA.get_next_word(word_length) << endl;
	}

    return 0;
}

