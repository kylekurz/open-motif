/*
**********************************************
// File:	Hamming_cluster.cpp
//
// Author:	Kyle Kurz
//
// Date:	4.22.09
//
// Purpose:	Define an Hamming cluster class
**********************************************
Copyright (C) 2009  Kyle Kurz, Lonnie Welch, Frank Drews, Lee Nau

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

#include "Hamming_cluster.h"

using namespace std;

//************************************************************
//Constructors
//************************************************************

//standard constructor
hamming_cluster::hamming_cluster()
{
	list = NULL;
}

//standard destructor
hamming_cluster::~hamming_cluster()
{
	clusters.clear();
}
	
//initialized hamming cluster
hamming_cluster::hamming_cluster(owef_args *from_input, data *structure, word_scoring *model)
{
	cout << "Clustering with Hamming distance..." << endl << endl;
	//point to the input parameters
	list = from_input;
	//count the words
	build_clusters(structure, model);
}

//funtion to build my clusters
void hamming_cluster::build_clusters(data *structure, word_scoring *model)
{
	vector<pair<string, scores> > my_seeds = model->get_seeds();
	for(int i=0; i<static_cast<int>(my_seeds.size()); i++)
	{
		vector<string> temp;
		temp.push_back(my_seeds[i].first);
		clusters.push_back(temp);
	}
	string next_word = "";
	for(int i=0; i<list->num_words[list->maxlength-1]; i++)
	{
		next_word = structure->get_next_word(list->maxlength);
		for(int j=0; j<static_cast<int>(clusters.size()); j++)
		{
			if(next_word.compare(clusters[j][0]) != 0)
			{
				
				if(hamming_dist(clusters[j][0], next_word) <= list->distance){
					
					clusters[j].push_back(next_word);
				}
			}
		}
	}
	if(!list->motif_score && !list->pwm && !list->logos && !list->regex)
	{
		stringstream stream;
		stream <<  list->prefix << "_" << list->maxlength << "_" << list->order << "_clusters.csv";
		string filename;
		stream >> filename;
		ofstream log(list->log_file.c_str(), ios::app);
		log << "Cluster file: " << filename << endl;
		log.close();
		ofstream ofile(filename.c_str());
		ofile << "#Word,S,E_s,O,E,O/E,O*ln(O/E),S*ln(S/E_s),RevComp,Present,Palindrome,Pval" << endl << endl;
		scores *word = new scores;
		for(int i=0; i<static_cast<int>(clusters.size()); i++)
		{
			ofile << "Cluster " << i+1 << endl;
			for(int j=0; j<static_cast<int>(clusters[i].size()); j++)
			{
				model->compute_scores(word, clusters[i][j], structure, list->order);
				//output the word and it's scores
				string converted = clusters[i][j];
				ofile << converted;
				ofile << "," << word->seqs << "," << word->E_s << "," << word->count << "," << word->E << "," << word->OE << "," << word->Oln << "," << word->Sln;
				if(list->revcomp)
				{
					ofile << "," << word->revcomp;
					if(word->revcomp_pres)
						ofile  << "," << "Yes";
					else
						ofile  << "," << "No";
					if(word->palindrome)
						ofile  << "," << "Yes";
					else
						ofile  << "," << "No";
				}
				if(list->pval)
				{
					ofile << "," << word->pval;
				}
				ofile << endl;
			}
			ofile << endl;
		}	
		ofile.close();
	}
	else
		create_logos(structure, model);
}

//get the hamming distance between two words
int hamming_cluster::hamming_dist(const string & s1, const string & s2)
{
	if (static_cast<int>(s1.length()) != static_cast<int>(s2.length()))
	{
		return -1;
	}
	
	int dist = 0;
	for (int i = 0; i < static_cast<int>(s1.length()); ++i)
	{
		if (s1[i] != s2[i])
			++dist;		
	}
	return dist;
}

