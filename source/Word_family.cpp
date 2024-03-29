
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

#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <memory>
#include <numeric>
#include "Word_family.h"

#define BLOCK_SIZE 500

using namespace std;
using namespace rmes;

extern "C" void *processor(void *);

//structure for passing to parallel processor
typedef struct args{
	//thread identifier
	int thr_id;
	
	//job variables
	data *structure;
	word_scoring *model;
	owef_args *my_list;
	
	//shared file variable
	ofstream ratio_file;
	
	//my data
	vector<string> my_data;
}args;

typedef std::vector<double> doublevec;
typedef std::vector<double> vecdouble;
typedef vector<short> shortvec;
typedef vector<shortvec> shortmat;

void *processor(void *_Object)
{
	args *p = (args *)_Object;
	for(int i=0; i<static_cast<int>(p->my_data.size()); i++)
	{
		//process the word
		
		//output the ratio
	}
	return NULL;
}

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
	
	//Computing sequence length
	int nseq = 0;
	vector<string> seqs;
	structure->get_seq_file(seqs);
	for(int i = 0; i < static_cast<int>(seqs.size());i++)
	{
		nseq += seqs[i].length();
	}
	cout << "Sequence Length " << nseq << endl;
	//int no_n = 3;
	cout << "Creating Word Families with " << list->no_n << " ambigous nucleotides." << endl;	
	//int nprocs = omp_get_num_procs();
	//cout << "number of processors: " << nprocs << endl;
	 
 //   	clock_t start,end;
  	//double duration_covar  = 0.0;
//	double duration_create = 0.0;
	
	int num_files = (list->maxlength - list->minlength)+1;
	int fam_number = 0;
	int i;
//	start=clock();
	for(i=0; i<num_files; i++)
	{
		stringstream stream_2;
	  	ofstream ratio_file;
	  	string file_name;
	  	stream_2 <<  list->prefix << "_" << i+list->minlength << "_" << list->order << "_wordfamilies.csv";
	  	stream_2 >> file_name;
		ratio_file.open(file_name.c_str());
  		ratio_file << "Motif,Ratio,CpRatio" << endl;
    
    		vector<string> v;
    		vector<string> families;
    
    		int threadID, totalThreads, j;
    		totalThreads = 0;
    		threadID = 0;
    		radix_trie *family_structure = new radix_trie(list, 1);
    		
    		vector<pair<string, scores> > word_list;
    		model->get_seeds(word_list);
    		
    		//for(int i=
    		
    		printf("word list count: %d\n", word_list.size());
    		
    		#ifdef _OPENMP
    		#pragma omp parallel for default(none) shared(nseq,ratio_file, i, structure, model, family_structure, word_list) private(j, threadID, families)
    		#endif
		for(j=0; j<static_cast<int>(word_list.size()); j++)
		{
			//printf("word: %s\n", word_list[j].first.c_str());
			int temp_n = list->no_n;
			if(list->no_n - i+list->minlength < 2)
  				temp_n = list->no_n - i+list->minlength;
			if(word_list[j].first.compare("") != 0)
  			{
				for (int k=1; k<static_cast<int> (word_list[j].first.length()-1); k++) 
				{
					for(int l=0; l<temp_n; l++)
					{
						if(k+l < static_cast<int>(word_list[j].first.length())-1)
						{
							string copy = word_list[j].first;
							copy.replace(k, l+1, l+1, 'N');
							if(family_structure->get_count(copy) == 0)
							{
								#ifdef _OPENMP
								#pragma omp critical
								{
								#endif
									family_structure->inc_count(copy);
								#ifdef _OPENMP
								}
								#endif
								vector<double> ratio = create_family(copy, structure, model, list->order, nseq);
								if(ratio.size() > 0)
								{
									#ifdef _OPENMP
									#pragma omp critical
									{
										ratio_file << copy << ',' << ratio[0] << ',' << ratio[1] << endl;
									}
									#else
									ratio_file << copy << ',' << ratio[0] << ',' << ratio[1] << endl;
									#endif
								}
							}
						}
	    				}
	    			}
	    		}
		}
		
		/*int num_blocks = (i + list->num_words[list->minlength-1]/BLOCK_SIZE) + 1;
		
		#ifdef _OPENMP
    		#pragma omp parallel for default(none) shared(nseq,ratio_file, i, structure, model, family_structure, num_blocks) private(j, threadID, families)
    		#endif
		for(j=0; j <num_blocks; j++)
		{
			vector<double> ratios;
			//printf("getting words\n");
			vector<string> word_list;
    			
    			#ifdef _OPENMP
    			#pragma omp critical
    			{
	    			structure->get_next_word_block(word_list, i+list->minlength, BLOCK_SIZE);
			}
			#else
			structure->get_next_word_block(word_list, i+list->minlength, BLOCK_SIZE);
			#endif
			
			int temp_n = list->no_n;
			if(list->no_n - i+list->minlength < 2)
  				temp_n = list->no_n - i+list->minlength;
	  		for(int x=0; x<static_cast<int>(word_list.size()); x++)
	  		{
	  			if(word_list[x].compare("") != 0)
	  			{
					for (int k=1; k<static_cast<int> (word_list[x].length()-1); k++) 
					{
						for(int l=0; l<temp_n; l++)
						{
							if(k+l < static_cast<int>(word_list[x].length())-1)
							{
								string copy = word_list[x];
								copy.replace(k, l+1, l+1, 'N');
								if(family_structure->get_count(copy) == 0)
								{
									family_structure->inc_count(copy);
									vector<double> ratio = create_family(copy, structure, model, list->order, nseq);
									if(ratio.size() > 0)
									{
										#ifdef _OPENMP
										#pragma omp critical
										{
											ratio_file << copy << ',' << ratio[0] << ',' << ratio[1] << endl;
										}
										#else
										ratio_file << copy << ',' << ratio[0] << ',' << ratio[1] << endl;
										#endif
									}
								}
							}
		    				}
		    			}
		    		}
	    		}
	    		word_list.clear();
  		}*/
  		ratio_file.close();
  		fam_number += families.size();
  	}
}


