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

//create an empty trie...
radix_trie::radix_trie(owef_args *from_input, int x)
{
	list = from_input;
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
	//int flag=0;
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
		else if(done == 4)
		{
			scores *s = p->job->get_stats(test);
			rc = MPI_Send(s, 1, MPI_INT, 0, 4, MPI_COMM_WORLD);
		}
	}
	return NULL;
}
#endif
	
//initialized radix trie
radix_trie::radix_trie(owef_args *from_input)
{
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
		last_word.push_back(word);
	}
	for(int h=0; h<num_procs(); h++)
        {
                vector<string> t1;
                vector<radix_trie_node *> t2;
                for(int i=0; i<list->maxlength; i++)
                {
                        t1.push_back(word);
                        t2.push_back(temp);
                }
                last_ext.push_back(t1);
                last_ext_loc.push_back(t2);
                next_branch_iterator.push_back(0);
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
	#else
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
	/*
	if(list->rank != 0)
	{
		int length = list->maxlength+1;
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
		}
	}*/
	#endif
	if(!list->score)
		output();
	reset();
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

//function to get the stats from a word
scores* radix_trie::get_stats(string motif)
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
		rc = MPI_Send(&motif[0], count, MPI_CHAR, dest, 4, MPI_COMM_WORLD);
		rc = MPI_Recv(&ret, ret_count, MPI_INT, dest, 4, MPI_COMM_WORLD, status); 
		return (scores *)ret;
	}
	else
	{
		char *t = &motif[0];
		int l = motif.length();
		return trie_find_stats(root, t, l);
	}
}
#endif

#ifndef KKURZ_MPI
//function to get word count
int radix_trie::get_count(string & motif)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_find(root, t, l);
}

//function to get sequence count
int radix_trie::get_seqs(string & motif)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_find_s(root, t, l);
}

//function to get the stats from a word
scores* radix_trie::get_stats(string & motif)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_find_stats(root, t, l);
}
#endif

//function to iteratively walk the regular expression provided and retrieve a list of all words
//in the trie that match
void radix_trie::get_regex_matches(vector<string> &matches, string & regex)
{
	vector<string> t1;
	radix_trie_node *tmp = root;
	int num_calls = 0;
	int num_n = 0;
	string temp = "";
	for(int i=0; i<static_cast<int>(regex.length()); i++)
	{
		int nb = branch_array[regex[i]-'A'];
		if(nb != ALPH-1)
		{
			temp += regex[i];
			tmp = tmp->branch[nb];
		}
		else
		{
			num_n = 1;
			int pos = i;
			while(branch_array[regex[++pos] - 'A'] == ALPH-1)
				num_n++;
			string partial = get_next_word(tmp, num_n);
			num_calls++;
			while(partial.compare("") != 0)
			{
				string t2 = temp + partial;
				t1.push_back(t2);
				t2.clear();
				partial.clear();
				partial = get_next_word(tmp, num_n);
				num_calls++;
			}
			i = regex.length()+1;
			tmp = NULL;
			reset_iterator(thread_num());
			next_branch_iterator[thread_num()] = 0;
		}
	}
	while (!t1.empty())
        {
        	tmp = root;
        	string temp = t1.back();
        	t1.pop_back();
        	for(int i=0; i<static_cast<int>(temp.length()); i++)
		{
			int nb = branch_array[temp[i]-'A'];
			if(nb != ALPH-1)
			{
				tmp = tmp->branch[nb];
			}
			else
			{
				num_n = 1;
				int pos = i;
				while(branch_array[regex[++pos] - 'A'] == ALPH-1)
					num_n++;
				string partial = get_next_word(tmp, num_n);
				num_calls++;
				while(partial.compare("") != 0)
				{
					string t2 = temp + partial;
					t1.push_back(t2);
					t2.clear();
					partial.clear();
					partial = get_next_word(tmp, num_n);
					num_calls++;
				}
				i = regex.length()+1;
				tmp = NULL;
				reset_iterator(thread_num());
				next_branch_iterator[thread_num()] = 0;
			}
		}
		for(int j=temp.length(); j<static_cast<int>(regex.length()); j++)
		{
			int nb = branch_array[regex[j]-'A'];
			if(nb != ALPH-1 && tmp->branch && tmp->branch[nb])
			{
				temp += regex[j];
				tmp = tmp->branch[nb];
				if(temp.length() == regex.length())
					matches.push_back(temp);
			}
			else
			{
				temp.clear();
				j = regex.length();
			}
		}
        }
        return;
}

