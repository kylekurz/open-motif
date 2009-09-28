
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


using namespace std;
using namespace rmes;

rmes_model::rmes_model()
{
//no op
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
        //cout << "num files: " << num_files << endl;
        ofstream ofiles[num_files];
        double d1=0,d2=0;
        for(int i=0; i<num_files; i++){
        
        	int order = list->order;
        	if(order > (i+list->minlength)-2)
          		order = (i+list->minlength)-2;
        	if(order < 0)
          		order = 0;	
        	stringstream stream;
        	stream <<  list->prefix << "_" << i+list->minlength << "_" << order << ".csv";
        	string filename;
        	stream >> filename;
        	ofstream log(list->log_file.c_str(), ios::app);
        	log << "Scored file: " << filename << endl;
        	log.close();
        	ofiles[i].open(filename.c_str());
        	string next_word = "";
        	ofiles[i] << "#Word,S,E_s,O,E,O/E,O*ln(O/E),S*ln(S/E_s)";	
        	if(list->revcomp){
          	ofiles[i] << ",RevComp,Present,Palindrome";
        	}
        	if(list->pval){
          		ofiles[i] << ",Pval";
        	}
        	ofiles[i] << endl;

        	double expect;
                double var;
                double ratio;
                stringstream stream_2;
                ofstream ratio_file;
                string file_name;
                stream_2 <<  list->prefix << "_" << i+list->minlength << "_" << order << "_ratio.csv";
                stream_2 >> file_name;
                ratio_file.open(file_name.c_str());
                ratio_file << "Word,Word_Count,Expect,Variance,Ratio"<<endl;
        	//cout<<"list num:"<<list->num_words[i+list->minlength-1]<<endl;
        	clock_t start,end;
        	double duration1,duration2;
        	
        	for(int j=0; j<list->num_words[i+list->minlength-1]; j++){
        		
        		//scores *word = new scores;
                        //get the next word from the data structure
                        
                        next_word = structure->get_next_word(i+list->minlength);
                        //cout<<"next word"<<next_word<<endl;
                        start=clock();
                        expect = condAsExpect(next_word,order,structure);
                        end=clock();
                        duration1 = (double)(end-start)/CLOCKS_PER_SEC;
                        d1=d1+duration1;
                      
                        
                        start=clock();
                        var=condAsVar(next_word,order,structure);
                        end=clock();
                        duration2 = (double)(end-start)/CLOCKS_PER_SEC;
                        d2=d2+duration2;
                        
                        ratio = compute_ratio(expect, var, structure->get_count(next_word));
                        ratio_file<<next_word<<","<<structure->get_count(next_word)<<","<<expect<<","<<var<<","<<ratio<<endl;
                        //cout<<"after ratio call Word:"<<next_word<<" Expect:"<< expect<<" Var:"<<var<<" Word_count:"<< structure->get_count(next_word)<<" Ratio:"<<ratio<<endl;
                        //compute the scores for that word
                        //compute_scores(word, next_word, structure, order);
        		
                }
                ratio_file.close();
        }
        printf("condAsExpect(s) took:%f condAsVar(s):%f\n",d1,d2);
}


