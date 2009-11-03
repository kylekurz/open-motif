/*
 * FASTASequence.h
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen, lnau
 */

#ifndef FASTASEQUENCE_H_
#define FASTASEQUENCE_H_

#include "SequenceString.h"

class FASTASequence
{
	public:
		FASTASequence( SequenceString * sequence, std::string & FASTAHeader);
		SequenceString * getSeqStr();
		std::string getHeader();

	private:
		SequenceString * sequence;
		std::string header;
};

#endif /* FASTASEQUENCE_H_ */
