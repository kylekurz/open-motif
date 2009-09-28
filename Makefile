#****************************************************************
#
#Author: Kyle Kurz
#
#Date: 7.14.09
#
#File: Makefile for OWEF framework
#
#Default build is MPI-based and requires the mpiCC compiler to
#be present on the system.  Alternatively, you can run this 
#with the command: make MAIN=OWEF_Main.cpp
#This command allows the system to use single-node, multi-core
#compilation with only g++, and eliminates the dependencies
#on mpiCC
#
#****************************************************************

CFLAGS = -Wall -O2 -DKKURZ_MPI
CC = mpiCC
DEBUG = -pg -g
MAIN = source/OWEF_Main.cpp
PLAT = multi
PROJ = bin/OWEFexec
VPATH = source:include

OBJ = Alphabet.o Cluster.o Counter.o Data_structure.o DNAAlphabet.o GFF_generator.o Motif.o Motif_logo.o Motif_scoring.o OWEF_args.o OWEF.o Radix_trie.o Radix_trie_node.o Rmes_model.o RMESString.o Scores.o Word.o Word_scoring.o

ifeq ($(PLAT),multi)
	MPI = 1;
endif

ifeq ($(PLAT),seq)
	CC = g++
	CFLAGS = -Werror -Wall -O2
endif

$(PROJ) : $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(MAIN) $^ -lpthread

Alphabet.o: Alphabet.cpp Alphabet.h
	$(CC) -c $(CFLAGS) $^

Cluster.o: Cluster.cpp Cluster.h
	$(CC) -c $(CFLAGS) $^

Counter.o: Counter.cpp Counter.h
	$(CC) -c $(CFLAGS) $^

Data_structure.o: Data_structure.cpp Data_structure.h
	$(CC) -c $(CFLAGS) $^

DNAAlphabet.o: DNAAlphabet.cpp DNAAlphabet.h
	 $(CC) -c $(CFLAGS) $^

GFF_generator.o: GFF_generator.cpp GFF_generator.h
	$(CC) -c $(CFLAGS) $^

Motif.o: Motif.cpp Motif.h
	$(CC) -c $(CFLAGS) $^
	
Motif_logo.o: Motif_logo.cpp Motif_logo.h
	$(CC) -c $(CFLAGS) $^
	
Motif_scoring.o: Motif_scoring.cpp Motif_scoring.h
	$(CC) -c $(CFLAGS) $^

OWEF_args.o: OWEF_args.cpp OWEF_args.h
	$(CC) -c $(CFLAGS) $^

OWEF.o: OWEF.cpp OWEF.h
	$(CC) -c $(CFLAGS) $^
	
Radix_trie_node.o: Radix_trie_node.cpp Radix_trie_node.h
	$(CC) -c $(CFLAGS) $^

Radix_trie.o: Radix_trie.cpp Radix_trie.h
	$(CC) -c $(CFLAGS) $^

Rmes_model.o: Rmes_model.cpp Rmes_model.h
	$(CC) -c $(CFLAGS) $^

RMESString.o: RMESString.cpp RMESString.h
	$(CC) -c $(CFLAGS) $^

Scores.o: Scores.cpp Scores.h
	$(CC) -c $(CFLAGS) $^
	
Word.o: Word.cpp Word.h
	$(CC) -c $(CFLAGS) $^
		
Word_scoring.o: Word_scoring.cpp Word_scoring.h
	$(CC) -c $(CFLAGS) $^
	
debug:Alphabet.o-db Cluster.o-db Counter.o-db Data_structure.o-db DNAAlphabet.o-db Execute-db OWEF_args.o-db OWEF.o-db Radix_trie_node.o-db Radix_trie.o-db Rmes_model.o-db RMESString.o-db Scores.o-db Word.o-db Word_scoring.o-db

Alphabet.o-db: Alphabet.cpp Alphabet.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)

Cluster.o-db: Cluster.cpp Cluster.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)
	
Data_structure.o-db: Data_structure.cpp Data_structure.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)

DNAAlphabet.o-db: DNAAlphabet.cpp DNAAlphabet.h
	 $(CC) -c $(CFLAGS) $^ $(DEBUG)

OWEF_args.o-db: OWEF_args.cpp OWEF_args.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)
	
OWEF.o-db: OWEF.cpp OWEF.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)
	
Radix_trie_node.o-db: Radix_trie_node.cpp Radix_trie_node.h
	$(CC) -c $(CFLAGS)$^ $(DEBUG)

Radix_trie.o-db: Radix_trie.cpp Radix_trie.h
	$(CC) -c $(CFLAGS) Radix_trie.cpp Radix_trie.h $(DEBUG)

Rmes_model.o-db: Rmes_model.cpp Rmes_model.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)

RMESString.o-db: RMESString.cpp RMESString.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)

Scores.o-db: Scores.cpp Scores.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)

Word.o-db: Word.cpp Word.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG)
		
Word_scoring.o-db: Word_scoring.cpp Word_scoring.h
	$(CC) -c $(CFLAGS) $^ $(DEBUG) 

Execute-db: $(MAIN) Alphabet.o-db Cluster.o-db Counter.o-db Data_structure.o-db DNAAlphabet.o-db OWEF_args.o-db OWEF.o-db Radix_trie_node.o-db Radix_trie.o-db Rmes_model.o-db RMESString.o-db Scores.o-db Word.o-db Word_scoring.o-db
	$(CC) -o OWEFexec $(CFLAGS) $(MAIN) Alphabet.o Cluster.o Data_structure.o DNAAlphabet.o OWEF_args.o OWEF.o Radix_trie_node.o Radix_trie.o Rmes_model.o RMESString.o Scores.o Word.o Word_scoring.o $(DEBUG) -lpthread

clean :
	rm -rf *.o *.gch *-db OWEFexec source/*.o source/*.gch source/*-db source/OWEFexec include/*.o include/*.gch include/*-db include/OWEFexec bin/*.o bin/*.gch bin/*-db bin/OWEFexec

