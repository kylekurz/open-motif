/*
 * SuffixArray.cpp
 * Lee Nau (lee.nau@gmail.com)
 */

#include "SuffixArray.h"

// for basic tests not incorporating the framework (sarray_test.cpp)
SuffixArray::SuffixArray(const char *file_name) {
	readFile(file_name);
	current_word_idx = 0;
	buildSA();
}

SuffixArray::SuffixArray(owef_args *list) {
	using namespace std;
	this->list = list;  // SuffixArray inherits from data, so list is inherited
	readFile(list->seq_file.c_str());  // read input file
	current_word_idx = 0;
	buildSA();

	// set up list members (such as iterators)
	for (int i = 0; i < list->maxlength; i++) {
		// index 0 in num_words contains words of length 1, and so on
		list->num_words[i] = numWords(i+1);  
	}
}

SuffixArray::~SuffixArray() {
    array_size = 0;
	current_word_idx = 0; 
    delete[] SA;    // free SA indexes
	delete[] text;  // free DNA string
}

inline bool SuffixArray::STRNCMP(unsigned char *w1, 
								 unsigned char *w2, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		if (w1[i] != w2[i])
			return false;
	}
	return true;
}

inline bool SuffixArray::STRLESS(unsigned char *w, uint32_t length) {
	for (uint32_t i = 0; i < length; i++) {
		if (w[i] == '\0')
			return true;  // true if w is less
	}
	return false;  // false if the word ISN'T less
}

// returns the number of words of a given length
uint32_t SuffixArray::numWords(uint32_t length) {
	uint32_t i, num_words = 0;

	if (length == 0)  return 0;  // no words!

	if (length > array_size) {  // should this be >= - only with $ char
		cout << "Error: word length exceeds string size! - no words!\n";
		return 0;
	}

	for (i = 0; i < array_size; i++) {
		
		/*while (strlen((char*)text+SA[i]) < length) {*/
		while (STRLESS(text+SA[i], length) == true) {
			++i;
		}
			
		++num_words;

		/*while (strncmp((char*)text+SA[i],
						 (char*)text+SA[i+1], length) == 0) {*/
		while (STRNCMP(text+SA[i], text+SA[i+1], length) == true) {
			++i;
		}
	}
	
	return num_words;
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

void SuffixArray::printArray() {
	for (saidx_t i = 0; i < (int) array_size; i++) {
		std::cout << "SA[ " << i << "]: " << SA[i] << "\t" 
			      << text+SA[i] << std::endl;
	}
}

std::string SuffixArray::get_next_word(int length) {
	// TODO: separate these two!
	///static int current_word_idx = 0; // moved this to class private member

	if (length > (int) array_size) {
		cout << "Error: word length exceeds DNA string size!\n";
		exit(1);  // TODO: replace with exception handling
	}

	// skip over words shorter than desired length
	/*while (strlen((char*)text+SA[current_word_idx]) < (unsigned int) length) {*/
	while (STRLESS(text+SA[current_word_idx], length) == true) {
		++current_word_idx;
	}

	if (current_word_idx+1 > array_size) {  // hack to avoid going over by 1
		cout << "Error: looking past the end of the Suffix Array for word!\n";
		exit(1);  // TODO: replace with exception handling
	}

	/*while (strncmp((char*)next_word, 
				   (char*)text+SA[current_word_idx+1], length) == 0) {*/
	while (STRNCMP(text+SA[current_word_idx], 
				   text+SA[current_word_idx+1], length) == true) {
		++current_word_idx;  // keep iterating over identical words
	}

	sauchar_t next_word[length];
	strncpy((char*)next_word, (char*)text+SA[current_word_idx], length);
	next_word[length] = '\0';
	++current_word_idx;  // increase it by 1 either way for next word
	return string((char*)next_word);
	//return string((char *)text+SA[current_word_idx], length);
}

int SuffixArray::get_count(string& motif) {
	return BinarySearch((const sauchar_t *) motif.c_str(), motif.size(), 
						0, array_size);
}

int SuffixArray::set_stats(string& motif, scores *new_stats) {

	#pragma omp critical  // TODO: move this!
	{
		scored_words[motif] = new_stats;
	}
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
		//cout << "Word does not occur: " << word << endl;
		//return -1;
		return 0;
    }
    saidx_t mid = low + ((high - low) / 2);
    if (strncmp((char *)text+SA[mid], (char *)word, word_length) > 0) {
		return BinarySearch(word, word_length, low, mid-1);
    } else if (strncmp((char *)text+SA[mid], (char *)word, word_length) < 0) {
		return BinarySearch(word, word_length, mid+1, high);
    } else {
		saidx_t left = mid, right = mid;
	
		/*while (strncmp((char *)text+SA[left-1], 
				       (char *)word, word_length) == 0) {*/
		while (STRNCMP(text+SA[left-1], (unsigned char *)word, 
										word_length) == true) {
			left--; 
		}
	
		/*while (strncmp((char*) text+SA[right+1], 
					    (char *)word, word_length) == 0) {*/
		while (STRNCMP(text+SA[right+1], (unsigned char *)word, 
										 word_length) == true) {
			right++;
		}

		saidx_t total_occr = right-left+1;

		//return mid;  // instead of returning index, see below
		return total_occr;  // return # occurrences 
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

