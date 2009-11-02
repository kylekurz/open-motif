#include <stdint.h>
#include "Data_structure.h"

#ifndef SUFFIX_ARRAY
#define SUFFIX_ARRAY

class suffix_array : public data
{
    public:
	suffix_array();
	suffix_array(owef_args *from_input);
	~suffix_array();

	int get_count(string motif);
	int get_seqs(string motif);
	scores* get_stats(string motif);
	vector<string> get_regex_matches(string regex);
	vector<string> get_seq_file();
	
	int set_stats(string motif, scores *new_stats);
	
	string get_next_word(int length);
	string get_next_extension(string seed, int length);
	void output();
	void reset();

    private:
	uint32_t *SA;
	uint32_t array_size;
};

#endif // SUFFIX_ARRAY
