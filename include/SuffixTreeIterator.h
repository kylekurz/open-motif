/*
 * SuffixTreeIterator.h
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen
 */

#ifndef SUFFIXTREEITERATOR_H_
#define SUFFIXTREEITERATOR_H_

class SuffixTree;

class SuffixTreeIterator
{
	public:
		SuffixTreeIterator(SuffixTree * stree, index_type word_length);

		bool hasNext();
		std::string next();

		index_type word_count()
		{
			return string_words.size();
		}

	private:
		SuffixTree * stree;
		std::queue<std::pair<SNode*,index_type> > q;
		std::list<SNode * > curr_path;
		index_type word_length;
		//std::vector<SequenceString*> words;
		std::vector<std::string> string_words;
	//	void populate(SNode * node, std::vector<SequenceString*> & words,  index_type rec);
		void populate_string( SNode * node, std::vector<std::string> & string_words, std::string curr_word, index_type rec);
		index_type counter;

};

#endif /* SUFFIXTREEITERATOR_H_ */
