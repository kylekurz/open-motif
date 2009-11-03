/*
 * FASTASequence.cpp
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen, lnau
 */

#include "FASTASequence.h"
FASTASequence::FASTASequence(SequenceString *sequence, std::string & FASTAHeader)
{
	this->sequence=sequence;
	this->header=FASTAHeader;
}

SequenceString* FASTASequence::getSeqStr()
{
	return sequence;
}

std::string FASTASequence::getHeader()
{
	return header;
}

