
/*
**********************************************
// File:        Word_family.cpp
// Purpose:     The one with all the word families.
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
GNU General Public License for more details.You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Word_family.h"

#include <iostream>

//create_family(string w);

using namespace std;
using namespace rmes;

word_family::word_family()
{
	//no op
}

word_family::~word_family()
{
	//no op
}

word_family::word_family(owef_args *input_list,data *structure, word_scoring *model)
{
	list=input_list;
	
	cout << "Creating Word Families" << endl;	
  int no_n = 3;
  
  clock_t start,end;
  double duration_covar  = 0.0;
	double duration_create = 0.0;
	
	int num_files = (list->maxlength - list->minlength)+1;
	for(int i=0; i<num_files; i++)
	{
		stringstream stream_2;
	  ofstream ratio_file;
	  string file_name;
	  stream_2 <<  list->prefix << "_" << i+list->minlength << "_" << list->order << "_wordfamilies.csv";
	  stream_2 >> file_name;
		ratio_file.open(file_name.c_str());
  	ratio_file << "Motif,Ratio" << endl;
    
    vector<string> v;
    vector<string> families;
    
    int terminator = 0;
    
    if(0)
    {
    	terminator = 50;
    }
    else
    {
    	terminator = list->num_words[i+list->minlength-1];
    }
    
    start=clock();
  	for(int j=0; j < terminator; j++)
		{
	    string next_word = structure->get_next_word(i+list->minlength);
	    
	    for (int i=1; i<static_cast<int> (next_word.length()-1); i++) 
			{
				char x = next_word[i];
				next_word[i] = 'N';
				if(find(families.begin(), families.end(), next_word) == families.end())
				{	
					families.push_back(next_word);
				}
	    	
	    	next_word[i] = x;
	    }
	  }

		int temp_n = no_n;;
		if(no_n - i+list->minlength < 2)
		{
  		temp_n = no_n - i+list->minlength;
  	}
  	
  	while(temp_n > 1)
  	{
	  	for(int u=0; u < static_cast<int>(families.size()); u++)
			{
				string word = families[u];
		   	for (int v=1; v < static_cast<int> (word.length()-1); v++) 
				{
					char x = word[v];
					word[v] = 'N';
					
					if(find(families.begin(), families.end(), word) == families.end())
					{				
						families.push_back(word);
		    	}
		    	word[v] = x;
		    }
		  }
		  temp_n--;
	  }
	  end=clock();
	  
	  duration_create += (double)(end-start)/CLOCKS_PER_SEC;
	  
	  start=clock();
	  for(int j=0; j < static_cast<int>(families.size()); j++)
	  {    
	  	string motif = families[j];
	  	//cout << "\tScoring(" << motif << ")" << endl;
			double ratio = create_family(motif,structure, model, list->order);
			//cout << "Done Scoring" << endl;
			if(ratio != 0)
			{	
	   		ratio_file << motif << "," << ratio << endl;
	   		//cout << motif << "," << ratio << endl;
	   	}
	   	//cout << "Done with " << motif << endl;
  	}
  	//cout << "Done with Loop" << endl;
	  end=clock();
	  
	  duration_covar += (double)(end-start)/CLOCKS_PER_SEC;
	  
  	ratio_file.close();
  	//cout << "Done Printing" << endl;
  }
  cout << "Get Next Word took " << duration_create << " seconds" << endl;
  cout << "CoVariance took    " << duration_covar  << " seconds" << endl;
  
}


double word_family::create_family(string w,data *structure, word_scoring *model, int order) 
{
	double ratio = 0;
	
	if(w.length() > 2)
	{		
		double expect = 0;
		double var = 0;
		double covar = 0;
		double count = 0;
		vector<string> temp = structure->get_regex_matches(w);
		
		if(static_cast<int>(temp.size() > 1))
		{
			for(int i=0; i < static_cast<int>(temp.size()); i++)
			{
				scores *t = new scores;
				model->compute_scores(t, temp[i], structure, order);			
				
				expect += t->expect;
				var += t->variance;
				count += structure->get_count(temp[i]);
				
				for(int j=i+1; j < static_cast<int>(temp.size()); j++)
				{
					scores *s = new scores;
					model->compute_scores(s, temp[j], structure, order);
					double temp_covar = condAsCoVar(temp[i],temp[j],order,structure,t,s, model);
					//cout << temp[i] << " " << temp[j] << " " << temp_covar << " " << t->expect << " " << s->expect << endl;
					covar += temp_covar;
				}				
			}
			
			var += covar;
			//if(var < 0)
			//{
				//cout << "Word " << w << " Var " << var << " Expect " << expect << " Covar " << covar << endl;
				ratio = (count - expect) / std::sqrt(var); // Needs to be (O - E) / sqrt(V)
			//}
		}
  }
	return ratio;
}

double word_family::condAsCoVar(string w1, string w2, int m, data *structure, scores *t, scores *s, word_scoring *model)
{
  Word word1 = w1;
  Word word2 = w2;
	if(word1.number() > word2.number())
	{
		string temp = w1;
		w1 = w2;
		w2 = temp;
	}
	//cout << "W1: " << w1 << " W2 " << w2 << endl;
  double covar=0.0;
  
  if (w1.compare(w2) == 0) 
  {
    covar=t->variance;
  } 
  else 
  {
    double expectW1=t->expect;
    double expectW2=s->expect;
  	
    if (expectW1 != 0.0 && expectW2 != 0.0) 
    {
    	vector<string> known_words;
			vector<string> v;                   
      string tmp="";
      for (long baseIndex=0;baseIndex<Alphabet::alphabet->size(); baseIndex++)
      {      
          tmp=toupper(Alphabet::alphabet->character(baseIndex));
          v.push_back(tmp);
      }
                    
      for (int i = 0; i<static_cast<int> (w1.length() - m); i++) 
      {      	
				string sub1 = w1.substr(i, m + 1);
				if (find(known_words.begin(), known_words.end(), sub1) == known_words.end() && w2.find(sub1)!=string::npos) 
				{
        	known_words.push_back(sub1);
        }
      
	      string sub2 = w2.substr(i, m + 1);
				if (find(known_words.begin(), known_words.end(), sub2) == known_words.end() && w1.find(sub2)!=string::npos) 
				{
	        known_words.push_back(sub2);
	      }
      }
			
			for(int i=0; i < static_cast<int>(known_words.size()); i++)
			{
				string word = known_words[i];				
				int cw1 = 0;
				int cw2 = 0;
				
				int strict_cw1 = 0;
				int strict_cw2 = 0;
				
				int q1 = 0;

				string sub = word.substr(i, m);
				for (int j = 0; j < 4; j++) 
				{
					string temp = sub;
					temp.append(v[j]);
					q1 += structure->get_count(temp);
					for (int i = 0; i<static_cast<int> (w1.length() - m); i++) 
      		{ 
						string sub1 = w1.substr(i, m + 1);
						if (sub1.compare(temp) == 0)
						{
							cw1++;
						}
						if (sub1.compare(word) == 0)
						{
							strict_cw1++;
						}
						
						string sub2 = w2.substr(i, m + 1);
						if (sub2.compare(temp) == 0)
						{
							cw2++;
						}
						if (sub2.compare(word) == 0)
						{
							strict_cw2++;
						}
					}

				}				
				covar += ((double)cw1*(double)cw2) / (double)q1;
				
				covar -= ((double)(strict_cw1 / 4) * (double)(strict_cw2 / 4)) / (double) structure->get_count(word);
      }
      
      double countW2Seq = 0.0;
      double countW2W1  = 0.0;
      double countW1Seq = 0.0;
      double countW1W2  = 0.0;
			
      for (long baseIndex=0;baseIndex<Alphabet::alphabet->size();baseIndex++) 
      {
      	string temp1 = w1.substr(0,m);
      	string temp2 = w2.substr(0,m);
      	
      	temp1.append(v[baseIndex]);
      	temp2.append(v[baseIndex]);
      	
      	for (int i = 0; i<static_cast<int> (w1.length() - m); i++) 
    		{ 
					string sub1 = w2.substr(i, m + 1);
					if (sub1.compare(temp1) == 0)
					{
						countW2W1++;
					}
					
					string sub2 = w1.substr(i, m + 1);
					if (sub2.compare(temp2) == 0)
					{
						countW1W2++;
					}
				}
      	      	
        countW1Seq+=structure->get_count(temp1);
        //cout << "temp2:" << temp2 << endl;
        countW2Seq+=structure->get_count(temp2);
      }

			covar -= countW2W1 / countW1Seq; //Checked and count W1Seq is right
			covar -= countW1W2 / countW2Seq; //Checked and it is the same as countW2W1 / countW1Seq when the seqs are inverted

      if (w1.substr(0,m).compare(w2.substr(0,m)) == 0)
      {
        covar += 1.0 / countW1Seq;
			}
			
      covar *= expectW1 * expectW2;

      Word word1 = w1;
      Word word2 = w2;
			
      for (int d = m + 1 - (int)w1.length() ; d < (int)w1.length()-m ; d++) 
      {
	  		auto_ptr<Word> overlapWord(word1.overlaps(word2,d));
	  		if (overlapWord.get())
	  		{
	  			scores *u = new scores;
	  			
	  			std::stringstream stream_1;
			    string overlap;
			    stream_1.clear();
			    stream_1 << Word(*overlapWord.get());
			    stream_1 >> overlap;
			    
					model->compute_scores(u, overlap, structure, m);
	    		covar += u->expect;
	    	}
      }
    }
  }
  //cout << w1 << " vs " << w2 << " Covar " << covar << endl;	
  return covar;
}










