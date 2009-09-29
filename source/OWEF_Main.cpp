/*
**********************************************
// File:	OWEF_Main.cpp
//
// Author:	Kyle Kurz
//
// Date:	2.20.09
//
// Purpose:	Define a main function for use
//		in the Open Word
//		Enumeration Framework.
**********************************************

Copyright (C) 2009  Jens Lichtenberg, Kyle Kurz, Lonnie Welch, Frank Drews, Sophie Schbath, Mark
Hoebeke, Finn Drablos, Geir Kjetil Sandve, Lee Nau, Xiaoyu Liang, Rami Alouran, Matthew Wiley

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
   
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <vector>

#include "OWEF.h"

using namespace std;

#define EXIT_SUCCESS 0

int main(int argc, char *argv[])
{
	//counting stage variables
	bool count;
	string seq_file = "";
	int word_length = 0;
	int min_length = 0;
	int min_seqs = 1;
	int min_O = 1;
	bool ancestral_filter = 0;
	bool enumerate = 0;
	bool missing = 0;
	bool N_filter = 0;
  	bool record_seqs = 0;
	
	//scoring stage variables
	bool score = 0;
	int order = 0;
	bool revcomp = 0;
	bool pval = 0;
	bool pthr = 0;
	double pthresh = 0.05;
	
	//clustering stage variables
	bool cluster = 0;
	int seeds = 25;
	string sort;
	string type;
	int distance = 1;
	bool motif_score = 0;
	bool pwm = 0;
	bool logos = 0;
	bool regex = 0;
	
	//cooccurrence stage variables
	bool cooccur = 0;
	bool map = 0;
	string mout;
	bool cdist = 0;
	string dout;
	int group_size=2;
	bool self = 0;
	bool upper_lower = 0;
	
	//word distribution stage variables
	bool word_distribution = 0;
	int dist_count = 5;
	bool normalize = 0;
	
	//scatter plot variables
	bool scatter = 0;
	
	//sequence clustering variables
	bool sequence_clustering = 0;
		
	//conservation anaylsis variables
	
	//module discovery variables
	bool discovery = 0;
	bool fixed = 0;
	int dimensions = 0;
	bool self_compare = 0;
	bool distributed = 0;
	bool preserve_order = 0;
	bool density = 0;
	bool overlap_words = 0;
	
	//job variables
	string directory = "";
	string prefix = "";
	bool parallel = 0;
	
	//if we don't get command line options, prompt for user input
	if(argc == 1)
	{
		cout << "Enter a job ID: ";
		cin >> prefix;
		cout << "Perform counting? (enter 0 for no, 1 for yes): ";
		cin >> count;
		if(count)
		{
			cout << "Sequence file name: ";
			cin >> seq_file;
			cout << "Word length: ";
			cin >> word_length;
			cout << "Minimun word length: ";
			cin >> min_length;
			cout << "Minimum number of sequence occurrences: ";
			cin >> min_seqs;
			cout << "Minimum number of occurrences: ";
			cin >> min_O;
			cout << "Ancestral filter (enter 0 for no, 1 for yes): ";
			cin >> ancestral_filter;
			cout << "N filter (enter 0 for no, 1 for yes): ";
			cin >> N_filter;
			cout << "List missing words? (enter 0 for no, 1 for yes): ";
			cin >> missing;
			cout << "Enumerate missing words? (enter 0 for no, 1 for yes): ";
			cin >> enumerate;
			cout << "Record sets of sequences for word occurrence? (enter 0 for no, 1 for yes): ";
			cin >> record_seqs;
		}
		cout << "Perform scoring? (enter 0 for no, 1 for yes): ";
		cin >> score;
		if(score)
		{
			cout << "Markov model order: ";
			cin >> order;
			cout << "Perform Reverse compliment analysis? (enter 0 for no, 1 for yes): ";
			cin >> revcomp;
			cout << "Perform P-value analysis? (enter 0 for no, 1 for yes): ";
			cin >> pval;
			if(pval)
			{
				cout << "Set cutoff threshold for output based on P-values? (enter 0 for no, 1 for  yes): ";
				cin >> pthr;
				if(pthr)
				{
					cout << "Cutoff threshold: ";
					cin >> pthresh;
				}
			}
		}
		/*
		cout << "Perform clustering? (enter 0 for no, 1 for yes): ";
		cin >> cluster;
		if(cluster)
		{
			cout << "How many clusters? ";
			cin >> seeds;
			cout << "What score to sort by? (r = O/E, o = O*ln(O/E), s = S*ln(S/Es)) ";
			cin >> sort;
			cout << "What clustering method? (h = hamming, e = edit) ";
			cin >> type;
			cout << "What distance? ";
			cin >> distance;
			cout << "Score the motifs (clusters)? (enter 0 for no, 1 for yes): ";
			cin >> motif_score;
			cout << "Create Position Weight Matrices for clusters? (enter 0 for no, 1 for yes): ";
			cin >> pwm;
			cout << "Create logos for the motifs? (enter 0 for no, 1 for yes): ";
			cin >> logos;
			cout << "Create regular expressions for the motifs? (enter 0 for no, 1 for yes): ";
			cin >> regex;
		}
		cout << "Create scatter plots of scores? (enter 0 for no, 1 for yes): ";
		cin >> scatter;
		cout << "Perform word distribution analysis? (enter 0 for no, 1 for yes): ";
		cin >> word_distribution;
		if(word_distribution)
		{	
			cout << "Number of words to analyze: ";
			cin >> dist_count;
			cout << "Normalize distributions? (enter 0 for no, 1 for yes): ";
			cin >> normalize;
		}
		cout << "Perform cooccurrence analysis? (enter 0 for no, 1 for yes): ";
		cin >> cooccur;
		if(cooccur)
		{
			cout << "Map the coocurrence? (enter 0 for no, 1 for yes): ";
			cin >> map;
			if(map)
			{
				cout << "Filename for map: ";
				cin >> mout;
			}
			cout << "Map the distances? (enter 0 for no, 1 for yes): ";
			cin >> cdist;
			if(cdist)
			{
				cout << "Filename for distances: ";
				cin >> dout;
			}
			cout << "Number of words in a group: ";
			cin >> group_size;
			cout << "Compare each word to itself? (enter 0 for no, 1 for yes): ";
			cin >> self;
			cout << "Use case sensitive analysis? (enter 0 for no, 1 for yes): ";
			cin >> upper_lower;
		}
		cout << "Perform sequence clustering? (enter 0 for no, 1 for yes): ";
		cin >> sequence_clustering;
		cout << "Perfrom module discovery? (enter 0 for no, 1 for yes): ";
		cin >> discovery;
		if(discovery)
		{
			cout << "Fixed wordlength? (enter 0 for no, 1 for yes): ";
			cin >> fixed;
			cout << "How many words per module? ";
			cin >> dimensions;
			cout << "Use same word multiple times in a module? (enter 0 for no, 1 for yes): ";
			cin >> self_compare;
			cout << "Generate distance distribution? (enter 0 for no, 1 for yes): ";
			cin >> distributed;
			cout << "Preserve order of words in module? (enter 0 for no, 1 for yes): ";
			cin >> preserve_order;
			cout << "Generate density distribution? (enter 0 for no, 1 for yes): ";
			cin >> density;
			cout << "Consider overlapping words? (enter 0 for no, 1 for yes): ";
			cin >> overlap_words;
		}
		 */
	}
	//otherwise read the options and set flags
	else
	{
		vector<string> flags;
		flags.push_back("--count");
		flags.push_back("-i");
		flags.push_back("-l");
		flags.push_back("-ml");
		flags.push_back("-ms");
		flags.push_back("-mo");
		flags.push_back("-a");
		flags.push_back("-n");
		flags.push_back("-m");
		flags.push_back("-e");
		flags.push_back("-rs");
		flags.push_back("--score");
		flags.push_back("-o");
		flags.push_back("-r");
		flags.push_back("-p");
		flags.push_back("-pt");
		flags.push_back("--cluster");
		flags.push_back("-s");
		flags.push_back("-c");
		flags.push_back("-t");
		flags.push_back("-d");
		flags.push_back("-sm");
		flags.push_back("-pwm");
		flags.push_back("-logos");
		flags.push_back("-regex");
		flags.push_back("--scatter");
		flags.push_back("--distribution");
		flags.push_back("-dc");
		flags.push_back("-dn");
		flags.push_back("--cooccurrence");
		flags.push_back("-cm");
		flags.push_back("-cd");
		flags.push_back("-cn");
		flags.push_back("-cc");
		flags.push_back("-cs");
		flags.push_back("--sequence");
		flags.push_back("--modules");
		flags.push_back("-mef");
		flags.push_back("-med");
		flags.push_back("-mes");
		flags.push_back("-medd");
		flags.push_back("-mepo");
		flags.push_back("-mede");
		flags.push_back("-meow");
		flags.push_back("--prefix");
		flags.push_back("--parallel");
		flags.push_back("-h");
		

		for(int i=1; i<argc; i++)
		{
			int loc = -1;
			for(int j=0; j<static_cast<int>(flags.size()); j++)
			{
				if(strcmp(flags[j].c_str(), argv[i])==0)
					loc = j;
			}
			switch(loc)
			{
				case 0:
					count = true;
					break;
				case 1:
					seq_file = argv[++i];
					break;
				case 2:
					word_length = atoi(argv[++i]);
					break;
				case 3:
					min_length = atoi(argv[++i]);
					break;
				case 4:
					min_seqs = atoi(argv[++i]);
					break;
				case 5:
					min_O = atoi(argv[++i]);
					break;
				case 6:
					ancestral_filter = true;
					break;
				case 7:
					N_filter = true;
					break;
				case 8:
					missing = true;
					break;
				case 9:
					enumerate = true;
					break;
       				case 10:
          				record_seqs = true;
          				break;
				case 11:
					score = true;
					break;
				case 12:
					order = atoi(argv[++i]);
					break;
				case 13:
					revcomp = true;
					break;
				case 14:
					pval = true;
					break;
				case 15:
					pthr = true;
					pthresh = atof(argv[++i]);
					break;
				case 16:
					cluster = true;
					break;
				case 17:
					seeds = atoi(argv[++i]);
					break;
				case 18:
					sort = argv[++i];
					break;
				case 19:
					type = argv[++i];
					break;
				case 20:
					distance = atoi(argv[++i]);
					break;
				case 21:
					motif_score = true;
					break;
				case 22:
					pwm = true;
					break;
				case 23:
					logos = true;
					break;
				case 24:
					regex = true;
					break;
				case 25:
					scatter = true;
					break;
				case 26:
					word_distribution = true;
					break;
				case 27:
					dist_count = atoi(argv[++i]);
					break;
				case 28:
					normalize = true;
					break;
				case 29:
					cooccur = true;
					break;
				case 30:
					map = true;
					mout = argv[++i];
					break;
				case 31:
					cdist = true;
					dout = argv[++i];
					break;
				case 32:
					group_size = atoi(argv[++i]);
					break;
				case 33:
					self = true;
					break;
				case 34:
					upper_lower = true;
					break;
				case 35:
					sequence_clustering = true;
					break;
				case 36:
					discovery = true;
					break;
				case 37:
					fixed = true;
					break;
				case 38:
					dimensions = atoi(argv[++i]);
					break;
				case 39:
					self_compare = true;
					break;
				case 40:
					distributed = true;
					break;
				case 41:
					preserve_order = true;
					break;
				case 42:
					density = true;
					break;
				case 43:
					overlap_words = true;
					break;
				case 44:
					prefix = argv[++i];
					break;
				case 45:
					parallel = true;
					break;
				case 46:
					cout << endl << "Command options format: \n./OWEFexec [--count -i sequence_file -l word_length -ml minimum_length -ms minimum_sequences -mo minimum_occurrences <-a> <-n> <-m> <-e> <-rs>] [--score -o markov_order <-r> <-p>] [--cluster -s #_of_clusters -c sort_column -t cluster_type -d distance -sm] [--scatter] [--distribution <-dc number> <-dn>] [--cooccurrence <-cm map_name> <-cd dist_name> <-cn number> <-cc> <-cs>] [--prefix JobID]" << endl;
					cout << endl << "Counting stage options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --count : perform the counting stage" << endl;
					cout << "	-i filename (-i indicates input file, filename is the name of the file on your system)" << endl;
					cout << "	-l wordlength (-l indicates maximum word length, wordlength is an integer)" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "	-ml mimimum_length (-ml indicates the minimum wordlength (range), minimum_length is an integer)" << endl;
					cout << "	-ms minimum_sequences (-ms indicates minimum number of sequences, minimum_sequences is an integer)" << endl;
					cout << "	-mo minimum_occurrences (-mo indicates minimum_number of occurrences, minimum_occurrences is an integer)" << endl;
					cout << "	-a (-a indicates that the input is marked for ancestral repeats and should be filtered accordingly)" << endl;
					cout << "	-n (-n indicates that arbitrary nucleotides be filtered from the input)" << endl;
					cout << "	-m (-m causes the framework to list out words NOT found in the input file)" << endl;
					cout << "	-e (-e fully enumerates any missing word listed)" << endl;
					cout << " -rs (-rs record the set of sequences a word occurrs in)" << endl;
					cout << endl << "Scoring stage options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --score : perform the scoring stage" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "	-o markov_order (-o indicates the order, markov_order is an integer greater than 0 and less than wordlength)" << endl;
					cout << "	-r (-r adds reverse compliment analysis to the stage)" << endl;
					cout << "	-p (-p adds p-value analyisis to the stage)" << endl;
					cout << "	-pt threshold (-pt limits output from scoring to p-values below a certain cutoff, threshold is a double)" << endl;
					cout << endl << "Clustering stage options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --cluster : perform the clustering stage" << endl;
					cout << "	-s #_of_clusters (-s indicates seeds, #_of_clusters is an integer)" << endl;
					cout << "	-c sort_column (-c indicates column to sort by, sort_column is a character, r = O/E score, s = S*ln(S/Es), o = O*ln(O/Eo))" << endl;
					cout << "	-t cluster_type (-t indicates cluster distance method, cluster_type is a character, e = edit distance, h = hamming distance)" << endl;
					cout << "	-d distance (-d indicates maximum distance between two words in a cluster, distance is an integer)" << endl;
					cout << "	-sm (-sm add motif scoring and logo creation to the stage)" << endl;
					cout << "	-pwm (create Position Weight Matrices for the motifs)" << endl;
					cout << "	-logos (create logos for the motifs)" << endl;
					cout << "	-regex (create regular expressions for the motifs)" << endl;
					cout << endl << "Scatter plot options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --scatter" << endl;
					cout << endl << "Word distribution options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --distribution" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "	-dc number (-dc indicates the number of top words used for computing distributions, number is an integer)" << endl;
					cout << "	-dn (-dc normalizes the word distributions to a percentage through the sequences)" << endl;
					cout << endl << "CoOccurrence options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --cooccurrence" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "	-cm map_name (-cm maps the cooccurrence to a plot, map_name is a string for the filename)" << endl;
					cout << "	-cd dist_name (-cd creates a file with the distances between groups, dist_name is a string for the filename)" << endl;
					cout << "	-cn number (-cn is the number of words in a group, number is an integer)" << endl;
					cout << "	-cc (-cc indicates using a word twice in a group (cooccurrence with itself))" << endl;
					cout << "	-cs (-cs indicates case sensitivity for distinct words)" << endl;
					cout << endl << "Sequence clustering options:" << endl;
					cout << endl << "  Required:" << endl;
					cout << "    --sequence" << endl;
					cout << endl << "Module discovery options:" << endl;
					cout << "    --modules" << endl;
					cout << "	-mef (-mef uses fixed wordlength, if absent, the wordlength is variable)" << endl;
					cout << "	-med dimensions (-med uses dimensions to set the size of the modules created)" << endl;
					cout << "	-mes (-mes allows the module to include the same word multiple times)" << endl;
					cout << "	-medd (-medd creates a distance distribution file)" << endl;
					cout << "	-mepo (-mepo preserves the order of the words in the module when searching)" << endl;
					cout << "	-mede (-mede creates a density distribution file)" << endl;
					cout << "	-meow (-meow allows words in a module to overlap in position)" << endl;
					cout << endl << "General job options:" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "    --prefix JobID (--prefix places a prefix in front of every file created during the job, JobID is a string)" << endl << endl;
					cout << "    --parallel (run the job with parallelizations active)" << endl << endl;
					exit(0);
					break;
				default:
					cout << "Command options format: \n./OWEFexec [--count -i sequence_file -l word_length -ml minimum_length -ms minimum_sequences -mo minimum_occurrences <-a> <-n> <-m> <-e> <-rs>] [--score -o markov_order <-r> <-p> <-pt threshold>] [--cluster <-s #_of_clusters> <-c sort_column> <-t cluster_type> <-d distance> <-sm> <-pwm> <-logos> <-regex>] [--scatter] [--cooccurrence <-cm map_name> <-cd dist_name> <-cn number> <-cc> <-cs>] [--sequence] [--modules <-mef> <-med dimensions> <-mes> <-medd> <-mepo> <-mede> <-meow>][--prefix JobID] [--parallel]" << endl;
					exit(-1);
					break;
			}
		}
	}
	//set our prefix up to be the input filename without the extension (if we don't get a prefix on the command line)
	if(prefix == "")
	{
		prefix = seq_file;
		if (prefix.find(".") != string::npos)
			prefix.erase(prefix.find("."), prefix.length()-prefix.find("."));
	}
	
	if(min_length == 0)
		min_length = word_length;

	//set working directory to be the same as the prefix (or job id)
	directory = prefix;
	string job_log = prefix+"_output_files.txt";
	
	//*******************************************************************
	//create a radix trie based on all the options we just set up
	//developers should replace this with a constructor to the dataset
	//they are implementing
	//*******************************************************************
	owef_args *from_input;
	from_input = new owef_args(count, seq_file, word_length, min_length, min_seqs, min_O, ancestral_filter, N_filter, missing, enumerate, record_seqs, score, order, revcomp, pval, pthr, pthresh, cluster, seeds, sort[0], type[0], distance, motif_score, pwm, logos, regex, word_distribution, dist_count, normalize, cooccur, map, mout, cdist, dout, group_size, self, upper_lower, sequence_clustering, discovery, fixed, dimensions, self_compare, distributed, preserve_order, density, overlap_words, prefix, parallel, job_log);
	
	#ifdef KKURZ_MPI
	
	//*******************************************************************
	//initialize the mpi code to create the proper communication 
	//capabilities
	//*******************************************************************
	//cout << "setting up mpi" << endl;
	int rc = MPI_Init(&argc, &argv);
	if(rc != MPI_SUCCESS)
	{
		printf("Error starting MPI program. Terminating.\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
	}
	
	MPI_Comm_size(MPI_COMM_WORLD, &(from_input->numtasks));
	MPI_Comm_rank(MPI_COMM_WORLD, &(from_input->rank));
	printf("Number of tasks= %d My rank= %d\n", from_input->numtasks, from_input->rank);

	#endif
	
	from_input->write_logs();

	owef *job;
	job = new owef(from_input);
	
	//used to issue commands to the system
	string cmd;
	stringstream stream;
	int system_return = 0;
	
	//if we want scatter plots...
	if(scatter)
	{
		ofstream log(from_input->log_file.c_str(), ios::app);
		log << "Scatter Plot: " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << "_OlnOE.png" << endl;
		log << "Scatter Plot: " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << "_SlnSE.png" << endl;
		log.close();
		//run the scatter plots
		stream.clear();
		stream << "./OlnOE_scatter_plot.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << ".csv " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order;
		getline(stream, cmd);
		system_return = system(cmd.c_str());
	
		if(from_input->missing && from_input->enumerate)
		{
			stream.clear();
			stream << "./missing_scatter.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << "_missing_scored.csv " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order;
			getline(stream, cmd);
			system_return = system(cmd.c_str());
		}

		stream.clear();
		stream << "./SlnSE_scatter_plot.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << 	from_input->order << ".csv " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order;
		getline(stream, cmd);
		system_return = system(cmd.c_str());
		
		if(pval)
		{
			stream.clear();
			stream << "./pval_scatter.sh " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order << ".csv " << from_input->prefix << "_" << from_input->maxlength << "_" << from_input->order;
			getline(stream, cmd);
			system_return = system(cmd.c_str());
		}
	}
		
	cout << "Creating results directory...\n\n";
	//Create results directory:
	stream.clear();
	stream << directory;
	if(from_input->count)
		stream << "_" << from_input->maxlength;
	if(from_input->score)
		stream << "_" << from_input->order;
	if(from_input->cluster)
		stream << "_" << from_input->type << "_" << from_input->seeds;
	stream >> directory;

	stream.clear();
	stream << "mkdir " << directory;
	getline(stream, cmd);
	cout << cmd << endl;
	
	system_return = system (cmd.c_str());

	//move everything over at once
	stream.clear();
	stream << "mv " << prefix << "*.* ./" << directory << "/";
	getline(stream, cmd);
	system_return = system(cmd.c_str());

	cout << "Results stored in " << directory << "/\n";
	cout << "\nRun successful!\n";
	
	#ifdef KKURZ_MPI
	if(from_input->rank == 0)
	{
		for(int i=1; i<from_input->numtasks; i++)
		{
			char x = 'x';
			int rc = 0;
			rc = MPI_Send(&x, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
			//cout << "rc: " << rc << endl;
		}
		MPI_Finalize();
	}
	#endif
	return EXIT_SUCCESS;
}
