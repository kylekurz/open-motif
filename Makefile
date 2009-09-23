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
MAIN = OWEF_Main.cpp
PLAT = multi
PROJ = OWEFexec

OBJ = Alphabet.o Cluster.o Counter.o Data_structure.o DNAAlphabet.o GFF_generator.o Module_discovery.o Motif.o Motif_logo.o Motif_scoring.o OWEF_args.o OWEF.o Radix_trie.o Radix_trie_node.o Rmes_model.o RMESString.o Scores.o Word.o Word_scoring.o

ifeq ($(PLAT),multi)
	MPI = 1;
endif

ifeq ($(PLAT),seq)
	CC = g++
	CFLAGS = -Werror -Wall -O2
endif

$(PROJ) : $(OBJ)
	$(CC) -o $(PROJ) $(CFLAGS) $(MAIN) $(OBJ) -lpthread

Alphabet.o: Alphabet.cc Alphabet.h
	$(CC) -c Alphabet.cc Alphabet.h

Cluster.o: Cluster.cpp Cluster.h
	$(CC) -c $(CFLAGS) Cluster.cpp Cluster.h

Counter.o: Counter.cc Counter.h
	$(CC) -c $(CFLAGS) Counter.cc Counter.h
	
#Cluster_method.o: Cluster_method.cpp Cluster_method.h
#	$(CC) -c $(CFLAGS) Cluster_method.cpp Cluster_method.h

Data_structure.o: Data_structure.cpp Data_structure.h
	$(CC) -c $(CFLAGS) Data_structure.cpp Data_structure.h

DNAAlphabet.o: DNAAlphabet.cc DNAAlphabet.h
	 $(CC) -c $(CFLAGS) DNAAlphabet.cc DNAAlphabet.h

#Edit_cluster.o: Edit_cluster.cpp Edit_cluster.h
#	$(CC) -c $(CFLAGS) Edit_cluster.cpp Edit_cluster.h

GFF_generator.o: GFF_generator.cpp GFF_generator.h
	$(CC) -c $(CFLAGS) GFF_generator.cpp GFF_generator.h
	
#Hamming_cluster.o: Hamming_cluster.cpp Hamming_cluster.h
#	$(CC) -c $(CFLAGS) Hamming_cluster.cpp Hamming_cluster.h
	
Markov_model.o: Markov_model.cpp Markov_model.h
	$(CC) -c $(CFLAGS) Markov_model.cpp Markov_model.h
	
Module_discovery.o: Module_discovery.cpp Module_discovery.h
	$(CC) -c $(CFLAGS) Module_discovery.cpp Module_discovery.h
	
Module_enumeration.o: Module_enumeration.cpp Module_enumeration.h
	$(CC) -c $(CFLAGS) Module_enumeration.cpp Module_enumeration.h

Motif.o: Motif.cpp Motif.h
	$(CC) -c $(CFLAGS) Motif.cpp Motif.h
	
Motif_logo.o: Motif_logo.cpp Motif_logo.h
	$(CC) -c $(CFLAGS) Motif_logo.cpp Motif_logo.h
	
Motif_scoring.o: Motif_scoring.cpp Motif_scoring.h
	$(CC) -c $(CFLAGS) Motif_scoring.cpp Motif_scoring.h

OWEF_args.o: OWEF_args.cpp OWEF_args.h
	$(CC) -c $(CFLAGS) OWEF_args.cpp OWEF_args.h
	
OWEF_MPI.o: OWEF_MPI.cpp OWEF_MPI.h
	$(CC) -c $(CFLAGS) OWEF_MPI.cpp OWEF_MPI.h
	
Parallel_radix_trie_MPI.o: Parallel_radix_trie_MPI.cpp Parallel_radix_trie_MPI.h
	$(CC) -c $(CFLAGS) Parallel_radix_trie_MPI.cpp Parallel_radix_trie_MPI.h
	
Radix_trie_node.o: Radix_trie_node.cpp Radix_trie_node.h
	$(CC) -c $(CFLAGS) Radix_trie_node.cpp Radix_trie_node.h

Rmes_model.o: Rmes_model.cpp Rmes_model.h
	$(CC) -c $(CFLAGS) Rmes_model.cpp Rmes_model.h

RMESString.o: RMESString.cc RMESString.h
	$(CC) -c $(CFLAGS) RMESString.cc RMESString.h

Scores.o: Scores.cpp Scores.h
	$(CC) -c $(CFLAGS) Scores.cpp Scores.h

Scores_ws.o: Scores_ws.cpp Scores_ws.h
	$(CC) -c $(CFLAGS) Scores_ws.cpp Scores_ws.h

Scores_om.o: Scores_om.cpp Scores_om.h
	$(CC) -c $(CFLAGS) Scores_om.cpp Scores_om.h
	
Word.o: Word.cc Word.h
	$(CC) -c $(CFLAGS) Word.cc Word.h
		
Word_scoring.o: Word_scoring.cpp Word_scoring.h
	$(CC) -c $(CFLAGS) Word_scoring.cpp Word_scoring.h

OWEF.o: OWEF.cpp OWEF.h
	$(CC) -c $(CFLAGS) OWEF.cpp OWEF.h
	
