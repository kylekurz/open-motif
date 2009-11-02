/*
 * SuffixTree.cpp
 *
 *  Created on: Oct 23, 2009
 *      Author: Lev A Neiman - lev.neiman@gmail.com
 */

#include "SuffixTree.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

#include <stdio.h>
#include <string.h>

SuffixTree::SuffixTree()
{
	//text.reserve();

}

SuffixTree::~SuffixTree()
{

}

index_type getFileSize( const char * file_name )
{
	FILE * fp = fopen( file_name, "r" );
	fseek( fp, 0L, SEEK_END );
	unsigned long long sz = ftell( fp );
	fclose( fp );
	return sz;

}

void SuffixTree::readFile( const char *file_name )
{
	using namespace std;

	text = (char*) malloc( getFileSize( file_name ) );
	index_type counter = 0;
	ifstream ifs( file_name );
	while (!ifs.eof())
	{
		while (ifs.peek() == '\n' || ifs.peek() == '\r')
			ifs.ignore();
		char t;
		ifs >> t;
		while (ifs.peek() == '\n' || ifs.peek() == '\r')
			ifs.ignore();
		if (t > 84)
			t -= 32;
		if (t != 'A' && t != 'C' && t != 'G' && t != 'T')
			continue;
		//text.push_back( t );
		text[counter++] = t;
	}
	ifs.close();
	cout << "There are " << counter << " symbols in source text" << endl;
	//text.push_back( '$' );
	text[counter++] = '$';
	this->text_size = counter;
#ifdef PRINT_STEPS
	for (int i = 0; i < text.size(); ++i)
	cout << text[i];
	cout << endl;
#endif
}

index_type SuffixTree::naiveCount( std::string needle )
{
	index_type count = 0;
	for (unsigned int i = 0; i < text_size - needle.size(); ++i)
	{
		bool ok = true;
		for (unsigned int j = 0; j < needle.size(); ++j)
		{
			if (text[i + j] != needle[j])
			{
				ok = false;
				break;
			}
		}
		if (ok)
			++count;
	}
	return count;
}

void SuffixTree::buildSTree()
{
	using namespace std;
#ifndef USE_CUSTOM_NEW
	root = new SNode();
#else
	root = NewSNode();
#endif
	root->str_a = text_size - 1;
	root->str_b = text_size - 1;
	root->t = NOT_LEAF;
	for (unsigned long long i = 0; i < text_size; ++i)
	{
		insertSuffix( i );
	}
	cout << "There are " << root->leafNodeCount() << " Leaf nodes..." << endl;
#ifdef PRINT_STEPS
	printNode( root, 0 );
#endif
}

int insert_count = 0;

void SuffixTree::insertSuffix( index_type a )
{
	using namespace std;
#ifdef PRINT_STEPS
	cout << "inserting: ";
	for (unsigned long long i = a; i < text.size(); ++i)
	cout << text[i];
	cout << endl;
	printNode( root, 0 );
	cout << "=============================" << endl;

#endif

#ifdef PRINT_PROGRESS
	insert_count++;
	if( insert_count % (text_size/10) == 0 && text_size/10 != 0)
	{
		cout<<text_size/10<<endl;
		cout<<(insert_count*10 / (text_size/10))<<"% done"<<endl;
	}
#endif

	int offset = SuffixTreeCompOffset( text[a] );
	SNode * root_suffix = root->children[offset];
	if (root_suffix == NULL)
	{
#ifndef USE_CUSTOM_NEW
		root->children[offset] = new SNode();
#else
		root->children[offset] = NewSNode();
#endif
		root->children[offset]->str_a = a;
		root->children[offset]->str_b = text_size - 1;
		root->children[offset]->t = a;
	}
	else
	{
		index_type t = root_suffix->str_a;
		index_type t1 = a;
		// search for node to insert suffix in.
		while (text[t] == text[t1] && t1 < text_size)
		{
			if (t == root_suffix->str_b)
			{
				SNode * new_p = NULL;
				offset = SuffixTreeCompOffset( text[++t1] );
				if (offset == -1)
					return;
#ifdef PRINT_STEPS
				cout << "Node ends with " << text[t1] << endl;
#endif
				new_p = root_suffix->children[offset];
				if (new_p == NULL)
				{
#ifndef USE_CUSTOM_NEW
					SNode * new_node = new SNode();
#else
					SNode * new_node = NewSNode();
#endif
					new_node->str_a = t1;
					new_node->str_b = text_size - 1;
					new_node->t = a;
					root_suffix->children[offset] = new_node;
					return;
				}
				else
				{
					//cout<<text[t1]<<endl;
					t = new_p->str_a;
					root_suffix = new_p;
					continue;
				}
			}

			++t;
			++t1;
		}
#ifndef USE_CUSTOM_NEW
		SNode * new_child = new SNode();
#else
		SNode * new_child = NewSNode();
#endif
		new_child->str_a = t1;
		new_child->str_b = text_size - 1;
		new_child->t = a;

#ifndef USE_CUSTOM_NEW
		SNode * second_half = new SNode();
#else
		SNode * second_half = new SNode();
#endif
		second_half->str_a = t;
		second_half->str_b = root_suffix->str_b;
		second_half->t = root_suffix->t;

		for (int i = 0; i < 5; ++i)
		{
			second_half->children[i] = root_suffix->children[i];
			root_suffix->children[i] = NULL;
		}

	//	int off_t = SuffixTreeCompOffset( text[t] );
	//	int off_t1 = SuffixTreeCompOffset( text[t1] );

		root_suffix->t = 0;
		root_suffix->str_b = t - 1;
		/*
		 SNode * old_second_half =root_suffix->children[off_t];
		 if( old_second_half != NULL )
		 {
		 second_half->children[compOffset(text[old_second_half->str_a])]=old_second_half;
		 }
		 SNode * old_new_child = root_suffix->children[off_t1];
		 if( old_new_child != NULL )
		 {
		 new_child->children[compOffset(text[old_new_child->str_a])] = old_new_child;
		 }
		 */
		root_suffix->children[SuffixTreeCompOffset( text[t] )] = second_half;
		root_suffix->children[SuffixTreeCompOffset( text[t1] )] = new_child;
		root_suffix->t = NOT_LEAF;
#ifdef PRINT_STEPS
		d_print( off_t);
		d_print(off_t1);
#endif

	}
}

