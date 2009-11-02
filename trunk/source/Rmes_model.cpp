
/*
**********************************************
// File:        Rmes_model.cpp
// Purpose:     Define a class for calculating scoring based on RMES model .
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


#include "Rmes_model.h"

#include <iostream>
#include <omp.h>
#include <limits>

using namespace std;
using namespace rmes;

rmes_model::rmes_model()
{
//no op
}

rmes_model::rmes_model(owef_args *input_list, data *structure, int x)
{
	list = input_list;
}

rmes_model::~rmes_model()
{
	//no op
}

rmes_model::rmes_model(owef_args *input_list,data *structure)
{
  cout<<"Scoring with Rmes Model"<<endl;
  list=input_list;
  //******************************************************************
  //score all the words
  //******************************************************************
  int num_files = (list->maxlength - list->minlength)+1;
  for(int i=0; i<num_files; i++)
  {
		string next_word;        
  	int order = list->order;
  	if(order > (i+list->minlength)-2)
    	order = (i+list->minlength)-2;
  	if(order < 0)
    	order = 0;	
    double ratio=0.0;
    double expect_p = 0.0;
    stringstream stream_2;
    ofstream ratio_file;
    string file_name;
    stream_2 <<  list->prefix << "_" << i+list->minlength << "_" << order << "_ratio.csv";
    stream_2 >> file_name;
    ratio_file.open(file_name.c_str());
    ratio_file << "Word,Word_Count,Expect,Variance,Gauss_Ratio,Poisson_Expect,Lambda,Stat, Fact"<<endl;

  	int threadID, totalThreads, j;
		totalThreads = 0;
		threadID = 0;
		
		long maxCount = 0;
		for(j=0; j<list->num_words[i+list->minlength-1]; j++)
  	{
    	string tempword = structure->get_next_word(i+list->minlength);
    	long tempcount = structure->get_count(tempword);
    	if(tempcount > maxCount)
    	{
    		maxCount = tempcount;
    	}
		}
		
		structure->reset();
		
		#ifdef _OPENMP
   		#pragma omp parallel for default(none) shared(maxCount, ratio_file, i, structure, order) private(expect_p, j, threadID, next_word, ratio)
   		#endif
   		for(j=0; j<list->num_words[i+list->minlength-1]; j++)
   		{
			#ifdef _OPENMP
			#pragma omp critical
	 		{
	    			next_word = structure->get_next_word(i+list->minlength);
			}
			#else
			next_word = structure->get_next_word(i+list->minlength);
			#endif
				
			scores *word = NULL;
			word = structure->get_stats(next_word);
			if(word == NULL || word->expect == -1 || word->variance == -1)
			{
				compute_scores(word, next_word, structure, order);
				structure->set_stats(next_word, word);
			}	
				
     	 		ratio = compute_ratio(word->expect, word->variance, structure->get_count(next_word));
      
      			//Computing Compound Poisson Statistics
      			int overlap=0;
      			double fact = 0.0;
      			expect_p = condAsPoissonExpect(next_word,order,word->expect,structure, overlap);
			double fac_am=0.0;
      			if (overlap > static_cast<int>(next_word.length()) - order || overlap == static_cast<int>(next_word.length()))
			{
			  	fac_am=0.0;
			} 
			else 
			{
			  	if (word->expect!=0.0)
			    		fac_am = 1.0-(expect_p/word->expect);
			  	else
			    		fac_am = 1.0 ;
			}

			double lambda = (( 1.0 - fac_am) * (1.0 - fac_am))* word->expect;
			
			double stat = 0.0;
			if (fac_am == 0.0) 
			{
			  	if (structure->get_count(next_word) <= word->expect)
			    		//stat =computeStatPoisson(structure->get_count(next_word),word->expect);
			    		stat = 1;
			  	else
			    		//stat =computeStatPoisson(structure->get_count(next_word)-1,word->expect);
			    		stat = 0;
		
			  	if (stat == numeric_limits<double>::infinity()) 
			  	{
			  	  	//cerr << "Warning in CompoundPoisson estimation. Statistic of word " << next_word << " is infinite." << endl;
			  	}
			}
			else 
			{
				short sign=1;
				double proba=0.0;
			  
//			  	for (short l=_lmin;l<=_lmax;l++) 
//  					maxCount=(_counter->maxCount(l) > maxCount) ? _counter->maxCount(l) : maxCount;
  		
      				vector<double> loi(maxCount+1);
      				vector<double> log_fact(maxCount+1);
      				log_fact[0] = 0.0;
      				for (long x=1; x <= maxCount; x++)
  					log_fact[x] = log_fact[x-1] + log(double(x));
  	
			  	double aux=log((1.0-fac_am)*(1.0-fac_am)*word->expect/double(fac_am));
			  	loi.resize(maxCount+1);
			  	fill(loi.begin(),loi.end(),0.0);
			  	loi[0]=exp(lambda/(fac_am-1.0));
			  	for (long x=1; x<=maxCount; x++)
				{
			  	  	for (long y=1; y <= x; y++) 
			    		{
			      			loi[x] += exp(double(x)*log(fac_am)+ log_fact[x-1] - log_fact[y]- log_fact[y-1] - log_fact[x-y]+ double(y)*aux + (fac_am-1.0)*word->expect);
			    		}
				}
			  	if (structure->get_count(next_word) <= word->expect) 
			  	{
			    		sign=-1;
			    		proba= lower_tail(expect_p,fac_am,maxCount) ;
			  	} 
			  	else 
			  	{
				    	sign=1;
			    		proba= upper_tail(expect_p,fac_am,maxCount) ;
			  	}
			  	if (proba != numeric_limits<double>::infinity()) 
			    		stat=sign*quantile(proba);
			  	else 
			    		stat=numeric_limits<double>::infinity();
			}
			fact=fac_am;
			#ifdef _OPENMP
			#pragma omp critical
			{
				ratio_file << next_word << "," << structure->get_count(next_word) << "," << word->expect << "," << word->variance << "," << ratio << "," << expect_p << "," << lambda << "," << stat << "," << fact << endl;
			}
			#else
			ratio_file << next_word << "," << structure->get_count(next_word) << "," << word->expect << "," << word->variance << "," << ratio << "," << expect_p << "," << lambda << "," << stat << "," << fact << endl;
			#endif
		}
		ratio_file.close();
  	}
	structure->reset();
}

//double rmes_model::computeStatPoisson()
//{
//	
//}

double rmes_model::condAsPoissonExpect(const Word &w,const short m, const double expect, data *structure, int &overlap)
{
  double retval=expect;
  overlap=w.length();
  for (short d=1; d< w.length(); d++) 
  {
    auto_ptr<Word> overlapWord(w.overlaps(d));
    if (overlapWord.get()) 
    {
			overlap=d;
			if (d<=(w.length()-m))
  			retval -= condAsExpect(*overlapWord.get(),m,structure);
			break;
    }
  }

  if (overlap < w.length()) 
  {
    for (short d=w.length() - overlap ; d <= w.length()-m ; d++) 
    {
			if (d % overlap) 
			{
  			auto_ptr<Word> overlapWord(w.overlaps(d));
  			if (overlapWord.get())
    			retval -= condAsExpect(*overlapWord.get(),m,structure);
			}
    }
  }
  return retval;
}

//* compute condAsExpect
double rmes_model::condAsExpect(const Word &w, const short m, data *structure) 
{
    long leftSubWord = w.substring(0, m);
    std::stringstream stream_1;
    stream_1 << Word(m + 1, leftSubWord);
    std::string word_1;
    stream_1 >> word_1;
    double expect = double(structure->get_count(word_1));
    for (int l = 1; l < w.length() - m; l++) {//for1
        long curSubWord = w.substring(l, l + m - 1);
        double quantity = 0.0;
        std::stringstream stream_2;
        std::string word_2;
        for (int letter = 0; letter < Alphabet::alphabet->size(); letter++) {
            stream_2.clear();
            stream_2 << Word(m + 1, Alphabet::alphabet->factor() * curSubWord + letter);
            stream_2 >> word_2;
            quantity += double(structure->get_count(word_2));
        }

        std::stringstream stream_3;
        std::string word_3;
        if (quantity == 0.0) {
            expect = 0.0;
            break;
        } else {
            long neighbor = curSubWord * Alphabet::alphabet->factor() + w[l + m];
            stream_3.clear();
            stream_3 << Word(m + 1, neighbor);
            stream_3 >> word_3;
            expect *= double(structure->get_count(word_3)) / quantity;
        }
    }

    return expect;
}

//*compute condAsVar with data*structure instead of Counter

double rmes_model::condAsVar(const Word &w, const short m, data *structure) {
    double var = 0.0;
    double SeqCount = 0.0;
    double WordCount = 0.0;

    //Counter subWordCounter(m + 1, m + 1);
    std::stringstream stream_1;
    string word_1;
    stream_1.clear();
    stream_1 << w;
    stream_1 >> word_1;

    double expect = condAsExpect(w, m, structure);
    if (expect != 0.0) {
        vector<string> known_words;
        vector<string> known_subwords;
        bool flag = 0;
        for (int i = 0; i<static_cast<int> (word_1.length() - m); i++) {
            string sub = word_1.substr(i, m + 1);

            if (find(known_words.begin(), known_words.end(), sub) == known_words.end()) {
                known_words.push_back(sub);
                string sw = sub.substr(0, m);

                if (find(known_subwords.begin(), known_subwords.end(), sw) == known_subwords.end()) {
                    double q1 = 0;
                    double q2 = 0;
                    double neighbor = 0; //for the neighbor index

                    known_subwords.push_back(sw);

                    vector<string> v;                   
                    string tmp="";
                    for (long baseIndex=0;baseIndex<Alphabet::alphabet->size(); baseIndex++){
			tmp = 'A' + reverse_branch[baseIndex];
                        v.push_back(tmp);
                    }
                    for (int j = 0; j < Alphabet::alphabet->size(); j++) {
                        string temp = sw;
                        int count = structure->get_count(temp.append(v[j]));
                        q1 += count;

                        if (flag == 0) {
                            SeqCount += count;
                        }

                        if (count > 0) {
                            double internal_q2 = 0;

                            for (int p = 0; p<static_cast<int> (word_1.length() - m); p++) {
                                string psub = word_1.substr(p, m + 1);

                                if (psub.compare(temp) == 0) {
                                    q2++;
                                    internal_q2++;
                                }
                            }

                            neighbor += internal_q2 * internal_q2 / count;
                        }
                    }
                    if (flag == 0) {
                        WordCount = q2;
                    }
                    flag = 1;
                    var += (q2 * q2) / q1;
                    var -= neighbor;
                }
            }
        }
    }
        var += (1.0 - 2.0 * double(WordCount)) / double (SeqCount);
        var *= expect * expect;
        var += expect;


        for (long d = 1; d < w.length() - m; d++) {
            auto_ptr<Word> overlapWord(w.overlaps(d));
            if (overlapWord.get())
                var += 2.0 * condAsExpect(*overlapWord.get(), m, structure);
        }
    	return var;

}

//compute the ratio of (N-E)/sqrt(v) where N is the count of word in the sequence
double rmes_model::compute_ratio(double expect,double variance,int word_count)
{
    double ratio=0.0;
    ratio = ((double)word_count - expect) / std::sqrt(variance);
    return ratio;
}

//* Computes condAsVar with rmes_counter derived class NEGLECT for now
double rmes_model::condAsVar(const Word &w, const short m, const Counter &cc)
{
  throw(-1);
}
 
//function to compute all scores of a word
void rmes_model::compute_scores(scores *word, string &motif, data *structure, int &order)
{
	double x = condAsExpect(motif,order,structure);
	//cout << x << endl;
	word->expect = x;
	word->variance = condAsVar(motif,order,structure);
	return;
}

//*rmes
double rmes_model::compute_E(string &motif, data *structure, int &order)
{
 throw(-1);
 
}
double rmes_model::compute_Es(string &motif, data *structure, int &order) 
{
throw(-1);
}
double rmes_model::compute_Oln(string &motif, data *structure, int &order) 
{
throw(-1);
}
double rmes_model::compute_Sln(string &motif, data *structure, int &order) 
{
throw(-1);
}
double rmes_model::compute_pval(string &motif, data *structure) 
{
throw(-1);
}
int  rmes_model::compute_rank(string &motif, data *structure) 
{
throw(-1);
}
int  rmes_model::compute_count(string &motif, data *structure) 
{
throw(-1);
}
int  rmes_model::compute_seqs(string &motif, data *structure) 
{
throw(-1);
}
vector<pair<string, scores> > rmes_model::get_seeds()
{
throw(-1);
}

double rmes_model::lower_tail(const double lambda, const double a, const long n) 
{
  double z ; /* ((1-a) * lambda / a) */
  double L_prec, L_cour, L_suiv, A_cour, A_suiv, S_cour, S_suiv ;
  long i ;

  static const double infinity=1e300;

  z = (1.0-a) * lambda / a ; 
  if (n==0)
    return(exp(-1.0*lambda)) ;
  else if (n==1)
    return(exp(-1.0*lambda)*(1.0+a*z));
  else
  {
		L_prec = -lambda ;
		L_cour = L_prec + log(a*z) ;
		A_cour = L_prec ;
		S_cour = 1.0 + a*z ;
		for (i = 2 ; i<=n ; i++)
	  {
	    L_suiv = L_cour + log( a*(2.0*i-2.0+z)/i + a*a*(2.0-i)*exp(L_prec-L_cour)/i ) ;
	    S_suiv = S_cour + exp(L_suiv-A_cour) ;
	    if ( (S_suiv>0) && (S_suiv<infinity))
	      A_suiv = A_cour ;
	    else
	    {
				A_suiv = A_cour + log(S_cour) ;
				S_suiv = 1.0 + exp(L_suiv-A_suiv) ;
	    }
	    L_prec = L_cour ;
	    L_cour = L_suiv ;
	    S_cour = S_suiv ;
	    A_cour = A_suiv ;
	  } 
		return(S_cour * exp(A_cour)); 
  }
}    