//* compute condAsExpect
double rmes_model::condAsExpect(const Word &w, const short m, data *structure) {


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
    //	cout << "Word " << w << endl;
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

                    //cout << "Subword " << sw << " Count " << structure->get_count(sw) << endl;
                    //cout << "\t" << sw << endl;
                    /*
                    vector<string> v;
                    v.push_back("a");
                    v.push_back("c");
                    v.push_back("g");                  
                    v.push_back("t");
                    */

                    vector<string> v;                   
                    string tmp="";
                    for (long baseIndex=0;baseIndex<Alphabet::alphabet->size(); baseIndex++){                       
                        //cout<<"char:"<<Alphabet::alphabet->character(baseIndex)<<" baseIndex:"<<baseIndex<<endl;
                        tmp=Alphabet::alphabet->character(baseIndex);
                        v.push_back(tmp);
                    }
                    for (int j = 0; j < 4; j++) {
                        string temp = sw;
                        int count = structure->get_count(temp.append(v[j]));
                        q1 += count;

                        if (flag == 0) {
                            //							cout << temp << count << endl;
                            SeqCount += count;
                        }

                        if (count > 0) {
                            double internal_q2 = 0;

                            for (int p = 0; p<static_cast<int> (word_1.length() - m); p++) {
                                string psub = word_1.substr(p, m + 1);
                                //cout << psub << " " << temp << endl;

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

//	cout << SeqCount << " " << WordCount << endl;

        var += (1.0 - 2.0 * double(WordCount)) / double (SeqCount);
        var *= expect * expect;
        var += expect;


        for (long d = 1; d < w.length() - m; d++) {
            auto_ptr<Word> overlapWord(w.overlaps(d));
            if (overlapWord.get())
                var += 2.0 * condAsExpect(*overlapWord.get(), m, structure);
        }
        
//	cout << w << "\t" << var << "\t" << endl;

    	return var;

}

//compute the ratio of (N-E)/sqrt(v) where N is the count of word in the sequence
double rmes_model::compute_ratio(double expect,double variance,int word_count)
{
    //std::cout<<endl<<"In compute ratio";
    double ratio=0.0;
    ratio = ((double)word_count - expect) / std::sqrt(variance);
    return ratio;
}

//* Computes condAsVar with rmes_counter derived class NEGLECT for now
double rmes_model::condAsVar(const Word &w, const short m, const Counter &cc)
{
  throw(-1);
    /*
    Counter &c = const_cast<Counter &> (cc);
    double var = 0.0;
    Counter subWordCounter(m + 1, m + 1);

    double expect = condAsExpect(w, m, c);
    if (expect != 0.0) {
        Counter subWordCounter(m + 1, m + 1);
        subWordCounter.countWords(w);

        for (long wordIndex = 0; wordIndex < c.numWords(m + 1) / Alphabet::alphabet->factor(); wordIndex++) {
            double q1 = 0.0;
            for (long baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
                q1 += double(c.wordCount(m + 1, Alphabet::alphabet->factor() * wordIndex + baseIndex));
            }
            if (q1 > 0.0) {
                double q2 = 0;
                for (long baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
                    q2 += double(subWordCounter.wordCount(m + 1, Alphabet::alphabet->factor() * wordIndex + baseIndex));
                }
                var += (q2 * q2) / q1;
                for (long baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
                    long neighborIndex = Alphabet::alphabet->factor() * wordIndex + baseIndex;
                    if (c.wordCount(m + 1, neighborIndex) > 0) {
                        var -= double(subWordCounter.wordCount(m + 1, neighborIndex) * subWordCounter.wordCount(m + 1, neighborIndex)) / double(c.wordCount(m + 1, neighborIndex));
                    }
                }
            }
        }

        long firstIndex = w.substring(0, m - 1);
        long seqCounts = 0;
        long wordCounts = 0;

        for (int baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
            seqCounts += c.wordCount(m + 1, Alphabet::alphabet->factor() * firstIndex + baseIndex);
            wordCounts += subWordCounter.wordCount(m + 1, Alphabet::alphabet->factor() * firstIndex + baseIndex);
        }

        var += (1.0 - 2.0 * double(wordCounts)) / double (seqCounts);
        var *= expect * expect;
        var += expect;

        for (long d = 1; d < w.length() - m; d++) {
            auto_ptr<Word> overlapWord(w.overlaps(d));
            if (overlapWord.get())
                var += 2.0 * condAsExpect(*overlapWord.get(), m, c);
        }
    }

    return var;*/
    

}
 
//function to compute all scores of a word
void rmes_model::compute_scores(scores *word, string &motif, data *structure, int &order)
{
   throw(-1);
   
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