Parallel_radix_trie.o: Parallel_radix_trie.cpp Parallel_radix_trie.h
	$(CC) -c $(CFLAGS) Parallel_radix_trie.cpp Parallel_radix_trie.h

Radix_trie.o: Radix_trie.cpp Radix_trie.h
	$(CC) -c $(CFLAGS) Radix_trie.cpp Radix_trie.h
	
debug:Alphabet.o-db Cluster.o-db Cluster_method.o-db Counter.o-db Data_structure.o-db DNAAlphabet.o-db Edit_cluster.o-db Execute-db Markov_model.o-db Hamming_cluster.o-db OWEF_args.o-db OWEF.o-db Parallel_radix_trie.o-db Radix_trie_node.o-db Radix_trie.o-db Rmes_model.o-db RMESString.o-db Scores.o-db Word.o-db Word_scoring.o-db

Alphabet.o-db: Alphabet.cc Alphabet.h
	$(CC) -c $(CFLAGS) Alphabet.cc Alphabet.h $(DEBUG)

Cluster.o-db: Cluster.cpp Cluster.h
	$(CC) -c $(CFLAGS) Cluster.cpp Cluster.h $(DEBUG)

Cluster_method.o-db: Cluster_method.cpp Cluster_method.h
	$(CC) -c $(CFLAGS) Cluster_method.cpp Cluster_method.h $(DEBUG)
	
Data_structure.o-db: Data_structure.cpp Data_structure.h
	$(CC) -c $(CFLAGS) Data_structure.cpp Data_structure.h $(DEBUG)

DNAAlphabet.o-db: DNAAlphabet.cc DNAAlphabet.h
	 $(CC) -c $(CFLAGS) DNAAlphabet.cc DNAAlphabet.h $(DEBUG)

Edit_cluster.o-db: Edit_cluster.cpp Edit_cluster.h
	$(CC) -c $(CFLAGS) Edit_cluster.cpp Edit_cluster.h $(DEBUG)
	
Hamming_cluster.o-db: Hamming_cluster.cpp Hamming_cluster.h
	$(CC) -c $(CFLAGS) Hamming_cluster.cpp Hamming_cluster.h $(DEBUG)
	
Markov_model.o-db: Markov_model.cpp Markov_model.h
	$(CC) -c $(CFLAGS) Markov_model.cpp Markov_model.h $(DEBUG)
	
Module_discovery.o-db: Module_discovery.cpp Module_discovery.h
	$(CC) -c $(CFLAGS) Module_discovery.cpp Module_discovery.h $(DEBUG)
	
Module_enumeration.o-db: Module_enumeration.cpp Module_enumeration.h
	$(CC) -c $(CFLAGS) Module_enumeration.cpp Module_enumeration.h $(DEBUG)

OWEF_args.o-db: OWEF_args.cpp OWEF_args.h
	$(CC) -c $(CFLAGS) OWEF_args.cpp OWEF_args.h $(DEBUG)
	
OWEF.o-db: OWEF.cpp OWEF.h
	$(CC) -c $(CFLAGS) OWEF.cpp OWEF.h $(DEBUG)
	
Parallel_radix_trie.o-db: Parallel_radix_trie.cpp Parallel_radix_trie.h
	$(CC) -c $(CFLAGS) Parallel_radix_trie.cpp Parallel_radix_trie.h $(DEBUG)
	
Radix_trie_node.o-db: Radix_trie_node.cpp Radix_trie_node.h
	$(CC) -c $(CFLAGS) Radix_trie_node.cpp Radix_trie_node.h $(DEBUG)

Radix_trie.o-db: Radix_trie.cpp Radix_trie.h
	$(CC) -c $(CFLAGS) Radix_trie.cpp Radix_trie.h $(DEBUG)

Rmes_model.o-db: Rmes_model.cpp Rmes_model.h
	$(CC) -c $(CFLAGS) Rmes_model.cpp Rmes_model.h $(DEBUG)

RMESString.o-db: RMESString.cc RMESString.h
	$(CC) -c $(CFLAGS) RMESString.cc RMESString.h $(DEBUG)

Scores.o-db: Scores.cpp Scores.h
	$(CC) -c $(CFLAGS) Scores.cpp Scores.h $(DEBUG)

Word.o-db: Word.cc Word.h
	$(CC) -c $(CFLAGS) Word.cc Word.h $(DEBUG)
		
Word_scoring.o-db: Word_scoring.cpp Word_scoring.h
	$(CC) -c $(CFLAGS) Word_scoring.cpp Word_scoring.h $(DEBUG) 



Execute-db: OWEF_Main.cpp Alphabet.o-db Cluster.o-db Counter.o-db Data_structure.o-db DNAAlphabet.o-db OWEF_args.o-db OWEF.o-db Radix_trie_node.o-db Radix_trie.o-db Rmes_model.o-db RMESString.o-db Scores.o-db Word.o-db Word_scoring.o-db
	$(CC) -o OWEFexec $(CFLAGS) OWEF_Main.cpp Alphabet.o Cluster.o Data_structure.o DNAAlphabet.o OWEF_args.o OWEF.o Radix_trie_node.o Radix_trie.o Rmes_model.o RMESString.o Scores.o Word.o Word_scoring.o $(DEBUG) -lpthread

clean :
	rm -rf *.o *.gch *-db OWEFexec