vector<double> word_family::create_family(string w,data *structure, word_scoring *model, int order, int nseq) 
{	
	//cout << "Family " << w << endl;
	vector<double> fam_scores;
	double ratio = 0;
	double score_cp = 0.0;
	
	if(w.length() > 2)
	{	
		double expect = 0;
		double var = 0;
		double covar = 0;
		double count = 0;
		double proba = 0.0;
		int sign = 0;
		
		vector<string> temp;
		structure->get_regex_matches(temp, w);
		if(static_cast<int>(temp.size() > 1))
		{
			for(int i=0; i < static_cast<int>(temp.size()); i++)
			{
				scores *t = NULL;
				t = structure->get_stats(temp[i]);
				if(t == NULL || t->expect == -1 || t->variance == -1)
				{
					t = new scores;
					model->compute_scores(t, temp[i], structure, order);
					structure->set_stats(temp[i], t);
				}	
				
				expect += t->expect;
				var += t->variance;
				int tmp = structure->get_count(temp[i]);
				if(tmp < 0)
					tmp = 0 - tmp;
				count += tmp;
				for(int j=i+1; j < static_cast<int>(temp.size()); j++)
				{
					scores *s = NULL;
					s = structure->get_stats(temp[j]);
					if(s == NULL || s->expect == -1 || s->variance == -1)
					{
						model->compute_scores(s, temp[j], structure, order);
						structure->set_stats(temp[j], s);
					}		
					double temp_covar = condAsCoVar(temp[i],temp[j],order,structure,t,s, model);
					covar += temp_covar;
				}				
			}
			
			var += covar;
			ratio = (count - expect) / std::sqrt(var); // Needs to be (O - E) / sqrt(V)
  		
  		/*
  		 * Compound Poisson Family Scoring
  		 */
  		
	  	//Integration STATUS
	  	long wordsPerFamily=temp.size();
	  	
		  vector<shortmat> periodpp(wordsPerFamily);
		  
		  for(vector<shortmat>::iterator mit=periodpp.begin(); mit != periodpp.end(); mit++) 
		  {
		    (*mit).resize(wordsPerFamily);
		    for (vector<shortvec>::iterator vit=(*mit).begin(); vit != (*mit).end(); vit++)
					(*vit).resize(w.length()-1);
		  }			

  		vector<double> espmots(wordsPerFamily);
  		vector<doublevec> A(wordsPerFamily);
			
			for(vector<doublevec>::iterator it=A.begin(); it != A.end(); it++) 
    		(*it).resize(wordsPerFamily);
    	    	    	
	  	for (long wordOneIndex=0; wordOneIndex < wordsPerFamily; wordOneIndex++) 
	    {
	    	scores *t = NULL;
				t = structure->get_stats(temp[wordOneIndex]);
				espmots[wordOneIndex]=t->expect;
	
				for (long wordTwoIndex=0; wordTwoIndex<wordsPerFamily; wordTwoIndex++) 
				{
				  for (short d=1; d < (short)w.length(); d++) 
				  {
				    periodpp[wordOneIndex][wordTwoIndex][d-1]=0;
				    Word w1 = temp[wordOneIndex];
				    Word w2= temp[wordTwoIndex];
				    auto_ptr<Word> overlapWord(w1.overlaps(w2,d));
	    			if (overlapWord.get()) 
	    			{
	      			bool fail=false;
	      			if (d>1) 
	      			{
								for (short wordThreeIndex=0; wordThreeIndex<wordsPerFamily;wordThreeIndex++) 
								{
		  						for (short d2=1; d2<d; d2++) 
		  						{
		  							Word w3 = temp[wordThreeIndex];
								    auto_ptr<Word> overlapWordTwo(w1.overlaps(w3,d2));
								    auto_ptr<Word> overlapWordThree(w3.overlaps(w2,d-d2));
		    						if (overlapWordTwo.get() && overlapWordThree.get()) 
		    						{
								      fail=true;
								      break;
		    						}
		  						}
								}
	      			}
	      			
	      			if (!fail) 
								periodpp[wordOneIndex][wordTwoIndex][d-1]=1;	      
	    			}	    
	  			}
				}
	    }
				    		
	    for (long wordOneIndex=0; wordOneIndex<wordsPerFamily;wordOneIndex++) 
	    {		
				for (long wordTwoIndex=0; wordTwoIndex<wordsPerFamily;wordTwoIndex++) 
				{
					
					//Here be seg fault
				  A[wordOneIndex][wordTwoIndex]=0.0;
				  short maux=order;
				  if (!maux)
	    			maux=1;
	  
	    		
	  			for (short d=1; d<=(short)w.length()-maux;d++) 
	  			{
	    			if (periodpp[wordOneIndex][wordTwoIndex][d-1]) 
	    			{	
	    				string subword = temp[wordOneIndex].substr(0,order+d-1);   	    				 					    				
	    				scores *z;
							z = structure->get_stats(subword);	
							double ex = 0;
							if(z == NULL)
							{
								z = new scores();
								int mo = 0;
								model->compute_scores(z, subword, structure, mo);
								ex = z->expect;
							}												
							
	      			A[wordOneIndex][wordTwoIndex] += ex;
	    			}
	  			}
	  
				  if (order == 0)
				    A[wordOneIndex][wordTwoIndex] /= double(nseq);
				  else {
				    double denominator=0.0;
				    for (short baseIndex=0;baseIndex<Alphabet::alphabet->size();baseIndex++)
				      denominator += structure->get_count(temp[wordTwoIndex].substr(0,order-1));
				    A[wordOneIndex][wordTwoIndex] /= denominator;
				  }
				  
				}
				
	    }
	    
	  	

	  	vector<vecdouble> ImoinsA(wordsPerFamily);
	    for (vector<vecdouble>::iterator it=ImoinsA.begin();it != ImoinsA.end(); it++)
				(*it).resize(wordsPerFamily);
	    
	    for (long wordOneIndex=0; wordOneIndex < wordsPerFamily;wordOneIndex++)
				for (long wordTwoIndex=0; wordTwoIndex < wordsPerFamily;wordTwoIndex++)
		  		if (wordOneIndex == wordTwoIndex)
		    		ImoinsA[wordOneIndex][wordTwoIndex] = 1 - A[wordOneIndex][wordTwoIndex];
		  		else
		    		ImoinsA[wordOneIndex][wordTwoIndex] = - A[wordOneIndex][wordTwoIndex];
	  	
	  	vector<double> aux(wordsPerFamily);
	    computeProduct(&ImoinsA,&espmots,&aux);
	    double lambda=accumulate(aux.begin(),aux.end(),0.0);
	    var=lambda;
	  	
	  	long xmax=count;
	    vector<double> lambdak(1+xmax);
	  	vector<double> auxbis(wordsPerFamily);
	  	
	    computeProduct(&ImoinsA,&aux,&auxbis);
	    copy(auxbis.begin(),auxbis.end(),aux.begin());
	    for(vector<double>::iterator it=lambdak.begin()+1;it != lambdak.end(); it++) 
	    {
				*it = accumulate(aux.begin(),aux.end(),0.0);
				computeProduct(&A,&aux,&auxbis);
				copy(auxbis.begin(),auxbis.end(),aux.begin());
	    }
	  	//Integration DONE
	  	vector<vecdouble> D(1+xmax);
	  	
	    for (vector<vecdouble>::iterator it=D.begin(); it != D.end(); it++)
				it->resize(1+xmax);
	    for (long x=0; x<=xmax; x++)
				for (long y=0; y<=xmax; y++)
				  if (x!=y || x==0)
				    D[x][y]=0;
				  else
				    D[x][x] = exp(-lambda)/double(x);
	
	    for (long y=1; y<=xmax; y++)
				for (long x=y+1; x<=xmax; x++) 
				{
					double sum=0.0;
					for (long k=1; k <= x-y; k++)
					  sum += lambdak[k] * D[x-k][y] * double(k)/double(x);
					D[x][y]=sum;
				}
				
	  	//What is Loi
	  	vector<double> loi(xmax+1);
	    loi[0]=exp(-lambda);
	    for (long x=1; x<=xmax; x++) 
	    {
				double sum=0.0;
				for (long k=1; k<=x; k++) 
	  			sum += k * lambdak[k] * D[x][k];
				loi[x] = sum; 
	    }	
	    
	  	if(count <= expect)
	  	{
	  		proba = 0.0;
	  		sign = -1;
	  		sort(loi.begin(),loi.end());
				
				if (loi.back() <= numeric_limits<double>::min()) 
				{
		  		cerr << "Family " << w << " has a p-value too close to 1." << endl << "Hence, it is significantly rare." << endl;
		  		
		  		score_cp = -1.0 * numeric_limits<double>::infinity();
				} 
				else 
				{
		  		for (vector<double>::iterator it=loi.begin();it != loi.end(); it++)
		    		if (*it <= 1)
		      		proba+=*it;
		    		else 
		    		{
		      		cerr << "Warning: p-value > 1 for " << w << ": it is however a very under-represented family." << endl;
		      		break;
		    		}
				}
	  	}
	  	else
	  	{
	  		proba = 1.0;
	  		sign = +1;
	  		
	  		loi.pop_back();
				
				sort(loi.begin(),loi.end());
				for (vector<double>::reverse_iterator rit=loi.rbegin(); rit != loi.rend(); rit++)
				{
		    	if (proba - *rit >= 0)
		    	{
		      	proba -= *rit;
		    	}
		  		else 
		  		{
		    		cerr << "Warning: p-value < 0 for "<< w << ": it is however a very over-represented family."<< endl;
		    		break;
		  		}
	  		}
	  	}
	  	
		  if (proba != numeric_limits<double>::infinity())
		  {
				score_cp = sign * model->quantile(proba);
			}
			else
			{
				score_cp = sign * numeric_limits<double>::infinity();
		  }
		  
		  temp.clear();
		}
	}
	
	fam_scores.push_back(ratio);
	fam_scores.push_back(score_cp);
	
  //cout << "CPRatio " << score_cp << endl << endl;
	return fam_scores;
}

