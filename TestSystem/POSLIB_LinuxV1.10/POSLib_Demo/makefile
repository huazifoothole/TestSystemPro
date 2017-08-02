


#
# makefile - simple makefile for the BeiYang driver
#
# Copyright 2002, BeiYang Company
#
#

# Important compiler and linker options
CC=g++
LD=g++
CFLAGS=-g -O2 
LDFLAGS=-g -ldl 

# Required libraries
LDLIBS= 

# Source files and modules
SRC=TestPOSLIB.c 
SHAREDHEADER=
MOD=$(SRC:.c=.o)

# Standard production rule
.c.o: 
	$(CC) $(CFLAGS) -c $<

# make all
all: TestPOSLIBDemo 

# linking the modules
TestPOSLIBDemo: $(MOD)
	$(LD) $(LDFLAGS) $(LDLIBS) -o $@ $(MOD) -lpthread POSLIB.a libusb.a -lm -Wall

# dependencies, here a shared header
$(SRC): $(SHAREDHEADER) 

# clear up the mess to start over
clean: 
	rm -f *.o *~ core  TestPOSLIBDemo 

