# Created by prankur.chauhan89@gmail.com
# makefile to build the puf_bch_encoder, puf_bch_decoder and puf_toolkit

# find sources in the current dir and name objects based on their name
SOURCES := $(shell find . -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, ${SOURCES})

# compiler settings
CC = g++

# Compiler Flags
CFLAGS += -fPIC -g -O2 -Wall

TARGET = PUF_BCH
SO_TARGET = libtoolkit.so

#the target build
all: $(TARGET) $(SO_TARGET)

so: $(SO_TARGET)

$(SO_TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(TARGET): ${OBJECTS}
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o:%.cpp %.h 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -r $(OBJECTS) $(TARGET) $(SO_TARGET)


	