double word_family::condAsCoVar(string w1, string w2, int m, data *structure, scores *t, scores *s, word_scoring *model)
{
	//printf("word 1: %s, word 2: %s\n", w1.c_str(), w2.c_str());
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
		covar=t->variance;
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
			        	known_words.push_back(sub1);
    
        			string sub2 = w2.substr(i, m + 1);
				if (find(known_words.begin(), known_words.end(), sub2) == known_words.end() && w1.find(sub2)!=string::npos) 
				        known_words.push_back(sub2);
      			}
      			for(int i=0; i < static_cast<int>(known_words.size()); i++)
      			{
				string word = known_words[i];				
				int cw1 = 0;
				int cw2 = 0;
	
				int strict_cw1 = 0;
				int strict_cw2 = 0;
				
				int q1 = 0;
				//printf("%s,%d,%d\n", word.c_str(), 0, m);
				string sub = word.substr(0, m);
				//printf("here\n");
				for (int j = 0; j < 4; j++) 
				{
					string temp = sub;
					temp.append(v[j]);
					q1 += structure->get_count(temp);
					for (int k = 0; k<static_cast<int> (w1.length() - m); k++) 
			      		{ 
						string sub1 = w1.substr(k, m + 1);
						if (sub1.compare(temp) == 0)
							cw1++;
						if (sub1.compare(word) == 0)
							strict_cw1++;
						
						string sub2 = w2.substr(k, m + 1);
						if (sub2.compare(temp) == 0)
							cw2++;
						if (sub2.compare(word) == 0)
							strict_cw2++;
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
						countW2W1++;
					
					string sub2 = w1.substr(i, m + 1);
					if (sub2.compare(temp2) == 0)
						countW1W2++;
				}
      	      	
        			countW1Seq+=structure->get_count(temp1);
        			countW2Seq+=structure->get_count(temp2);
      			}

			covar -= countW2W1 / countW1Seq; //Checked and count W1Seq is right
			covar -= countW1W2 / countW2Seq; //Checked and it is the same as countW2W1 / countW1Seq when the seqs are inverted

      			if (w1.substr(0,m).compare(w2.substr(0,m)) == 0)
			        covar += 1.0 / countW1Seq;
			
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
	return covar;
}

void word_family::computeProduct(vector<vecdouble> *A, vector<double> *v, vector<double> *res)
{
  fill (res->begin(),res->end(),0.0);
  vector<vecdouble>::iterator ALineit=A->begin();
  for (vector<double>::iterator rit=res->begin();rit != res->end(); rit++, ALineit++)
    for (vector<double>::iterator AColit=ALineit->begin(),vit=v->begin();AColit != ALineit->end(); AColit++, vit++)
			*rit += (*AColit)*(*vit);
}
