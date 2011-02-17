CC		=	gcc
CXX		=	g++
MAKE	=	make

CFLAGS	 =	-c -Wall -Iinclude -I../bamtools/src -g
CXXFLAGS = $(CFLAGS)
LDFLAGS	 =	-g -lz

SOURCES	=	src/cnvs_file_writer.c \
			src/cnvs_file_reader.c

OBJECTS = $(SOURCES:.c=.o)

OBJ_DIR = obj
SRC_DIR = src


OBJS_CMPL = $(addprefix $(OBJ_DIR)/, $(notdir $(OBJECTS)))

.SUFFIXES: .c .cpp .o .h

all: $(OBJECTS) bam_loader

bam_loader: $(SOURCES) src/bam_loader.o
	$(CXX) $(LDFLAGS) $(OBJS_CMPL) obj/bam_loader.o ../bamtools/lib/libbamtools.a -o bam_loader

.c.o:
	$(CXX) $(CFLAGS) $< -o $(OBJ_DIR)/$(notdir $@)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $(OBJ_DIR)/$(notdir $@)

test: src/binary_reader.o
	$(CXX) $(LDFLAGS) $(OBJS_CMPL) obj/binary_reader.o -o binary_reader

clean:
	rm -rf $(OBJ_DIR)/*.o bam_loader test binary_reader
