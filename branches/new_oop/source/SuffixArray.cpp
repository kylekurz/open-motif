/*
 * SuffixArray.cpp
 * Lee Nau (lee.nau@gmail.com)
 */

#include "SuffixArray.h"

SuffixArray::SuffixArray() {}

SuffixArray::SuffixArray(owef_args *list) {
	using namespace std;
	this->list = list;  // SuffixArray inherits from data, so list is inherited
	readFile(list->seq_file.c_str());  // read input file
	current_word_idx = 0;
	buildSA();
}

SuffixArray::~SuffixArray() {
    array_size = 0;
	current_word_idx = 0; 
    delete[] SA;    // free SA indexes
	delete[] text;  // free DNA string
}

void SuffixArray::buildSA() {
	using namespace std;
	array_size = strlen((const char*) text);  // C string style
	SA = new saidx_t[array_size];
	cout << "Creating a Suffix Array with Num Elements: " << array_size << endl;
	if (divsufsort(text, SA, array_size) != 0) {
		cout << "Error creating Suffix Array!\n";
		exit(1);  // TODO: replace with exception handling
	}
	cout << "Done creating SA\n";
}

void SuffixArray::readFile(const char *file_name) {
	using namespace std;

    cout << "Reading FASTA file from disk: " << file_name << "... ";
       
	string str;
    const unsigned int LINE_SIZE = 1024;  // MOVE ME!
    char line[LINE_SIZE];

    ifstream fin(file_name);
    if (!fin) {
		cout << "Error: Unable to open FASTA file\n";
		exit(1);  // terminate process for now - replace later with Exception
    } else {
		cout << "File successfully opened\n";
    }
    
    // TODO: put this in while-loop so that multiple seqs can be read
    fin.getline(line, LINE_SIZE);  // first eat header
    while (fin.getline(line, LINE_SIZE)) {
		str += line;
    }
			
    // TODO: make me more efficient!
    // convert entire string to uppercase
    for (uint32_t i = 0; i < str.size(); i++) {  
		str[i] = toupper(str[i]);       // change me later?
    }
			
    ///str += '$';  // TODO: add terminating character!  - FIXME!
    ///num_seqs = 1;  // TODO: make this variable!

	text = new unsigned char[str.size()];
	strcpy((char *)text, str.c_str());

    fin.close();  // close the input file
}

std::string SuffixArray::get_next_word(int length) {
	///static int current_word_idx = 0;

	if (length > (int) array_size) {
		cout << "Error: word length exceeds DNA string size!\n";
		return "";
	}

	sauchar_t next_word[length];

	if (strlen((char *)text+SA[current_word_idx]) >= (unsigned int) length) {
		strncpy((char *)next_word, (char *)text+SA[current_word_idx], length);
		next_word[length] = '\0'; // null-terminate
	} else {  // error case
		std::cout << "Error: current word shorter than desired length\n";
		return "";
	}

	while (strncmp((char*)next_word, 
				   (char*)text+SA[current_word_idx+1], length) == 0) {
		++current_word_idx;  // keep iterating over identical words
	}

	return string((char *) next_word);
}

int SuffixArray::get_count(string& motif) {
	return BinarySearch((const sauchar_t *) motif.c_str(), motif.size(), 
						0, array_size);
}

int SuffixArray::set_stats(string& motif, scores *new_stats) {
	scored_words[motif] = new_stats;
	return 1;
}

scores* SuffixArray::get_stats(string& motif) {
	if (scored_words.find(motif) == scored_words.end())
		return NULL;
	else
		return scored_words[motif];
}

void SuffixArray::reset() {
	current_word_idx = 0;  // reset current word index 
}

// returns the number of occurrences of a word
saidx_t SuffixArray::BinarySearch(const sauchar_t *word, uint32_t word_length, 
								  saidx_t low, saidx_t high) {
    if (high < low) {
		cout << "Word does not occur: " << word << endl;
		return -1;
    }
    saidx_t mid = low + ((high - low) / 2);
    if (strncmp((char *)text+SA[mid], (char *)word, word_length) > 0) {
		return BinarySearch(word, word_length, low, mid-1);
    } else if (strncmp((char *)text+SA[mid], (char *)word, word_length) < 0) {
		return BinarySearch(word, word_length, mid+1, high);
    } else {
//	cout << string((char *)text+SA[mid]).substr(0, word_length) << " == " 
//	     << word << endl;
		saidx_t left = mid, right = mid;
	
		while (strncmp((char *)text+SA[left-1], 
				       (char *)word, word_length) == 0) {
			left--; 
		}
	
		while (strncmp((char*) text+SA[right+1], 
					    (char *)word, word_length) == 0) {
			right++;
		}

		cout << "Left: " << left << " right: " << right << endl;
		saidx_t total_occr = right-left+1;
		cout << "Total Occr: " << total_occr << endl;
//	for (saidx_t i = left; i <= right; i++) {
//	    cout << "SA[" << i << "]: " 
//	 << string((char *)text+SA[i]).substr(0, word_length) << endl;
//	}

		//return mid;  // instead of returning index, return # occurrences
		return total_occr;
	}
}

saidx_t SuffixArray::BinarySearchIterative(const sauchar_t *word, 
									       uint32_t word_length) {
    uint32_t text_size = strlen((char *)text);
    ///uint32_t num_occr = 0;
    saidx_t low, mid, high;
    saidx_t left, right;
    low = 0;
    high = text_size; 
    while (low < high) {
		mid = low + ((high - low) / 2);
		if (strncmp((char *)text+SA[mid], (char *)word, word_length) < 0) {
			low = mid + 1;
		} else {
			high = mid;
		}
    }
	
    // TODO: combine this logic (if and while) 
    if ((low < (saidx_t) text_size) && 
	    (strncmp((char *)text+SA[low], (char *)word, word_length) == 0)) {
		left = low;
		cout << "Left: " << left;
		do { ++low; }
		while ((low < (saidx_t) text_size) && 
			(strncmp((char *)text+SA[low], (char *)word, word_length) == 0));//{
//	    cout << "SA[" << low << "]: " 
//		 << string((char *)text+SA[low]).substr(0, word_length) << endl;
	    //++low;
	//}
		right = low - 1;
		cout << " right: " << right << endl;
		cout << "Total Occr: " << right-left+1 << endl;
		return right;

    } else {
		cout << "Word does not occur: " << word << endl;
		return -1;
    }
}
