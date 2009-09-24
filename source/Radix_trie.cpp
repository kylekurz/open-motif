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
	flag=0;
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
	//vector<VSTRUCT> temp;
	//for(int i=0; i<list->maxlength; i++)
	//	last_word.push_back(temp);
	radix_trie_node *temp = NULL;
	string word = "";
	for(int i=0; i<list->maxlength; i++)
	{
		last_loc.push_back(temp);
		last_word.push_back(word);
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
/*
//function to get E value
double radix_trie::get_E(string motif)
{
	//radix trie must compute E value for the string
	throw(-1);
}

//function to get E_s value
double radix_trie::get_Es(string motif)
{
	//radix trie must compute Es value for the string
	throw(-1);
}

//function to get O*ln(O/E) value
double radix_trie::get_Oln(string motif)
{
	//radix trie must compute Oln value for the string
	throw(-1);
}

//funtion to get S*ln(O/E) value
double radix_trie::get_Sln(string motif)
{
	//radix trie must compute Sln value for the string
	throw(-1);
}

//function to get p-value
double radix_trie::get_pval(string motif)
{
	//radix trie must compute p value for the string
	throw(-1);
}

//function to get rank
int radix_trie::get_rank(string motif)
{
	//radix trie must compute rank value for the string
	throw(-1);
}
*/
#ifdef KKURZ_MPI
//function to get word count
int radix_trie::get_count(string motif)
{
	motif = convert(motif);
	int count = motif.length();
	int ret_count = 1;
	int dest = motif[0] - 'A';
	int ret, rc;
	MPI_Status *status = NULL;
	//compute all the necessary scores
	//cout << " count: count " << count << " dest " << dest << " motif " << motif << endl;
	if(dest != list->rank)
	{
		motif = unconvert(motif);
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
	motif = convert(motif);
	int count = motif.length();
	int ret_count = 1;
	int dest = motif[0] - 'A';
	int ret, rc;
	MPI_Status *status = NULL;
	//compute all the necessary scores
	//cout << "seqs: count " << count << " dest " << dest << " motif " << motif << endl;
	if(dest != list->rank)
	{
		motif = unconvert(motif);
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
	string temp = convert(motif);
	char *t = &temp[0];
	int l = temp.length();
	return trie_find(root, t, l);
}

//function to get sequence count
int radix_trie::get_seqs(string motif)
{
	string temp = convert(motif);
	char *t = &temp[0];
	int l = temp.length();
	return trie_find_s(root, t, l);
}
#endif

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
	int n_loc = motif.find('e');
	if(n_loc >= 0 && list->n_filter)
		return (-1);
	int N_loc = motif.find('E');
	if(N_loc >= 0 && list->n_filter)
		return (-1);
	if(list->anc_filter)
	{
		for(int i=0; i<static_cast<int>(motif.size()); i++)
		{
			if(motif[i]-'A' < 0 || motif[i]-'A' > 4)
				return(-1);
		}
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
		int idx = *s - 'A';
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
	int idx = *s-'A';
	if(!node->branch[idx])
		return (-1);
	return trie_get (node->branch[*s - 'A'], s + 1, length-1);
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
	return trie_find (node->branch[*s - 'A'], s + 1, length-1);
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
	return trie_find_s(node->branch[*s - 'A'], s + 1, length-1);
}

//function to delete the trie
void radix_trie::trie_clear (radix_trie_node * node)
{
	for(int i=0; i<5; i++)
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
	//cout << num_files << endl;
	ofstream ofiles[num_files];
	for(int i=0; i<num_files; i++)
	{
		//cout << i << endl;
		stringstream stream;
		stream <<  list->prefix << "_" << i+list->minlength << ".csv";
		string filename;
		stream >> filename;
		ofstream log(list->log_file.c_str(), ios::app);
		log << "Word Counting file: " << filename << endl;
		log.close();
		ofiles[i].open(filename.c_str());
		ofiles[i] << "#Word,Sequences,Occurs" << endl;
		//cout << list->min_O << ',' << list->min_seqs << endl;
		//cout << "max J: " << list->num_words[(i+list->minlength)-1] << endl;
		for(int j=0; j<list->num_words[(i+list->minlength)-1]; j++)
		{
			//cout << "J: " << j << endl;
			string to_out = get_next_word(i+list->minlength);
			//cout << "to_out: " << to_out << endl;
			if(get_count(to_out) >= list->min_O && get_seqs(to_out) >= list->min_seqs)
				ofiles[i] << to_out << ',' << get_seqs(to_out) << ',' << get_count(to_out) << endl;
		}
		//output(ofiles[i], root, "", i+list->minlength);
	}
}
//#ifdef KKURZ_MPI
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
					char x = 'A'+next_branch;
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
							next_branch = (branch_id - 'A') + 1;
							//not needed, new direction
							/*
							#ifdef KKURZ_MPI
							MPI_Status status;
							int rc, dest, i;
							string ret, to_send;
							stringstream str;
							str << length;
							str >> to_send;
							dest = ret_word[0] - 'A';
							if(ret_word.length() == 0)
							{
								if(next_branch != list->rank)
								{
									string motif = unconvert(ret_word);
									i = list->rank+1;
									str << length;
									str >> to_send;
									//cout << "first loop " << i << endl;
									while(i < ALPH-1 && ret_word.compare("") == 0)
									{
										rc = MPI_Send(&to_send, to_send.length(), MPI_CHAR, i, 4, MPI_COMM_WORLD);
										rc = MPI_Recv(&ret, length, MPI_CHAR, i, 4, MPI_COMM_WORLD, &status);
										ret_word = ret;
										if(ret_word.compare("") == 0)
											i++;
									}
									last_loc[length-1] = NULL;
									last_word[length-1] = ret_word;
									return unconvert(ret_word);
								}
							}
							#endif
							*/
							node = root;
							for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
								if(node && node->branch && node->branch[ret_word[i] - 'A']) {
									node = node->branch[ret_word[i] - 'A'];
								}
							}
						}						
					}
				}
			}
			last_loc[length-1] = node;
			last_word[length-1] = ret_word;
			return unconvert(ret_word);
		}
	}
	//otherwise we have something and can start from there...
	else {
		//cout << "in else " << last_word[length-1] << endl;
		//not needed, new direction
		/*
		#ifdef KKURZ_MPI
		MPI_Status status;
		int rc, dest, i;
		string ret, to_send;
		stringstream str;
		str << length;
		str >> to_send;
		dest = last_word[length-1][0] - 'A';
		if (dest != list->rank ) {
			string motif = unconvert(ret_word);
			i = list->rank+1;
			//cout << "second loop " << i << endl;
			while(i < ALPH-1 && ret_word.compare("") == 0)
			{
				rc = MPI_Send(&to_send, to_send.length(), MPI_CHAR, i, 4, MPI_COMM_WORLD);
				rc = MPI_Recv(&ret, length, MPI_CHAR, i, 4, MPI_COMM_WORLD, &status);
				ret_word = ret;
				if(ret_word.compare("") == 0)
					i++;
			}
			last_loc[length-1] = NULL;
			last_word[length-1] = ret_word;
			return unconvert(ret_word);
		}
		#endif
		*/
		char branch_id = last_word[length-1][last_word[length-1].length()-1];
		ret_word = last_word[length-1].substr(0, last_word[length-1].length()-1);
		next_branch = (branch_id - 'A') + 1;
		//cout << "ret word " << ret_word << " next branch " << next_branch << endl;
		node = root;
		for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
			if(node && node->branch && node->branch[ret_word[i] - 'A']) {
				node = node->branch[ret_word[i] - 'A'];
			}
		}
		//hunt until we find a word long enough to be the "first"
		while(static_cast<int>(ret_word.length()) < length) 
		{
			//cout << "in while" << next_branch << endl;
			//if we can keep following this branch
			//cout << next_branch << " " << branch_id << endl;
			if(node && node->branch && node->branch[next_branch]) 
			{
				//cout << "in if" << endl;
				char x = 'A'+next_branch;
				ret_word += x;
				//cout << "later loop " << ret_word << endl;
				node = node->branch[next_branch];
				next_branch = 0;
			}
			//otherwise, gotta increment and look at the next branch
			else 
			{
				//cout << "in else" << endl;
				next_branch++;
				if (next_branch >= ALPH) {
					char branch_id;
					while (next_branch >= ALPH) {
						branch_id = ret_word[ret_word.length()-1];					
						ret_word = ret_word.substr(0, ret_word.length()-1);
						//cout << "new sub " << ret_word << endl;
						next_branch = (branch_id - 'A') + 1;
						//cout << "HERE " << branch_id << endl;
						//not needed, new direction
						/*
						#ifdef KKURZ_MPI
						if(ret_word.length() == 0)
						{
							if(next_branch != list->rank)
							{
								string motif = unconvert(ret_word);
								i = list->rank+1;
								str << length;
								str >> to_send;
								while(i < ALPH-1 && ret_word.compare("") == 0)
								{
									//cout << "second inner loop " << i << endl;
									rc = MPI_Send(&to_send, to_send.length(), MPI_CHAR, i, 4, MPI_COMM_WORLD);
									rc = MPI_Recv(&ret, length, MPI_CHAR, i, 4, MPI_COMM_WORLD, &status);
									ret_word = ret;
									if(ret_word.compare("") == 0)
										i++;
								}
								last_loc[length-1] = NULL;
								last_word[length-1] = ret_word;
								return unconvert(ret_word);
							}
						}
						#endif
						*/
						node = root;
						for (int i=0; i<static_cast<int>(ret_word.length()); i++) {
							if(node && node->branch && node->branch[ret_word[i] - 'A']) {
								node = node->branch[ret_word[i] - 'A'];
							}
						}
					}	
				}
			}
		}
	}	
	last_loc[length-1] = node;
	last_word[length-1] = ret_word;
	return unconvert(ret_word);
}

