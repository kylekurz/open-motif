// Lee Nau, new test main

#include <iostream>
#include <string>
#include <ctime>
#include <stdint.h>
#include "../include/FASTAFile.h"

int main(int argc, char *argv[])
{
    time_t time;  
    //const std::string filename = "Ecoli_Genome.fasta";
	const char *file_name = "../bin/Ecoli_Genome.fasta";
    const std::string jobname = std::string(file_name) + " " + ctime(&time); 
    std::cout << jobname << std::endl;

    const uint8_t min_word_len = 6;
    const uint8_t max_word_len = 8;
    const uint8_t markov_order = 4;

    FASTAFile fafile(file_name); 

	std::cout << "Number of sequences in file: " << fafile.getNumSeqs() 
			  << std::endl;

	std::cout << "Header: " << fafile.getSeq(0)->getHeader() << std::endl;
	std::cout << "Sequence: " << *fafile.getSeq(0)->getSeqStr() << std::endl;

    return 0;
}

