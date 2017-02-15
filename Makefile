# Created by prankur.chauhan89@gmail.com
# makefile to build the puf_bch_encoder, puf_bch_decoder and puf_toolkit

# find sources in the current dir and name objects based on their name
SOURCES := $(shell find . -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, ${SOURCES})

# compiler settings
CC = g++

# Compiler Flags
CFLAGS += -g -O2 -Wall

TARGET = PUF_BCH

#the target build
all: $(TARGET)

$(TARGET): ${OBJECTS}
	$(CC) $(OBJECTS) -o $@

clean: 
	rm -r $(OBJECTS)


	


