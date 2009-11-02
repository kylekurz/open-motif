/*
 * FASTAFile.cpp
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen
 */

#include "../include/FASTAFile.h"

FASTAFile::FASTAFile(const char * file_name)
{


}

FASTAFile::~FASTAFile()
{
	// TODO Auto-generated destructor stub
}

FASTASequence *FASTAFile::getSeq(int i)
{
	return this->sequences[i];
}

unsigned int FASTAFile::getNumSeqs()
{
	return this->sequences.size();
}




