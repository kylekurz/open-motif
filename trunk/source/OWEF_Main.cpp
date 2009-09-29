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
	
	//word family stage variables
	bool fam = 0;
	
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
		cout << "Perform familying? (enter 0 for no, 1 for yes): ";
		cin >> fam;
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
		flags.push_back("--family");
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
					fam = true;
					break;
				case 17:
					prefix = argv[++i];
					break;
				case 18:
					parallel = true;
					break;
				case 19:
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
					cout << endl << "General job options:" << endl;
					cout << endl << "  Optional:" << endl;
					cout << "    --prefix JobID (--prefix places a prefix in front of every file created during the job, JobID is a string)" << endl << endl;
					cout << "    --parallel (run the job with parallelizations active)" << endl << endl;
					exit(0);
					break;
				default:
					cout << "Command options format: \n./OWEFexec [--count -i sequence_file -l word_length -ml minimum_length -ms minimum_sequences -mo minimum_occurrences <-a> <-n> <-m> <-e> <-rs>] [--score -o markov_order <-r> <-p> <-pt threshold>] [--prefix JobID] [--parallel]" << endl;
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
	from_input = new owef_args(count, seq_file, word_length, min_length, min_seqs, min_O, ancestral_filter, N_filter, missing, enumerate, record_seqs, score, order, revcomp, pval, pthr, pthresh, fam, prefix, parallel, job_log);
	
	from_input->write_logs();

	owef *job;
	job = new owef(from_input);
	
	//used to issue commands to the system
	string cmd;
	stringstream stream;
	int system_return = 0;
		
	cout << "Creating results directory...\n\n";
	//Create results directory:
	stream.clear();
	stream << directory;
	if(from_input->count)
		stream << "_" << from_input->maxlength;
	if(from_input->score)
		stream << "_" << from_input->order;
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
	
	return EXIT_SUCCESS;
}
