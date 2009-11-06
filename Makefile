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

CPPFLAGS = -O2 -Wall -Werror -DKKURZ_MPI -ggdb
CC = mpicxx
MAIN = source/OWEF_Main.cpp
PLAT = seq
PROJ = bin/OWEFexec
DEBUG := no

ifeq ($(PLAT),multi)
MPI = 1;
CC = mpicxx
endif

ifeq ($(PLAT),seq)
CC = g++
CPPFLAGS = -Wall -Werror -O3 -fopenmp -ggdb
endif

SRCDIR := source
OBJDIR := $(if $(filter yes,$(DEBUG)),debug,release)



INCLUDE_DIRECTORIES = include
CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES)) -ggdb

SRCS := SuffixTree.cpp SuffixTreeIterator.cpp Ds_iterator.cpp FASTAFile.cpp FASTASequence.cpp Alphabet.cpp Cluster.cpp Counter.cpp Data_structure.cpp DNAAlphabet.cpp GFF_generator.cpp Motif.cpp Motif_logo.cpp Motif_scoring.cpp OWEF_args.cpp OWEF.cpp Radix_trie.cpp Radix_trie_leaf.cpp Radix_trie_node.cpp Rmes_model.cpp RMESString.cpp Scores.cpp Word.cpp Word_family.cpp Word_scoring.cpp divsufsort.cpp SuffixArray.cpp
#SRCS := $(addprefix $(SRCDIR)/,$(SRCS))
OBJS := $(addprefix $(OBJDIR)/,$(SRCS:.cpp=.o))

all: $(OBJS)

install: $(OBJS)
	$(CC) -o $(PROJ) $(CPPFLAGS) $(MAIN) $^ -lpthread -ggdb
	
lev_test:
	g++ source/lev_suffix_tree_test.cpp $(CPPFLAGS) -ostree -O3
	
stitest:
	g++ source/SuffixTreeIteratorTest.cpp $(CPPFLAGS) -ostitest -O3

ifeq ($(OBJDIR),debug)
all: CPPFLAGS = -Wall -g -pg
all: CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))
install: CPPFLAGS = -Wall -g -pg -ggdb
install: CPPFLAGS += $(addprefix -I ,$(INCLUDE_DIRECTORIES))
endif

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $(CPPFLAGS) $(OUTPUT_OPTION) $<
	
clean :
	rm -rf *.o *.gch *-db OWEFexec source/*.o source/*.gch source/*-db source/OWEFexec include/*.o include/*.gch include/*-db include/OWEFexec bin/*.o bin/*.gch bin/*-db bin/OWEFexec release/*.o release/*.gch release/*-db release/OWEFexec debug/*.o debug/*.gch debug/*-db debug/OWEFexec
