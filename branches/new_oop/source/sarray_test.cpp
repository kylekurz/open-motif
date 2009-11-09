// Lee Nau, new test main for suffix array

#include <iostream>
#include <string>
#include <ctime>
#include <stdint.h>

#include "Data_structure.cpp"
#include "SuffixArray.cpp"
#include "divsufsort.cpp"

using namespace std;

int main(int argc, char *argv[])
{
	const char *file_name = "bin/fakeDNA.fasta";
	string word = "A";
    const uint8_t word_len = 2;
    //const uint8_t markov_order = 6;

	SuffixArray SA(file_name);

	SA.printArray();

	for (uint32_t i = 0; i < 10; i++) {
		cout << "next word: " << SA.get_next_word(word_len) << endl;
	}

	cout << "Word: " << word << " occurs: " << SA.get_count(word) << " times\n";

	for (uint32_t i = 0; i < SA.arraySize(); i++) {
		cout << "# Words of Length: " << i << "\t" << SA.numWords(i) << endl;
	}

    return 0;
}

