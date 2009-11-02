/*
 * FASTASequence.cpp
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen
 */

#include "FASTASequence.h"
FASTASequence::FASTASequence(SequenceString *sequence, std::string & FASTAHeader)
{
	this->sequence=sequence;
	this->header=FASTAHeader;
}


