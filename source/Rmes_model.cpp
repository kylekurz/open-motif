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
        	double duration;
        	
        	for(int j=0; j<list->num_words[i+list->minlength-1]; j++){
        		
        		//scores *word = new scores;
                        //get the next word from the data structure
                        
                        next_word = structure->get_next_word(i+list->minlength);
                        //cout<<"next word"<<next_word<<endl;
                        start=clock();
                        expect = condAsExpect(next_word,order,structure);
                        end=clock();
                        duration = (double)(end-start)/CLOCKS_PER_SEC;
                        d1=d1+duration;
                      
                        
                        start=clock();
                        var=condAsVar(next_word,order,structure);
                        end=clock();
                        duration = (double)(end-start)/CLOCKS_PER_SEC;
                        d2=d2+duration;
                        
                        ratio = compute_ratio(expect, var, structure->get_count(next_word));
                        ratio_file<<next_word<<","<<structure->get_count(next_word)<<","<<expect<<","<<var<<","<<ratio<<endl;
                        //cout<<"after ratio call Word:"<<next_word<<" Expect:"<< expect<<" Var:"<<var<<" Word_count:"<< structure->get_count(next_word)<<" Ratio:"<<ratio<<endl;
                        //compute the scores for that word
                        //compute_scores(word, next_word, structure, order);
        		
                }
                ratio_file.close();
        }
        //printf("d1:%f d2:%f\n",d1,d2);
}


//* compute condAsExpect
double rmes_model::condAsExpect(const Word &w, const short m, data *structure)
  {

    long leftSubWord=w.substring(0,m);
    std::stringstream stream_1;
    stream_1 << Word(m+1,leftSubWord);
    std::string word_1;
    stream_1 >> word_1;
    double expect = double(structure->get_count(word_1));

    for (int l=1; l<w.length()-m;l++) {//for1
      long curSubWord=w.substring(l,l+m-1);
      double quantity=0.0;
      std::stringstream stream_2 ;
      std::string word_2;
      for (int letter=0;letter<Alphabet::alphabet->size();letter++){
          stream_2.clear();
          stream_2 << Word(m+1,Alphabet::alphabet->factor()*curSubWord+letter);
          stream_2 >> word_2;
          quantity+=double(structure->get_count(word_2));
      }

      std::stringstream stream_3;
      std::string word_3;
      if (quantity == 0.0) {
	expect=0.0;
	break;
      } else {
          long neighbor=curSubWord*Alphabet::alphabet->factor()+w[l+m];
          stream_3.clear();
          stream_3 << Word(m+1,neighbor);
          stream_3 >> word_3;
	  expect*= double(structure->get_count(word_3))/quantity;
      }
    }

    return expect;
  }


//*compute condAsVar with data*structure instead of Counter
double rmes_model::condAsVar(const Word &w, const short m, data *structure)
{
    double var = 0.0;
    Counter subWordCounter(m + 1, m + 1);
    double expect = condAsExpect(w, m, structure);
    if (expect != 0.0) {
        Counter subWordCounter(m + 1, m + 1);
        subWordCounter.countWords(w);
        
  //      double num_Words=std::pow(double(Alphabet::alphabet->size()) , m+1);
  //      for (long wordIndex = 0; wordIndex < num_Words / Alphabet::alphabet->factor(); wordIndex++) 
{//for 1
    string next_word="";
    string nw="";
    std::stringstream stream;
    stream << w;
    stream >> nw;
    next_word = get_next_extension(nw,m+1); 
    while(next_word.compare("") != 0)
    {
            double q1 = 0.0;
            std::stringstream stream_1;
            string word_1;
            q1 += double(structure->get_count(next_word));
            if (q1 > 0.0) {
                double q2 = 0;
                for (long baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
                    q2 += double(subWordCounter.wordCount(m + 1, Alphabet::alphabet->factor() * wordIndex + baseIndex));
                }
                var += (q2 * q2) / q1;
                std::stringstream stream_2;
                std::string word_2;
                std::stringstream stream_3;
                std::string word_3;
                for (long baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
                    long neighborIndex = Alphabet::alphabet->factor() * wordIndex + baseIndex;
                    stream_2.clear();
                    stream_2 << Word(m + 1, neighborIndex);
                    stream_2 >> word_2;
                    if (structure->get_count(word_2) > 0) {
                        stream_3.clear();
                        stream_3 << Word(m + 1, neighborIndex);
                        stream_3 >> word_3;
                        var -= double(subWordCounter.wordCount(m + 1, neighborIndex) * subWordCounter.wordCount(m + 1, neighborIndex)) / double(structure->get_count(word_3));
                    }
                }
            }
        }
	    
        long firstIndex = w.substring(0, m - 1);
        long seqCounts = 0;
        long wordCounts = 0;
        std::stringstream stream_4;
        std::string word_4;
        
        for (int baseIndex = 0; baseIndex < Alphabet::alphabet->size(); baseIndex++) {
            stream_4.clear();
            stream_4 << Word(m + 1, Alphabet::alphabet->factor() * firstIndex + baseIndex);
            stream_4 >> word_4;
            seqCounts += structure->get_count(word_4);
            wordCounts += subWordCounter.wordCount(m + 1, Alphabet::alphabet->factor() * firstIndex + baseIndex);
        }
        var += (1.0 - 2.0 * double(wordCounts)) / double (seqCounts);
        var *= expect * expect;
        var += expect;

        for (long d = 1; d < w.length() - m; d++) {
            auto_ptr<Word> overlapWord(w.overlaps(d));
            if (overlapWord.get())
                var += 2.0 * condAsExpect(*overlapWord.get(), m, structure);
        }

    }
    
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
    return 0;

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