//function to iterate through the subtree and find all occurring words of length x, //from a given seed of length y string 
string radix_trie::get_next_extension(string seed, int length)
{
	seed = convert(seed);
	cout << "extension word = " << seed << endl;
	if(flag)
	{
		flag = 0;
		return "AAA";
	}
	else
		return "";
}

//function to mutate a string to the next physical permutation
void radix_trie::mutate(string &previous_word)
{
	if(list->n_filter)
	{
		int end = list->maxlength-1;
		if(previous_word[end] != 'D')
		{
			switch(previous_word[end])
			{
				case 'A':
					previous_word[end]='B';
					break;
				case 'B':
					previous_word[end]='C';
					break;
				case 'C':
					previous_word[end]='D';
					break;
			}
		}
		else
		{
			for(int i=end; i>=0; --i)
			{
				if(previous_word[i]=='D')
					previous_word[i]='A';
				else
				{
					switch(previous_word[i])
					{
						case 'A':
							previous_word[i]='B';
							break;
						case 'B':
							previous_word[i]='C';
							break;
						case 'C':
							previous_word[i]='D';
							break;
					}
					i=0;
				}
			}
		}
		return;
	}
	else
	{
		int end = list->maxlength-1;
		if(previous_word[end] != 'E')
		{
			switch(previous_word[end])
			{
				case 'A':
					previous_word[end]='B';
					break;
				case 'B':
					previous_word[end]='C';
					break;
				case 'C':
					previous_word[end]='D';
					break;
				case 'D':
					previous_word[end]='E';
					break;
			}
		}
		else
		{
			for(int i=end; i>=0; --i)
			{
				if(previous_word[i]=='E')
					previous_word[i]='A';
				else
				{
					switch(previous_word[i])
					{
						case 'A':
							previous_word[i]='B';
							break;
						case 'B':
							previous_word[i]='C';
							break;
						case 'C':
							previous_word[i]='D';
							break;
						case 'D':
							previous_word[i]='E';
							break;
					}
					i=0;
				}
			}
		}
		return;
	}
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

//convert all characters in a word to upper_case
string radix_trie::convert(string check)
{
	//cout << "converting" << endl;
	for(int i=0; i<static_cast<int>(check.length()); i++)
	{
		if(!(list->anc_filter))
		{
			switch(check[i])
			{
				case 'a':
					check[i]='A';
					break;
				case 'c':
					check[i]='B';
					break;
				case 'g':
					check[i]='C';
					break;
				case 't':
					check[i]='D';
					break;
				case 'n':
					check[i]='E';
					break;
				case 'A':
					break;
				case 'C':
					check[i]='B';
					break;
				case 'G':
					check[i]='C';
					break;
				case 'T':
					check[i]='D';
					break;
				case 'N':
					check[i]='E';
					break;
				default:
					check[i]='E';
					break;
			}
		}
		else
		{
			switch(check[i])
			{
				case 'a':
					break;
				case 'c':
					check[i]='b';
					break;
				case 'g':
					check[i]='c';
					break;
				case 't':
					check[i]='d';
					break;
				case 'n':
					check[i]='e';
					break;
				case 'A':
					break;
				case 'C':
					check[i]='B';
					break;
				case 'G':
					check[i]='C';
					break;
				case 'T':
					check[i]='D';
					break;
				case 'N':
					check[i]='E';
					break;
				default:
					check[i]='E';
					break;
			}
		}
	}	
	//cout << "returning" << endl;
	return check;
}

//convert all characters in a word to upper_case
string radix_trie::unconvert(string check)
{
	for(int i=0; i<static_cast<int>(check.length()); i++)
	{
		switch(check[i])
		{
			case 'A':
				break;
			case 'B':
				check[i]='C';
				break;
			case 'C':
				check[i]='G';
				break;
			case 'D':
				check[i]='T';
				break;
			case 'E':
				check[i]='N';
				break;
			default:
				check[i]='N';
				break;
		}
	}	
	return check;
}

//#ifdef KKURZ_MPI
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
		tag = convert(seqs[h]);
		//cout << "rank " << list->rank << " char " << tag[0] << endl;
		//cout << "sub " << (tag[0] - 'A') << endl;
		for(int i=0; i<(static_cast<int>(tag.length())); i++)
		{
			#ifdef KKURZ_MPI
			if((tag[i] - 'A') != list->rank)
				continue;
			else
			{
			#endif
				int length = min(static_cast<int>(tag.length())-i,list->maxlength);
				//cout << "length " << length << endl;
				for(int k=1; k<=length; k++)
				{
					string to_count = tag.substr(i, k);
					//cout << "to count " << to_count << endl;
					inc_count(to_count);
				}
			#ifdef KKURZ_MPI
			}
			#endif
		}
		list->seq++;
		tag.clear();
	}
	//cout << "rank " << list->rank << endl;
	//if(!(list->score))
	//	output();
	//in.close();
	//cout << "rank " << list->rank << endl;
	for(int i=0; i<list->maxlength; i++)
	{
		vector<string> tmp;
		breadth_search.push_back(tmp);
	}
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