double rmes_model::upper_tail(const double lambda, const double a, const long n) 
{
  double z ; /* (theta * lambda / a) */
  double L_prec, L_cour, L_suiv, A_cour, A_suiv, S_cour, S_suiv ;
  L_suiv = 0;
  long i ;
  bool converged=false ;

  static const double precision=1e-13;
  static const double infinity=1e300;
  
  if (n==0)
    return(1.0) ;
  else if (n==1)
    return(1.0 - exp(-lambda));
  else
  {
		z = (1.0-a) * lambda / a ; 

		L_prec = -lambda ;  /* ln P(PA=0) */
		L_cour = L_prec + log(a*z) ;/* ln P(PA=1) */

		for (i = 2 ; i<=n ; i++)  
	  {
	    L_suiv = L_cour + log( a*(2.0*i-2.0+z)/i + a*a*(2.0-i)*exp(L_prec-L_cour)/i ) ;
	    L_prec = L_cour ;
	    L_cour = L_suiv ;     
	  }
		A_cour = L_suiv ;  /* ln P(PA=n) */
		S_cour = 1.0 ;

		while (!converged)
	  {
	    L_suiv = L_cour + log( a*(2.0*i-2.0+z)/i + a*a*(2.0-i)*exp(L_prec-L_cour)/i ) ;
	    
	    if (L_suiv < log(precision) + A_cour + log(S_cour))
	      converged = true ;
	    
	    S_suiv = S_cour + exp(L_suiv-A_cour) ;
	    if ( (S_suiv>0) && (S_suiv<infinity))
	      A_suiv = A_cour ;
	    else
	    {
				A_suiv = A_cour + log(S_cour) ;
				S_suiv = 1.0 + exp(L_suiv-A_suiv) ;
	    } 
	    L_prec = L_cour ;
	    L_cour = L_suiv ;     
	    S_cour = S_suiv ;
	    A_cour = A_suiv ;  
	    i=i+1; 
	  }

		return(S_cour * exp(A_cour));
  }
}
double rmes_model::quantile(const double p)
{
  static const double c0=2.515515;
  static const double c1=0.802853;
  static const double c2=0.010328;
  static const double d1=1.432788;
  static const double d2=0.189269;
  static const double d3=0.001308;

  static short sign=0;
  static double tmp, tmp1, tmp2;
  double ret=0.0;

  if (p < 0.0 || p > 1.0 || p == numeric_limits<double>::infinity()) 
  {
    ret=numeric_limits<double>::quiet_NaN();
  } 
  else 
  {
    if (p <= 0.5) 
    {
			sign=+1;
			tmp= -2.0 * log(p);
    } 
    else 
    {
			sign=-1;
			tmp= -2.0 * log(1.0 - p);
    }
    if (tmp == numeric_limits<double>::infinity()) 
    {
			ret=tmp;
    } 
    else 
    {
			tmp = sqrt(tmp);
			tmp1 = c0 + (c1 + c2*tmp) * tmp;
			tmp2 = 1.0 + (d1 + (d2 + d3 * tmp) * tmp) * tmp;
			ret = tmp - tmp1 / tmp2;
    }
    ret = sign*ret;
  }

  return ret;
}
