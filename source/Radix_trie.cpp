//**********************************************
// File:	Radix_trie.cpp
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a radix trie class for use
//		with the WordSeeker pipeline.
//**********************************************
/*
Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley
*/

#include "Radix_trie.h"


using namespace std;

extern "C" void *listener(void *);

//structure for passing to parallel processor
typedef struct args{
	int thr_id;
	radix_trie *job;
	owef_args *my_list;
}args;

//************************************************************
//Constructors
//************************************************************

//standard constructor
radix_trie::radix_trie()
{
	root = NULL;
}

//standard destructor
radix_trie::~radix_trie()
{
	trie_clear(root);
}

// listen for MPI requests for words
#ifdef KKURZ_MPI
void* listener(void *_Object) 
{
	int flag=0;
	args *p = (args *)_Object;
	int length = p->my_list->maxlength+1;
	int done = 1;
	int ret, rc;
	char to_count[p->my_list->maxlength+1];
	MPI_Status status;
	while(done != 0)
	{
		for(int a=0; a<p->my_list->maxlength+1; a++)
			to_count[a] = (char)NULL;
		rc = MPI_Recv(&to_count, length, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		//cout << "rc on slave receive: " << rc << endl;
		done = status.MPI_TAG;
		string test = to_count;
		if(done == 0)
		{
			//cout << "done " << done << endl;		
			MPI_Finalize();
			exit(0);
		}
		else if(done == 1)
		{
			ret = p->job->get_count(test);
			rc = MPI_Send(&ret, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		}
		else if(done == 2)
		{
			ret = p->job->get_seqs(test);
			rc = MPI_Send(&ret, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);	
		}
		else if(done == 3)
		{
			int x = atoi(to_count);
			ret = p->my_list->num_words[x];
			rc = MPI_Send(&ret, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
		}
	}
	return NULL;
}
#endif
	
//initialized radix trie
radix_trie::radix_trie(owef_args *from_input)
{

	//push the alphabet into the vector (optimized later)
	conversion.push_back('A');
	conversion.push_back('C');
	conversion.push_back('G');
	conversion.push_back('T');
	conversion.push_back('B');
	conversion.push_back('D');
	conversion.push_back('E');
	conversion.push_back('F');
	conversion.push_back('G');
	conversion.push_back('H');
	conversion.push_back('I');
	conversion.push_back('J');
	conversion.push_back('K');
	conversion.push_back('L');
	conversion.push_back('M');
	conversion.push_back('O');
	conversion.push_back('P');
	conversion.push_back('Q');
	conversion.push_back('R');
	conversion.push_back('S');
	conversion.push_back('U');
	conversion.push_back('V');
	conversion.push_back('W');
	conversion.push_back('X');
	conversion.push_back('Y');
	conversion.push_back('Z');
	
	#ifdef KKURZ_MPI
	// create new thread for listener, which will wait for requests
	pthread_t listen_thread;
	void *x = NULL;
	args *p = new args;
	p->thr_id = 1;
	p->job = this;	
	p->my_list = list;
	pthread_create(&listen_thread, NULL, listener, x);
	#endif

	cout << endl << "Creating Radix trie..." << endl << endl;

	//point to the input parameters
	list = from_input;
	
	//set up the trie
	root = NULL;
	clock_t start, end;
	double duration;
	start = clock();
	radix_trie_node *temp = NULL;
	string word = "";
	for(int i=0; i<list->maxlength; i++)
	{
		last_loc.push_back(temp);
		last_ext_loc.push_back(temp);
		last_word.push_back(word);
		last_ext.push_back(word);
	}
	next_branch = 0;
	
	//count the words
	count_words();
	end = clock();
	duration = (double)(end-start)/CLOCKS_PER_SEC;
	printf("%f seconds\n", duration);
	
	#ifndef KKURZ_MPI
	//print the number of words found to the screen
	cout << "Number of words of length: " << endl;
	for(int i=0; i<list->maxlength; i++)
		cout << i+1 << " " << list->num_words[i] << endl;
	#endif
	
	#ifdef KKURZ_MPI
	if(list->rank == 0)
	{
		int word_counts[list->maxlength];
		for(int h=0; h<list->maxlength; h++)
		{
			word_counts[h] += list->num_words[h];
			int ret;
			MPI_Status status;
			string offset;
			stringstream temp;
			temp << h;
			temp >> offset;
			
			for(int dest=1; dest<list->numtasks; dest++)
			{
				int rc = MPI_Send(&offset[0], offset.length(), MPI_CHAR, dest, 3, MPI_COMM_WORLD);
				rc = MPI_Recv(&ret, 1, MPI_INT, dest, 3, MPI_COMM_WORLD, &status); 
				//cout << "adding: " << ret << endl;
				word_counts[h] += ret;
			}
		}
		//print the number of words found to the screen
		cout << "Number of words of length: " << endl;
		for(int i=0; i<list->maxlength; i++)
			cout << i+1 << " " << word_counts[i] << endl;
		cout << endl;
		if(list->missing)
		{
			ofstream ofile;
			stringstream stream;
			stream <<  list->prefix << "_" << list->maxlength << "_" << list->order << "_missing_scored.csv";
			string filename;
			stream >> filename;
			ofile.open(filename.c_str());
			ofile << "#Word" << endl;
			miss_search(root, ofile, list->maxlength, "");
			ofile.close();
		}
	}
	if(list->rank != 0)
	{
		/*int length = list->maxlength+1;
		int done = 1;
		int ret, rc;
		char to_count[list->maxlength+1];
		MPI_Status status;
		while(done != 0)
		{
			for(int a=0; a<list->maxlength+1; a++)
				to_count[a] = (char)NULL;
			rc = MPI_Recv(&to_count, length, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			//cout << "rc on slave receive: " << rc << endl;
			done = status.MPI_TAG;
			string test = to_count;
			if(done == 0)
			{
				//cout << "done " << done << endl;		
				MPI_Finalize();
				exit(0);
			}
			else if(done == 1)
			{
				ret = get_count(test);
				rc = MPI_Send(&ret, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
			}
			else if(done == 2)
			{
				ret = get_seqs(test);
				rc = MPI_Send(&ret, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);	
			}
			else if(done == 3)
			{
				int x = atoi(to_count);
				ret = list->num_words[x];
				rc = MPI_Send(&ret, 1, MPI_INT, 0, 3, MPI_COMM_WORLD);
			}
		}*/
	}
	#endif
	if(!list->score)
		output();
}

//************************************************************
//Accessors
//All accessors simply return the value of their associated
//variable.  No modifications allowed. If data structure does
//not store information, throw exception with (-1)
//************************************************************

#ifdef KKURZ_MPI
//function to get word count
int radix_trie::get_count(string motif)
{
	//motif = convert(motif);
	int count = motif.length();
	int ret_count = 1;
	int dest = locate_branch(motif[0]);
	int ret, rc;
	MPI_Status *status = NULL;
	//compute all the necessary scores
	//cout << " count: count " << count << " dest " << dest << " motif " << motif << endl;
	if(dest != list->rank)
	{
		//motif = unconvert(motif);
		rc = MPI_Send(&motif[0], count, MPI_CHAR, dest, 1, MPI_COMM_WORLD);
		rc = MPI_Recv(&ret, ret_count, MPI_INT, dest, 1, MPI_COMM_WORLD, status); 
		return ret;
	}
	else
	{
		char *t = &motif[0];
		int l = motif.length();
		return trie_find(root, t, l);
	}
}

//function to get sequence count
int radix_trie::get_seqs(string motif)
{
	//motif = convert(motif);
	int count = motif.length();
	int ret_count = 1;
	int dest = locate_branch(motif[0]);
	int ret, rc;
	MPI_Status *status = NULL;
	//compute all the necessary scores
	if(dest != list->rank)
	{
		//motif = unconvert(motif);
		rc = MPI_Send(&motif[0], count, MPI_CHAR, dest, 2, MPI_COMM_WORLD);
		rc = MPI_Recv(&ret, ret_count, MPI_INT, dest, 2, MPI_COMM_WORLD, status); 
		return ret;
	}
	else
	{
		char *t = &motif[0];
		int l = motif.length();
		return trie_find_s(root, t, l);
	}
}
#endif

#ifndef KKURZ_MPI
//function to get word count
int radix_trie::get_count(string motif)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_find(root, t, l);
}

//function to get sequence count
int radix_trie::get_seqs(string motif)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_find_s(root, t, l);
}
#endif

vector<string> radix_trie::get_regex_matches(string regex)
{
	//regex = convert(regex);
	vector<string> t1, ret_vector;
	t1.push_back(regex);
	while (!t1.empty()) 
	{
		string temp = t1.back();
		t1.pop_back();
		int found = temp.find_first_not_of("AaCcGgTt");
		if(found != -1)
		{
			for(int k=0; k<4; k++)
			{
				temp[found] = 'A'+k;
				//cout << "TEMP " << temp << endl;
				t1.push_back(temp);
			}
		}
		else 
		{
			//cout << "Before TEMP " << temp << endl;
			if(get_count(temp) != 0)
			{
				//cout << "Else TEMP " << temp << endl;
				ret_vector.push_back(temp);
			}
		}

	}
	return ret_vector;
}

//function to get the sequences from the input file
vector<string> radix_trie::get_seq_file()
{
	vector<string> seqs;
	ifstream in;
	//cout << list->seq_file << endl;
	in.open(list->seq_file.c_str());
	if(in.fail())
	{
		cerr << "FAILED TO OPEN INPUT FILE" << endl;
		exit (-1);
	}
	//read in the file, keeping tags separate from the sequences
	while(!in.eof())
	{
		string tag;
		if(in.peek()=='>'||in.peek()==' ')
		{
			string t;
			getline(in, t);
			continue;
		}
		else if(!in.eof())
		{
			string t;
			while(in.peek() != '>' && !in.eof())
			{
				getline(in, t);
				tag.append(t);
			}	
			seqs.push_back(tag);
		}
		tag.clear();
	}
	in.close();
	return seqs;
}	
		
//************************************************************
//Modifiers
//All modifiers return their new value by default.  This may
//be ignored based on developer preference.
//
//If not implemented by the data structure, throw (-1)
//************************************************************

//************************************************************
//General Purpose Functions
//May do any number of operations, should still be implmented
//for each data structure incorporated.
//************************************************************

//function to increment count
int radix_trie::inc_count(string motif)
{
	if(list->n_filter)
	{
		int N_loc = motif.find_first_of("Nn");
		if(N_loc >= 0)
			return (-1);
	}
	if(list->anc_filter)
	{
		int anc_res = motif.find_first_of("abcdefghijklmnopqrstuvwxyz");
		if(anc_res >= 0)
			return (-1);
	}
	char *t = &motif[0];
	int l = motif.length();
	int retval = trie_get(root, t, l);
	if(retval == -1)
	{
		trie_add(root, t, l, l-1);
		return(1);
	}
	else
		return retval;
}

//function to increment seqs
int radix_trie::inc_seqs(string motif)
{
	char *t = &motif[0];
	int l = motif.length();
	int retval = trie_get(root, t, l);
	if(retval == -1)
	{
		trie_add(root, t, l, l-1);
		return (1);
	}
	else
		return retval;
}

//function to add a new element to the radix trie
void radix_trie::trie_add (radix_trie_node *& node, char *s, int length, int level)
		/* add string s to the trie and attach data to it */
{	
	if (length==0)
	{
		list->num_words[level] = (list->num_words[level])+1;
		node->data = 1;
		node->num_seq = 1;
		node->last_seq = list->seq;
	}
	else
	{
		int idx = locate_branch(*s);
		if(!node)
			node = new radix_trie_node();
		if (!node->branch)
		{
			node->branch = node->new_pointer_array();
			//set all the pointers to NULL (avoid seg fault!)
			for(int i=0; i<5; i++)
				node->branch[i]=NULL;
		}
		if(!node->branch[idx])
			node->branch[idx] = new radix_trie_node();
		trie_add (node->branch[idx], s + 1, length-1, level);
	}
}

//function to get the count of a word in the radix trie and increment it
int radix_trie::trie_get (radix_trie_node * &node, char *s, int length)
		/* return data associated with string s, or NULL if not found */
{
	if(!node)
		return (-1);
	if (length==0)
	{
		if(node->last_seq != list->seq)
		{
			(node->num_seq)++;
			node->last_seq = list->seq;
		}
		return (node->data)++;
	}
	if(!node->branch)
		return (-1);
	int idx = locate_branch(*s);
	if(!node->branch[idx])
		return (-1);
	return trie_get (node->branch[idx], s + 1, length-1);
}

//function to get the count of a word in the radix trie (no increment)
int radix_trie::trie_find (radix_trie_node * &node, char *s, int length)
		/* return data associated with string s, or NULL if not found */
{
	if (!node)
		return (0);
	if (length==0)
		return (node->data);
	if(!node->branch)
		return (0);
	return trie_find (node->branch[locate_branch(*s)], s + 1, length-1);
}

//function to get the sequence count of a word in the radix trie (no increment)
int radix_trie::trie_find_s (radix_trie_node * &node, char *s, int length)
		/* return data associated with string s, or NULL if not found */
{
	if (!node)
		return (0);
	if (length==0)
		return (node->num_seq);
	if(!node->branch)
		return (0);
	return trie_find_s(node->branch[locate_branch(*s)], s + 1, length-1);
}

//function to delete the trie
void radix_trie::trie_clear (radix_trie_node * node)
{
	for(int i=0; i<ALPH; i++)
	{
		if(node->branch)
		{
			if(node->branch[i] != NULL)
			{
				trie_clear(node->branch[i]);
			}
			delete node;
			node = NULL;
		}
	}
}

//output to a file so we can use it in the next step
void radix_trie::output()
{
	int num_files = (list->maxlength - list->minlength)+1;
	ofstream ofiles[num_files];
	for(int i=0; i<num_files; i++)
	{
		stringstream stream;
		stream <<  list->prefix << "_" << i+list->minlength << ".csv";
		string filename;
		stream >> filename;
		ofstream log(list->log_file.c_str(), ios::app);
		log << "Word Counting file: " << filename << endl;
		log.close();
		ofiles[i].open(filename.c_str());
		ofiles[i] << "#Word,Sequences,Occurs" << endl;
		for(int j=0; j<list->num_words[(i+list->minlength)-1]; j++)
		{
			string to_out = get_next_word(i+list->minlength);
			if(get_count(to_out) >= list->min_O && get_seqs(to_out) >= list->min_seqs)
				ofiles[i] << to_out << ',' << get_seqs(to_out) << ',' << get_count(to_out) << endl;
		}
		//output(ofiles[i], root, "", i+list->minlength);
	}
}

void radix_trie::reset()
{
	for (int i=0; i<static_cast<int> (last_word.size()); i++) {
		last_word[i] = "";
	}
	for (int j=0; j<static_cast<int> (last_loc.size()); j++) {
		last_loc[j] = NULL;
	}
}

//systematically returns EVERY word stored in the trie at a given length.  It is the caller's responsibility
//to verify that the word meets any requirements of the job (i.e. min_O and min_seqs before output)
string radix_trie::get_next_word(int length)
{
	string ret_word = "";
	radix_trie_node *node = NULL;
	//if we haven't returned anything yet...
	if(last_loc[length-1] == NULL && last_word[length-1].compare("") == 0) 
	{
		//if we have a trie here...
		if(root && root->branch)	
		{
			node = root;
			//hunt until we find a word long enough to be the "first"
			while(static_cast<int>(ret_word.length()) < length) 
			{
				//if we can keep following this branch
				if(node->branch && node->branch[next_branch]) 
				{
					char x = conversion[next_branch];
					ret_word += x;
					node = node->branch[next_branch];
					next_branch = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else 
				{
					next_branch++;
					if (next_branch >= ALPH) {
						while (next_branch >= ALPH) {
							char branch_id = ret_word[ret_word.length()-1];
							ret_word = ret_word.substr(0, ret_word.length()-1);
							next_branch = (locate_branch(branch_id)) + 1;
							node = root;
							for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
								if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
									node = node->branch[locate_branch(ret_word[i])];
								}
							}
						}						
					}
				}
			}
			last_loc[length-1] = node;
			last_word[length-1] = ret_word;
			return ret_word;
		}
	}
	//otherwise we have something and can start from there...
	else {
		char branch_id = last_word[length-1][last_word[length-1].length()-1];
		ret_word = last_word[length-1].substr(0, last_word[length-1].length()-1);
		next_branch = locate_branch(branch_id) + 1;
		node = root;
		for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
			if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
				node = node->branch[locate_branch(ret_word[i])];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while(static_cast<int>(ret_word.length()) < length) 
		{
			//if we can keep following this branch
			if(node && node->branch && node->branch[next_branch]) 
			{
				char x = conversion[next_branch];
				ret_word += x;
				node = node->branch[next_branch];
				next_branch = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else 
			{
				next_branch++;
				if (next_branch >= ALPH) {
					char branch_id;
					while (next_branch >= ALPH) {
						branch_id = ret_word[ret_word.length()-1];					
						ret_word = ret_word.substr(0, ret_word.length()-1);
						next_branch = locate_branch(branch_id) + 1;
						node = root;
						for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
							if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
								node = node->branch[locate_branch(ret_word[i])];
							}
						}
					}	
				}
			}
		}
	}	
	last_loc[length-1] = node;
	last_word[length-1] = ret_word;
	return ret_word;
}

