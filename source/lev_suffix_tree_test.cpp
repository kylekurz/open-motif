/*
 * main.cc
 *
 *  Created on: Oct 23, 2009
 *      Author: Lev A Neiman - lev.neiman@gmail.com
 */

//#define PRINT_STEPS
#define PRINT_PROGRESS

#include "SuffixTree.cpp"
#include "SuffixTreeIterator.cpp"
#include "Data_structure.cpp"
#include "Ds_iterator.cpp"
#include <sys/timeb.h>

using namespace std;

int getTimeSpan( timeb & start, timeb & end )
{
	//cout<<start.time<<" : "<<end.time<<endl<<start.millitm<<" : "<<end.millitm<<endl;
	if (start.time == end.time)
		return end.millitm - start.millitm;
	return (end.time - (start.time + 1)) * 1000 + (1000 - start.millitm) + end.millitm;
}


int main( int argc, char ** argv )
{
	srand( time( NULL ));
	timeb start;
	ftime( &start );

	string haystack = "test.txt";
	//string haystack = "E.coli_K-12.fa";
	std::string needle = "GATT";
	cout << "Haystack = " << haystack << endl;
	cout << "Needle = " << needle << endl;

	SuffixTree stree;
	//	stree.readFile("/home/entheogen/workspace/SuffixTree/lev_data/test.txt");
	timeb read_start;
	ftime( &read_start );
	{
		stree.readFile( string( "/home/entheogen/workspace/SuffixTree/lev_data/" + haystack ).c_str() );
	}
	timeb read_end;
	ftime( &read_end );
	cout << "Reading file took " << getTimeSpan( read_start, read_end ) << " milliseconds" << endl;
	//stree.genRandom(10);

	timeb build_tree_start;
	ftime( &build_tree_start );
	{
		stree.buildSTree();
	}
	timeb build_tree_end;
	ftime( &build_tree_end );
	cout << "Building suffix tree took " << getTimeSpan( build_tree_start, build_tree_end ) << " milliseconds" << endl;
	//cout<<"There were "<<SNODE_COUNT<<" nodes created, which take up "<<(sizeof( struct SuffixTreeNode)*SNODE_COUNT)<<" bytes"<<endl;

	timeb naive_count_start;
	ftime( &naive_count_start );
	{
		cout <<"Naive count result = "<< stree.naiveCount( needle ) << endl;
	}
	timeb naive_count_end;
	ftime( &naive_count_end );
	cout << "Naive matching took " << getTimeSpan( naive_count_start, naive_count_end ) << " milliseconds" << endl;
	cout <<"Total time for naive matching = "<< getTimeSpan( naive_count_start, naive_count_end ) + getTimeSpan( read_start, read_end ) <<" milliseconds"<<endl;


	timeb count_start;
	ftime( & count_start );
	{
		cout << "Suffix tree result = "<<stree.count( needle ) << endl;
	}
	timeb count_end;
	ftime( & count_end);
	cout<<"Suffix tree search took "<<getTimeSpan( count_start, count_end)<< " milliseconds"<<endl;
	cout<<"Total time for Suffix Tree search = "<<getTimeSpan( count_start, count_end)+getTimeSpan( read_start, read_end )+getTimeSpan( build_tree_start, build_tree_end)<<" milliseconds"<<endl;


	timeb end;
	ftime( &end );
	cout << "Overall time = " << getTimeSpan( start, end ) << " milliseconds" << endl;

	timeb iterator_start;
	ftime(&iterator_start);
	SuffixTreeIterator iter(&stree,2);
	timeb iterator_end;
	ftime(&iterator_end);
	cout << "Iterator creation time = " << getTimeSpan( iterator_start, iterator_end ) << " milliseconds" << endl;
	cout<<"Total words found = "<<iter.word_count()<<endl;

	while( iter.hasNext())
	{
	//	cout<<"'"<<iter.next()<<"'"<<endl;
	//	cout<<"'"<<stree.get_next_word(2)<<"'"<<endl;
	}
}

