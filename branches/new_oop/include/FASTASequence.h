/*
 * FASTASequence.h
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen
 */

#ifndef FASTASEQUENCE_H_
#define FASTASEQUENCE_H_

#include "SequenceString.h"

class FASTASequence
{
	public:
		FASTASequence( SequenceString * sequence, std::string & FASTAHeader);



	private:
		SequenceString * sequence;
		std::string header;


};




#endif /* FASTASEQUENCE_H_ */
