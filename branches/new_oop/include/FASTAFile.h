/*
 * FASTAFile.h
 *
 *  Created on: Nov 2, 2009
 *      Author: entheogen
 */

#ifndef FASTAFILE_H_
#define FASTAFILE_H_

#include <vector>
#include "FASTASequence.h"

class FASTAFile
{
	public:
		FASTAFile(const char * file_name);
		virtual ~FASTAFile();

		FASTASequence * getSeq(int i);
		unsigned int getNumSeqs();

	private:
		std::vector<FASTASequence*> sequences;
};

#endif /* FASTAFILE_H_ */
