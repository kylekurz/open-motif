#include "Suffix_array.h"

suffix_array::suffix_array() {
    SA = NULL;
    array_size = 0;
}

suffix_array::suffix_array(owef_args *from_input) {
    list = from_input;
    SA = NULL;
    array_size = 0;
}

suffix_array::~suffix_array() {
    delete[] SA;
    array_size = 0;
}

int suffix_array::get_count(string motif) {
    return 0;  // placeholder - fix me
}

int suffix_array::get_seqs(string motif) {
    return 0;
}

scores* get_stats(string motif) {
    return NULL;
}

vector<string> suffix_array::get_regex_matches(string regex) {
    vector<string> regex_matches;
    return regex_matches;
}

vector<string> suffix_array::get_seq_file() {
    vector<string> seq_file;
    return seq_file;
}

int suffix_array::set_stats(string motif, scores *new_stats) {
    return 0;
}

string suffix_array::get_next_word(int length) {
    return "";
}

string suffix_array::get_next_extension(string seed, int length) {
    return "";
}

void suffix_array::output() {
    cout << "";
}

void suffix_array::reset() {
    // what should this do?
}


