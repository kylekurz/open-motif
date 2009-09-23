/*
**********************************************
// File:	Edit_cluster.cpp
//
// Author:	Kyle Kurz
//
// Date:	4.22.09
//
// Purpose:	Define an Edit cluster class
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

#include "Edit_cluster.h"

using namespace std;

//************************************************************
//Constructors
//************************************************************

//standard constructor
edit_cluster::edit_cluster()
{
	list = NULL;
}

//standard destructor
edit_cluster::~edit_cluster()
{
	clusters.clear();
}
	
//initialized edit cluster
edit_cluster::edit_cluster(owef_args *from_input, data *structure, word_scoring *model)
{
	cout << "Clustering with Edit distance..." << endl << endl;
	//point to the input parameters
	list = from_input;
	//count the words
	build_clusters(structure, model);
}

//funtion to build my clusters
void edit_cluster::build_clusters(data *structure, word_scoring *model)
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
				if(edit_dist(clusters[j][0], next_word) <= list->distance)
					clusters[j].push_back(next_word);
			}
		}
	}
	if(!list->motif_score)
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

//get the edit distance between two words
int edit_cluster::edit_dist(const string & s1, const string & s2)
{
	int m = s1.length(); 
	int n = s2.length(); 

	// d is a table with m+1 rows and n+1 columns
	int i, j, cost; 
	vector<vector<int> > d (m+1);
   
	for(i = 0; i < m+1; i++)
		d[i].resize(n+1);

	for(i = 0; i < m+1; i++)
		d[i][0] = i;

	for(i = 0; i < n+1; i++)  
		d[0][i] = i;


	for(i	= 1; i < m+1; i++)
	{
		for(j = 1; j < n+1; j++)
		{
			if( s1[i-1] == s2[j-1])
				cost = 0;
			else
				cost = 1;
			d[i][j] = min3(d[i-1][j] + 1, d[i][j-1]+1, d[i-1][j-1]+cost);
		}
	}
  	return d[m][n];
}

#define MIN(a,b) ((a)>(b)?(b):(a))

int edit_cluster::min3(int a, int b, int c)
{
	return( MIN( MIN(a,b), MIN(b,c)));
}    

