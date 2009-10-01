
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
  
	
	int num_files = (list->maxlength - list->minlength)+1;
	//cout << "num files: " << num_files << endl;
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
    
  	for(int j=0; j<list->num_words[i+list->minlength-1]; j++)
		{
	    string next_word = structure->get_next_word(i+list->minlength);	    
	    for (int i=1; i<static_cast<int> (next_word.length()-1); i++) 
			{
				char x = next_word[i];
				next_word[i] = 'N';
				
				if(find(v.begin(), v.end(), next_word) == v.end()) 
				{
					v.push_back(next_word);
	    		ratio_file << next_word << "," << create_family(next_word,structure, model, list->order) << endl;
	    	}
	    	
	    	next_word[i] = x;
	    }
  	}
  	ratio_file.close();
  }
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
		
		//cout << "size " << temp.size() << endl;
		
		for(int i=0; i < static_cast<int>(temp.size()); i++)
		{
			scores *t = new scores;
			model->compute_scores(t, temp[i], structure, order);			
			
			expect += t->expect;
			var += t->variance;
			count += structure->get_count(temp[i]);
			
			//Compute the Covariance
			for(int j=i+1; j < static_cast<int>(temp.size()); j++)
			{
				scores *s = new scores;
				model->compute_scores(s, temp[j], structure, order);
				covar += condAsCoVar(temp[i],temp[j],order,structure,t,s);				
				//cout << "Exploded Word: " << temp[i] << " vs. " <<  temp[j] << endl;
			}			
		}
		var += covar;
		
		//cout << "Word: " << w << " Expect: " << expect << " Variance: " << var << endl;
		
		ratio = expect / std::sqrt(var); // Needs to be (O - E) / sqrt(V)
  }
	return ratio;
}

double word_family::condAsCoVar(string w1, string w2, int m, data *structure, scores *t, scores *s)
{
	
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
        	//cout << "W1 " << sub1 << endl;
        }
      
	      string sub2 = w2.substr(i, m + 1);
				if (find(known_words.begin(), known_words.end(), sub2) == known_words.end() && w1.find(sub2)!=string::npos) 
				{
	        known_words.push_back(sub2);
	        //cout << "W2 " << sub2 << endl;
	      }
      }
			
			for(int i=0; i < static_cast<int>(known_words.size()); i++)
			{
				string word = known_words[i];
				//cout << "Known Words " << word << endl;
				//Count the subword in the two words cw1 and cw2
				int cw1 = 0;
				int cw2 = 0;
				int q1 = 0;
				for (int i = 0; i<static_cast<int> (w1.length() - m); i++) 
      	{      	
					string sub1 = w1.substr(i, m + 1);
					if (sub1.compare(word) == 0)
					{
						cw1++;
					}
					string sub2 = w2.substr(i, m + 1);
					if (sub2.compare(word) == 0)
					{
						cw2++;
					}
					
					//Count the subword in the sequences q1
					for (int i = 0; i<static_cast<int> (word.length() - m); i++) 
		      {      	
						string sub = word.substr(i, m);
						for (int j = 0; j < 4; j++) 
						{
							string temp = sub;
							temp.append(v[j]);
							q1 += structure->get_count(temp);
						}
					}
					
					cout << "Count W1 " << cw1 << endl;
			  	cout << "Count W2 " << cw2 << endl;
			  	cout << "Q1       " << q1  << endl;
				}				
				
				covar += (double)cw1*(double)cw2 / (double)q1;
      }
      /*
      long firstIndexW1 = w1.substring(0,m-1);
      long firstIndexW2 = w2.substring(0,m-1);
      double countW2Seq=0.0;
      double countW2W1=0.0;
      double countW1Seq=0.0;
      double countW1W2=0.0;
			*/
			
			/*
      for (long baseIndex=0;baseIndex<Alphabet::alphabet->size();baseIndex++) 
      {
        countW1W2+=wordCounterW2.wordCount(m+1,Alphabet::alphabet->factor()*firstIndexW1+baseIndex);
        countW1Seq+=c.wordCount(m+1,Alphabet::alphabet->factor()*firstIndexW1+baseIndex);
        countW2W1+=wordCounterW1.wordCount(m+1,Alphabet::alphabet->factor()*firstIndexW2+baseIndex);
        countW2Seq+=c.wordCount(m+1,Alphabet::alphabet->factor()*firstIndexW2+baseIndex);
      }

      covar -= countW2W1 /  countW2Seq;
      covar -= countW1W2 /countW1Seq ;

      if (firstIndexW1 == firstIndexW2)
      {
        covar += 1.0 / countW1Seq;
			}
			
      covar *= expectW1 * expectW2;

      for (long d=m+1-w1.length() ; d < w1.length()-m ; d++) 
      {
        auto_ptr<Word> overlapWord(w1.overlaps(w2,d));
        if (overlapWord.get())
        {
          covar += condAsExpect(*overlapWord.get(),m,c);
        }
      }*/
    }
  }
  cout << "Word1: " << w1 << " Word2: " << w2 << " Covar " << covar << endl;
  return covar;
}










