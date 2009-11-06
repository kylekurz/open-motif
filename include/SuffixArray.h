/* SuffixArray.h
 * Migrated to open-motif: Friday, Nov 6, 2009
 * Lee Nau (lee.nau@gmail.com)
 * OU Bioinformatics
 */

#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H

#include <iostream>
#include <map>
#include <cstdlib>   // for exit()
#include <stdint.h>  // for C99 types
#include <cstring>

#include "divsufsort.h"
#include "Data_structure.h"

// Suffix Array types - refine?
typedef int32_t saint_t;
typedef int32_t saidx_t;
typedef uint8_t sauchar_t;

class SuffixArray : public data {
    public:
		// constructors
		SuffixArray(const char *file_name);
		SuffixArray(owef_args *list);
		virtual ~SuffixArray();

		// accessors
		virtual string get_next_word(int length);
		virtual void get_next_word_block(vector<string>& block, int length,
							        int count) {}
		virtual scores* get_stats(string& motif);
		virtual int get_count(string& motif);
		virtual int get_seqs(string& motif) { return 0; } 
		virtual void get_regex_matches(vector<string>& matches, string& regex){}
		virtual void output() {}
		uint32_t arraySize() { return array_size; }
		void printArray();

		// modifiers
		virtual void reset();
		virtual int set_stats(string& motif, scores *new_stats);

    private:
		unsigned char *text;  // actual original DNA text (move me later)
		saidx_t *SA;
		uint32_t array_size;
		uint32_t current_word_idx;  // for reset() and get_next_word() 
		std::map<std::string,scores*> scored_words;

		void readFile(const char *file_name);
		void buildSA();
		saidx_t BinarySearch(const sauchar_t *word, 
							 uint32_t word_length, saidx_t low, saidx_t high);
		saidx_t BinarySearchIterative(const sauchar_t *word, 
						              uint32_t word_length);		
};

#endif  // SUFFIX_ARRAY_H

