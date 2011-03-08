CC		=	gcc
CXX		=	g++
MAKE	=	make

CFLAGS	 =	-Wall -Iinclude -I../bamtools/src -I../ -g -lz
CXXFLAGS = $(CFLAGS)

all: bam_loader binary_reader
	
.PHONY: all

bam_loader: src/bam_loader.cpp ../CNV_FileUtilities.c
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/bam_loader.cpp ../CNV_FileUtilities.c ../bamtools/lib/libbamtools.a -o bam_loader

binary_reader: src/binary_reader.cpp ../CNV_FileUtilities.c
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/binary_reader.cpp ../CNV_FileUtilities.c -o binary_reader

clean:
	rm -rf *.o bam_loader* binary_reader*

.PHONY: clean