//function to get the sequences from the input file
void radix_trie::get_seq_file(vector<string> &sequences)
{
	ifstream in;
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
			sequences.push_back(tag);
		}
		tag.clear();
	}
	in.close();
	return;
}	
		
//************************************************************
//Modifiers
//All modifiers return their new value by default.  This may
//be ignored based on developer preference.
//
//If not implemented by the data structure, throw (-1)
//************************************************************

int radix_trie::set_count(string motif, int count)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_set(root, t, l, count);
}

int radix_trie::trie_set(radix_trie_node *node, char *s, int length, int count)
{	
	if (!node)
		return (0);
	if (length==0)
	{
		node->data = count;
		return (node->data);
	}
	if(!node->branch)
		return (0);
	return trie_set (node->branch[locate_branch(*s)], s + 1, length-1, count);
}

//function to set the statistics of a word in the trie
int radix_trie::set_stats(string & motif, scores *new_stats)
{
	string temp = motif;
	char *t = &temp[0];
	int l = temp.length();
	return trie_stats(root, t, l, new_stats);
}

//function to set the statistics of a word in the trie
int radix_trie::trie_stats(radix_trie_node *node, char *s, int length, scores *new_stats)
{
	if (!node)
		return (0);
	if (length==0)
	{
		((radix_trie_leaf *)node)->storage->expect = new_stats->expect;
		((radix_trie_leaf *)node)->storage->variance = new_stats->variance;
		return (node->data);
	}
	if(!node->branch)
		return (0);
	return trie_stats (node->branch[locate_branch(*s)], s + 1, length-1, new_stats);	
}

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
		int N_loc = motif.find_first_not_of("AaCcGgTt");
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
	int retval = trie_get(root, t, l, l-1);
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
	int retval = trie_get(root, t, l, l-1);
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
			for(int i=0; i<ALPH; i++)
				node->branch[i]=NULL;
		}
		if(!node->branch[idx])
		{
			if(length > 1)
				node->branch[idx] = new radix_trie_node();
			if(length == 1)
			{
				node->branch[idx] = new radix_trie_leaf();
				((radix_trie_leaf *)node->branch[idx])->storage = new scores;
			}
		}				
		trie_add (node->branch[idx], s + 1, length-1, level);
	}
}

