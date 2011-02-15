CC		=	gcc
CXX		=	g++
MAKE	=	make

CFLAGS	 =	-c -Wall -Iinclude -g
CXXFLAGS = $(CFLAGS)
LDFLAGS	 =	-g

SOURCES	=	src/cnvs_file_writer.c \
			src/cnvs_file_reader.c

OBJECTS = $(SOURCES:.c=.o)

OBJ_DIR = obj
SRC_DIR = src


OBJS_CMPL = $(addprefix $(OBJ_DIR)/, $(notdir $(OBJECTS)))

.SUFFIXES: .c .cpp .o .h

all: $(OBJECTS) 

.c.o:
	$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$(notdir $@)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $(OBJ_DIR)/$(notdir $@)

test: src/test.c
	$(CC) $(LDFLAGS) -I include $(OBJS_CMPL) src/test.c -o test

clean:
	rm -rf $(OBJ_DIR)/*.o cnv_sensor
