/*
 * SuffixTreeIterator.cpp
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen
 */

#include "SuffixTreeIterator.h"

SuffixTreeIterator::~SuffixTreeIterator(){}

std::string SuffixTreeIterator::next()
{
	using namespace std;
	if( counter >= string_words.size() )counter=0;
	return this->string_words[counter++];
	/*
	 if (q.empty())
	 return string( "" );

	 pair<SNode *, index_type> cur = q.front();
	 q.pop();
	 SNode * cur_node = cur.first;

	 index_type walka = cur.second;
	 while (walka != this->word_length - 1)
	 {
	 if ((cur_node->str_b - cur_node->str_a) + walka < word_length)
	 {
	 for (int i = 0; i < 5; ++i)
	 {
	 if (cur_node->children[i] != NULL)
	 q.push( make_pair( cur.first->children[i], walka + cur.first->str_b - cur.first->str_a ) );
	 }
	 }
	 }
	 */
}

bool SuffixTreeIterator::hasNext()
{
	return counter < string_words.size();
	//return !q.empty();
}

SuffixTreeIterator::SuffixTreeIterator( SuffixTree *stree, index_type word_length )
{
	using namespace std;
	this->stree = stree;
	this->word_length = word_length;
	this->counter = 0;
	//stree->printNode(stree->root,0);
	for (int i = 0; i < 5; ++i)
	{

		if (stree->root->children[i] != NULL)
			q.push( make_pair( stree->root->children[i], 0 ) );

		if (stree->root->children[i] != NULL)
		{
			populate_string( stree->root->children[i], this->string_words, string( "" ), 0 );
		}
	}
}

/*

void SuffixTreeIterator::populate( SNode *node, std::vector<SequenceString*> & words, index_type rec )
{
	using namespace std;
	index_type node_size = (node->str_b - node->str_a) + 1;
	if (rec > word_length)
		return;

	cout << "======================" << endl;
	d_print(rec);
	stree->printNode( node, 0 );
	cout << "======================" << endl;

	if (node_size + rec >= this->word_length)
	{
		index_type b = node->str_b - (rec - this->word_length);
		SequenceString *new_seq = new SequenceString( this->stree->text, node->str_a, b );
		new_seq->print();
		words.push_back( new_seq );
	}
	//	else
	{
		for (int i = 0; i < 4; ++i)
		{
			if (node->children[i] != NULL)
			{
				populate( node->children[i], words, rec + node_size );
			}
		}
	}
}

*/

void SuffixTreeIterator::populate_string( SuffixTreeNode *node, std::vector<std::string> & string_words, std::string curr_word, index_type rec )
{
	using namespace std;
	index_type node_size = (node->str_b - node->str_a) + 1;
	if (rec > word_length)
		return;
/*
	cout << "======================" << endl;
	d_print(rec);
	stree->printNode( node, 0 );
	cout << "======================" << endl;
	*/

	if (node_size + rec >= this->word_length)
	{
		index_type b = node->str_b - (rec - this->word_length);
		for (index_type i = node->str_a; i <= b && curr_word.size()<word_length; ++i)
		{
			if( stree->text[i]=='$')return;
			curr_word += stree->text[i];
		}
		string_words.push_back( curr_word );
	}
	else
	{
		for (index_type i = node->str_a; i <= node->str_b; ++i)
		{
			curr_word += stree->text[i];
		}

		{
			for (int i = 0; i < 4; ++i)
			{
				if (node->children[i] != NULL)
				{
					populate_string( node->children[i], string_words, curr_word, rec + node_size );
				}
			}
		}
	}
}

