#
# This is an example Makefile for a countwords program.  This
# program uses both the scanner module and a counter module.
# Typing 'make' or 'make count' will create the executable file.
#

# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = gcc
CFLAGS  = -g -Wall

default:  cypher

cypher:  cypher.o
	$(CC) $(CFLAGS) -o cypher cypher.c

cypher.o:  cypher.c cypher.h
	$(CC) $(CFLAGS) -c cypher.c

clean:
	$(RM) count *.o *~