//systematically returns EVERY word stored in the trie at a given length.  It is the caller's responsibility
//to verify that the word meets any requirements of the job (i.e. min_O and min_seqs before output)
string radix_trie::get_next_word(radix_trie_node *temp_root, int length)
{
	string ret_word = "";
	radix_trie_node *node = NULL;
	//if we haven't returned anything yet...
	if(last_ext_loc[length-1] == NULL && last_ext[length-1].compare("") == 0) 
	{
		//if we have a trie here...
		if(temp_root && temp_root->branch)	
		{
			node = temp_root;
			//hunt until we find a word long enough to be the "first"
			while(static_cast<int>(ret_word.length()) < length) 
			{
				//if we can keep following this branch
				if(node->branch && node->branch[next_branch]) 
				{
					char x = conversion[next_branch];
					ret_word += x;
					node = node->branch[next_branch];
					next_branch = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else 
				{
					next_branch++;
					if (next_branch >= ALPH) {
						while (next_branch >= ALPH) {
							char branch_id = ret_word[ret_word.length()-1];
							ret_word = ret_word.substr(0, ret_word.length()-1);
							next_branch = locate_branch(branch_id) + 1;
							node = temp_root;
							for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
								if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
									node = node->branch[locate_branch(ret_word[i])];
								}
							}
							if(ret_word.length()==0 && next_branch >= ALPH)
							{
								last_ext_loc[length-1] = NULL;
								last_ext[length-1].clear();
								last_ext[length-1] = "";
								return "";
							}
						}						
					}
				}
			}
			last_ext_loc[length-1] = node;
			last_ext[length-1] = ret_word;
			return ret_word;
		}
	}
	//otherwise we have something and can start from there...
	else {
		char branch_id = last_ext[length-1][last_ext[length-1].length()-1];
		ret_word = last_ext[length-1].substr(0, last_ext[length-1].length()-1);
		next_branch = locate_branch(branch_id) + 1;
		node = temp_root;
		for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
			if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
				node = node->branch[locate_branch(ret_word[i])];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while(static_cast<int>(ret_word.length()) < length) 
		{
			//if we can keep following this branch
			if(node && node->branch && node->branch[next_branch]) 
			{
				char x = conversion[next_branch];
				ret_word += x;
				node = node->branch[next_branch];
				next_branch = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else 
			{
				next_branch++;
				if (next_branch >= ALPH) {
					char branch_id;
					while (next_branch >= ALPH) {
						branch_id = ret_word[ret_word.length()-1];					
						ret_word = ret_word.substr(0, ret_word.length()-1);
						next_branch = locate_branch(branch_id) + 1;
						node = temp_root;
						for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
							if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
								node = node->branch[locate_branch(ret_word[i])];
							}
						}
						if(ret_word.compare("") == 0 && (next_branch >= ALPH || next_branch < 0))
						{
							last_ext_loc[length-1] = NULL;
							last_ext[length-1].clear();
							last_ext[length-1] = "";
							return "";
						}
					}	
				}
			}
		}
	}	
	last_ext_loc[length-1] = node;
	last_ext[length-1] = ret_word;
	return ret_word;
}

