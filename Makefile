# Created by prankur.chauhan89@gmail.com
# makefile to build the puf_bch_encoder, puf_bch_decoder and puf_toolkit

# find sources in the current dir and name objects based on their name
SOURCES := $(shell find . -type f -name '*.cpp')
OBJECTS := $(patsubst %.cpp, %.o, ${SOURCES})

# compiler settings
CC = g++

# Compiler Flags
override CFLAGS += -g -O2 -Wall


TARGET = PUF_BCH
SO_TARGET = libtoolkit.so

#the target build
all: $(SO_TARGET)

target: clean $(TARGET)

$(SO_TARGET): CFLAGS+= -fPIC -D JNI
$(SO_TARGET): INCLUDE+=-I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux
$(SO_TARGET): $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

$(TARGET): ${OBJECTS}
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

%.o:%.cpp %.h 
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -r $(OBJECTS)
cleanall:
	rm -r $(OBJECTS) $(TARGET) $(SO_TARGET)