index_type SuffixTree::count( std::string needle )
{
	using namespace std;
	SNode * walka = root->children[SuffixTreeCompOffset( needle[0] )];
	if (walka == NULL)
		return 0;
	index_type t = walka->str_a;
	for (index_type i = 0; i < needle.size(); ++i)
	{
		//cout<<needle[i]<<endl;
		if (t == walka->str_b && i < needle.size() - 1)
		{
			walka = walka->children[SuffixTreeCompOffset( needle[i + 1] )];
			//	cout<<walka<<endl;
			if (walka == NULL)
				return 0;
			t = walka->str_a;
			continue;
		}
		if (text[t] != needle[i])
			return 0;
		++t;
	}

	return walka->leafNodeCount();
	//return walka->leafNodeCount_iterative();
	//return count_tail_recursive( walka );
}
void SuffixTree::writeTextToFile( const char *file_name )
{
	using namespace std;
	ofstream ofs( file_name );
	for (unsigned int i = 0; i < text_size; ++i)
	{
		ofs << text[i];
	}
	ofs.close();
}

void SuffixTree::genRandom( int length )
{
	using namespace std;
	text = (char*) malloc( length + 1 );
	int i;
	for (i = 0; i < length; ++i)
	{
		int t = rand() % 4;
		char b='A';
		switch (t)
		{
			case 0:
				b = 'A';
				break;
			case 1:
				b = 'C';
				break;
			case 2:
				b = 'G';
				break;
			case 3:
				b = 'T';
				break;
		}
		text[i] = b;
	}
	text[i] = '$';
	text_size = i + 1;
#ifdef PRINT_STEPS
	cout << "generated string = ";
	for (int i = 0; i < text.size(); ++i)
	cout << text[i];
	cout << endl;
#endif
}



std::string genTabs( int t )
{
	std::string s;
	for (int i = 0; i < t; ++i)
		s += '\t';
	return s;
}

void SuffixTree::printNode( SNode *node, int rec )
{
	using namespace std;
	string t = genTabs( rec );
	cout << t;
	for (index_type i = node->str_a; i <= node->str_b; ++i)
	{
		cout << text[i];
	}
	cout << ":" << node->t << endl;
	for (int i = 0; i < NUM_CHAR; ++i)
	{
		if (node->children[i] != NULL)
		{
			printNode( node->children[i], rec + 1 );
		}
	}

}

#ifdef USE_CUSTOM_NEW

unsigned long long __new_alloc_chunk = 1<<30;
unsigned long long __new_op_end;
char * __new_op_curr_free = NULL;
char * __new_op_buff = NULL;

SNode *SuffixTree::NewSNode()
{
	if ((__new_op_buff + __new_alloc_chunk - __new_op_curr_free < sizeof(SNode)) || __new_op_buff == NULL)
	{
		__new_op_buff = (char*)sbrk(__new_alloc_chunk);//(char*) malloc( __new_alloc_chunk );
		//	memset( __new_op_buff, 0, __new_alloc_chunk );
		__new_op_curr_free = __new_op_buff;
	}
	SNode*ret = (SNode*) __new_op_curr_free;
	ret->init();
	__new_op_curr_free += sizeof(SNode);
	return ret;
}
#endif