//function to iterate through the subtree and find all occurring words of length x, //from a given seed of length y string 
string radix_trie::get_next_extension(string seed, int length)
{
	length -= seed.length();
	radix_trie_node *temp_root;
	temp_root = root;
	for(int i=0; i<static_cast<int>(seed.length()); i++)
		temp_root = temp_root->branch[locate_branch(seed[i])];
	string t = get_next_word(temp_root, length);
	if(t.compare("")==0)
		return "";
	else
		return seed.append(t);
}



//function to enumerate all possible strings from a string containing an 'X'
void radix_trie::enumerate_string(ofstream &ofile, vector<string> words)
{
	while(!words.empty())
	{
		string x = words.back();
		words.pop_back();
		int pos = x.find('X');
		if(pos >= 0)
		{
			x[pos] = 'A';
			words.push_back(x);
			x[pos] = 'C';
			words.push_back(x);
			x[pos] = 'G';
			words.push_back(x);
			x[pos] = 'T';
			words.push_back(x);
		}
		else {ofile << x << endl;}
		x.clear();
	}
}

//function to search the trie and find what words are NOT in it
void radix_trie::miss_search(radix_trie_node *node, ofstream &ofile, int length_rem, string word)
{
	//if we have the right number of characters
	if(length_rem == 0){return;}
	//if we dont have a node (specifically root)
	if(node==NULL || node->branch==NULL)
	{
		//cout << "null node" << endl;
		int temp = length_rem;
		string now = word;
		now.append(temp, 'X');
		if(list->enumerate)
		{
			if(temp >=0)
			{
				vector<string> t;
				t.push_back(now);
				enumerate_string(ofile, t);
				t.clear();	
			}
			else{ofile << now << endl;}
		}
		else{ofile << now << endl;}
		now.clear();
		return;
	}
	for(int i=0; i<4; i++)
	{
		if(node->branch[i]==NULL)
		{
			//cout << "null branch" << endl;
			int temp = length_rem;
			string now = word;
			string a;
			switch(i)
			{
				case 0:
					a = "A";
					break;
				case 1:
					a = "C";
					break;
				case 2:
					a = "G";
					break;
				case 3:
					a = "T";
					break;
				default:
					a = "N";
					break;
			}
			now += a;
			temp--;
			now.append(temp, 'X');
			if(list->enumerate)
			{
				if(temp >=0)
				{
					vector<string> t;
					t.push_back(now);
					enumerate_string(ofile, t);
					t.clear();	
				}
				else{ofile << now << endl;}
			}
			else{ofile << now << endl;}
			now.clear();
		}
		else
		{	
			//cout << "recursing" << endl;
			int temp = length_rem;
			string a;
			switch(i)
			{
				case 0:
					a = "A";
					break;
				case 1:
					a = "C";
					break;
				case 2:
					a = "G";
					break;
				case 3:
					a = "T";
					break;
				default:
					a = "N";
					break;
			}
			string now = word;
			now += a;
			miss_search(node->branch[i], ofile, --temp, now);
			now.clear();
		}
	}
}