//function to get the count of a word in the radix trie and increment it
int radix_trie::trie_get (radix_trie_node * &node, char *s, int length, int level)
		/* return data associated with string s, or NULL if not found */
{
	if(!node)
		return (-1);
	if (length==0)
	{
		if(node->data == 0)
			list->num_words[level] = (list->num_words[level])+1;
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
	return trie_get (node->branch[idx], s + 1, length-1, level);
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

//function to get the stats of a word in the radix trie (no increment)
scores* radix_trie::trie_find_stats (radix_trie_node * &node, char *s, int length)
		/* return data associated with string s, or NULL if not found */
{
	if (!node)
		return (NULL);
	if (length==0)
		return ((radix_trie_leaf *)node)->storage;
	if(!node->branch)
		return (NULL);
	return trie_find_stats (node->branch[locate_branch(*s)], s + 1, length-1);
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

void radix_trie::reset_iterator(int thread)
{
	for (int i=0; i<static_cast<int> (last_ext[thread].size()); i++) {
		last_ext[thread][i] = "";
	}
	for (int j=0; j<static_cast<int> (last_ext_loc[thread].size()); j++) {
		last_ext_loc[thread][j] = NULL;
	}
}

//get the next consecutive block of words from the trie
void radix_trie::get_next_word_block(vector<string> &block, int length, int count)
{
	for(int i=0; i<count; i++)
	{
		string t = get_next_word(length);
		if(t.compare("") == 0)
			return;
		else
			block.push_back(t);
		t.clear();
	}
	return;
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
					///char x = conversion[next_branch];
					char x = 'A' + reverse_branch[next_branch];  // ljn 10/5/2009
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
							if(ret_word.length()==0 && next_branch >= ALPH)
							{
								last_loc[length-1] = NULL;
								last_word[length-1].clear();
								last_word[length-1] = "";
								return "";
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
				///char x = conversion[next_branch];
				char x = 'A' + reverse_branch[next_branch];  // ljn 10/5/2009
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
						if(ret_word.compare("") == 0 && (next_branch >= ALPH || next_branch < 0))
						{
							last_loc[length-1] = NULL;
							last_word[length-1].clear();
							last_word[length-1] = "";
							return "";
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

//systematically returns EVERY word stored in the trie at a given length from a provided root (could be the real root).  
//It is the caller's responsibility to verify that the word meets any requirements of the job (i.e. min_O and min_seqs before output)
string radix_trie::get_next_word(radix_trie_node *temp_root, int length)
{
	string ret_word = "";
	radix_trie_node *node = NULL;
	//if we haven't returned anything yet...
	if(last_ext_loc[thread_num()][length-1] == NULL && last_ext[thread_num()][length-1].compare("") == 0) 
	{
		//if we have a trie here...
		if(temp_root && temp_root->branch)	
		{
			node = temp_root;
			//hunt until we find a word long enough to be the "first"
			while(static_cast<int>(ret_word.length()) < length) 
			{
				//if we can keep following this branch
				if(node->branch && node->branch[next_branch_iterator[thread_num()]]) 
				{
					///char x = conversion[next_branch_iterator[thread_num()]];
					char x = 'A' + reverse_branch[next_branch_iterator[thread_num()]];  // ljn 10/5/2009
					ret_word += x;
					node = node->branch[next_branch_iterator[thread_num()]];
					next_branch_iterator[thread_num()] = 0;
				}
				//otherwise, gotta increment and look at the next branch
				else 
				{
					next_branch_iterator[thread_num()]++;
					if (next_branch_iterator[thread_num()] >= ALPH) {
						while (next_branch_iterator[thread_num()] >= ALPH) {
							char branch_id = ret_word[ret_word.length()-1];
							ret_word = ret_word.substr(0, ret_word.length()-1);
							next_branch_iterator[thread_num()] = locate_branch(branch_id) + 1;
							node = temp_root;
							for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
								if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
									node = node->branch[locate_branch(ret_word[i])];
								}
							}
							if(ret_word.length()==0 && next_branch_iterator[thread_num()] >= ALPH)
							{
								last_ext_loc[thread_num()][length-1] = NULL;
								last_ext[thread_num()][length-1].clear();
								last_ext[thread_num()][length-1] = "";
								return "";
							}
						}						
					}
				}
			}
			last_ext_loc[thread_num()][length-1] = node;
			last_ext[thread_num()][length-1] = ret_word;
			return ret_word;
		}
	}
	//otherwise we have something and can start from there...
	else {
		char branch_id = last_ext[thread_num()][length-1][last_ext[thread_num()][length-1].length()-1];
		ret_word = last_ext[thread_num()][length-1].substr(0, last_ext[thread_num()][length-1].length()-1);
		next_branch_iterator[thread_num()] = locate_branch(branch_id) + 1;
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
			if(node && node->branch && node->branch[next_branch_iterator[thread_num()]]) 
			{
				///char x = conversion[next_branch_iterator[thread_num()]];
				char x = 'A' + reverse_branch[next_branch_iterator[thread_num()]];  // ljn 10/5/2009
				ret_word += x;
				node = node->branch[next_branch_iterator[thread_num()]];
				next_branch_iterator[thread_num()] = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else 
			{
				next_branch_iterator[thread_num()]++;
				if (next_branch_iterator[thread_num()] >= ALPH) {
					char branch_id;
					while (next_branch_iterator[thread_num()] >= ALPH) {
						branch_id = ret_word[ret_word.length()-1];					
						ret_word = ret_word.substr(0, ret_word.length()-1);
						next_branch_iterator[thread_num()] = locate_branch(branch_id) + 1;
						node = temp_root;
						for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
							if(node && node->branch && node->branch[locate_branch(ret_word[i])]) {
								node = node->branch[locate_branch(ret_word[i])];
							}
						}
						if(ret_word.compare("") == 0 && (next_branch_iterator[thread_num()] >= ALPH || next_branch_iterator[thread_num()] < 0))
						{
							last_ext_loc[thread_num()][length-1] = NULL;
							last_ext[thread_num()][length-1].clear();
							last_ext[thread_num()][length-1] = "";
							return "";
						}
					}	
				}
			}
		}
	}	
	last_ext_loc[thread_num()][length-1] = node;
	last_ext[thread_num()][length-1] = ret_word;
	return ret_word;
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
	
	vector<string> seqs;
	get_seq_file(seqs);
	int h;
	
	for(h=0; h<static_cast<int>(seqs.size()); h++)
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
				for(int k=length; k>=1; k--)
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

// ljn added 10/5/2009
int radix_trie::locate_branch(char x)
{
	int branch_index = -1;
	x = toupper(x);
	branch_index = branch_array[x - 'A'];
	return branch_index;
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

int radix_trie::thread_num()
{
	#ifdef _OPENMP
	return omp_get_thread_num();
	#else
	return 0;
	#endif
}

int radix_trie::num_procs()
{
	#ifdef _OPENMP
	return omp_get_num_procs();
	#else
	return 1;
	#endif
}


