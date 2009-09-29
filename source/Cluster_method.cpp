/*
**********************************************
// File:	Cluster_method.cpp
//
// Author:	Kyle Kurz
//
// Date:	4.22.09
//
// Purpose:	Define a class for use
//		in clustering in the Open
//		Word Enumeration Framework.
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

#include "Cluster_method.h"

const int A_IND = 0;					//Index of row in pwm where A counts are stored
const int C_IND = 1;
const int G_IND = 2;
const int T_IND = 3;
const int ALPHA_SIZE = 4;				//Number of characters in alphabet

//standard constructor
cluster::cluster()
{
	//no-op
}

//standard destructor
cluster::~cluster()
{
	//no-op
}

void cluster::create_logos(data *structure, word_scoring *model)
{
	cout << "Creating motif logos..." << endl << endl;
	stringstream stream;
	stream <<  list->prefix << "_" << list->maxlength << "_" << list->order << "_clusters.csv";
	string filename;
	stream >> filename;
	ofstream log(list->log_file.c_str(), ios::app);
	log << "Cluster file: " << filename << endl;
	log.close();
	ofstream ofile(filename.c_str());
	ofile << "#Word,S,E_s,O,E,O/E,O*ln(O/E),S*ln(S/E_s)";
	if(list->revcomp)
		ofile << ",RevComp,Present,Palindrome";
	if(list->pval)
		ofile << ",Pval";
	ofile << endl << endl;
	//if(list->
	scores_ws *word = new scores_ws;
	for(int i=0; i<static_cast<int>(clusters.size()); i++)
	{
		ofile << "Cluster " << i+1 << endl;
		
		//stuff for motif logos
		double cluster_score = 0;
		cluster_score = score_cluster(clusters[i], structure, model);
		vector<vector<float> > pwm;
		pwm = compute_pwm(clusters[i], structure);
		if(list->regex)
			output_reg_exp(ofile, pwm);
		if(list->pwm)
			output_pwm(ofile, pwm);
		
		//ofile << cluster_score << endl;
		//create motif logo
		if(list->logos)
		{
			string logo_name("motif");
			stringstream logo_stream;
			logo_stream << list->prefix << "_" << logo_name << "_" << i+1 << ".png";
			logo_stream >> logo_name;
			create_motif_logo(pwm, logo_name);
		}
			
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
	
	
	return;
}

double cluster::score_cluster(vector<string> cluster, data *structure, word_scoring *model)
{
	int cluster_O = 0;
	double cluster_E = 0;

	for (int i = 0; i < static_cast<int>(cluster.size()); i++)
	{
		cluster_O += structure->get_count(cluster[i]);
		cluster_E += model->compute_E(cluster[i], structure, list->order);
	}
	double score = cluster_O / cluster_E;
	return score;
}

void cluster::output_reg_exp(ostream & out_file, const vector<vector<float> > & pwm)
{
	string letters;
	for (int j = 0; j < static_cast<int>(pwm[0].size()); ++j)
	{
		letters.clear();
		if (pwm[A_IND][j] > 0) letters += "A";
		if (pwm[C_IND][j] > 0) letters += "C";
		if (pwm[G_IND][j] > 0) letters += "G";
		if (pwm[T_IND][j] > 0) letters += "T";

		if (letters.length() > 1)
		{
			out_file << "[" << letters << "]";
		}
		else
		{
			out_file << letters;
		}
	}
	out_file << endl;
}

void cluster::output_pwm(ostream & out_file, const vector<vector<float> > & pwm)
{
	for (int i = 0; i < ALPHA_SIZE; ++i)
	{
		//out_file << "-";
		for (int j = 0; j < static_cast<int>(pwm[i].size()); ++j)
		{
			out_file << pwm[i][j];
			if (j < static_cast<int>(pwm[i].size()) - 1)
			{
				out_file << ',';
			}
		}
		out_file << endl;
	}
	out_file << endl;
}

vector<vector<float> > cluster::compute_pwm(vector<string> cluster_data, data *structure)
{
	vector<vector<float> > pwm;
	for (int i = 0; i < ALPHA_SIZE; ++i)
	{
		pwm.push_back(vector<float>(cluster_data[0].length()));
	}
	
	string word;

	for (int i = 0; i < static_cast<int>(cluster_data.size()); ++i)
	{
		word = cluster_data[i];
		
		for (int j = 0; j < static_cast<int>(word.length()); ++j)
		{
			switch(word[j])
			{
				case 'A':
					pwm[A_IND][j] += structure->get_count(cluster_data[i]);
					break;
				case 'C':
					pwm[C_IND][j] += structure->get_count(cluster_data[i]);
					break;
				case 'G':
					pwm[G_IND][j] += structure->get_count(cluster_data[i]);
					break;
				case 'T':
					pwm[T_IND][j] += structure->get_count(cluster_data[i]);
					break;
				default:
					cout << "Character other than <ACGT> in function compute_pwm! Ignoring character!\n";
			}
		}
	}
	
	float col_sum = 0;	//sum of values down any column of pwm

	for (int i = 0; i < ALPHA_SIZE; ++i)
	{
		col_sum += pwm[i][0];
	}
		
	for (int i = 0; i < static_cast<int>(pwm[0].size()); ++i)
	{
		for (int j = 0; j < ALPHA_SIZE; ++j)
		{
			pwm[j][i] /= col_sum;
		}
	}
	return pwm;
}

void cluster::create_motif_logo(const vector<vector<float> > & pwm, const string & filename)
{
	string cmd;
	stringstream stream;
	stream << '[';
	
	for (int i = 0; i < static_cast<int>(pwm.size()); ++i)
	{
		stream << '[';
		for (int j = 0; j < static_cast<int>(pwm[i].size()) - 1; ++j)
		{
			stream << pwm[i][j] << ',';
		}
		stream << pwm[i][pwm[i].size() - 1] << ']' << ',';
	}

	int system_return = 0;
	stream >> cmd;
	cmd[cmd.length() - 1] = ']';
	cmd.insert (0, "perl ./TFBS.pl ");
	cmd += " ";
	cmd += filename;
	cmd += " 2>> " + list->prefix + "_TFBS_log.txt";
	
	system_return = system(cmd.c_str());	//invoke perl module to create logo
	//cout<<"system retun:"<<system_return<<"cmd:"<<cmd<<endl;
}