//function to count all the words in an input
void radix_trie::count_words()
{
	//******************************************************************
	//start reading in words and counting them
	//Developers should modify the count call in this loop with a call
	//to their data structure of choice.
	//******************************************************************
	
	vector<string> seqs = get_seq_file();
	
	for(int h=0; h<static_cast<int>(seqs.size()); h++)
	{
		string tag;
		#ifdef KKURZ_MPI
		if(list->rank == 0)
		{
		#endif
			list->total_input_length += seqs[h].length();
			list->background_seqs.push_back(seqs[h].length());
		#ifdef KKURZ_MPI
		}
		#endif
		tag = seqs[h];
		for(int i=0; i<(static_cast<int>(tag.length())); i++)
		{
			#ifdef KKURZ_MPI
			if(locate_branch(tag[i]) != list->rank)
				continue;
			else
			{
			#endif
				int length = min(static_cast<int>(tag.length())-i,list->maxlength);
				for(int k=1; k<=length; k++)
				{
					string to_count = tag.substr(i, k);
					inc_count(to_count);
				}
			#ifdef KKURZ_MPI
			}
			#endif
		}
		list->seq++;
		tag.clear();
	}
}

int radix_trie::locate_branch(char x)
{
	x = toupper(x);
	int loc = -1;
	for(int i=0; i<static_cast<int>(conversion.size()); i++)
	{
		if(x == conversion[i])
		{
			loc = i;
			i = conversion.size();
		}
	}
	if(loc >= ALPH)
		loc = ALPH-1;
	return loc;
}

//convert all letters in word to uppercase letters
string radix_trie::to_upper(const string & word)
{
	string temp(word);
	for(int i=0; i<static_cast<int>(word.length()); ++i)
	{
		temp[i] = toupper(word[i]);
	}
	return temp;
}

