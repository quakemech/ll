# ******************************************************************************
#
# @file			Makefile
#
# @brief        Makefile for Multi Thread Safe Linked List 
#
# @copyright    Copyright (C) 2024 Barrett Edwards. All rights reserved.
#
# @date         Aug 2024
# @author       Barrett Edwards
#
# ******************************************************************************

CC=gcc
#CFLAGS?= -g3 -O0 -Wall -Wextra
CFLAGS?= -g3 -O0
VISIBILITY=-fvisibility=hidden
MACROS?=
INCLUDE_DIR?=/usr/local/include
LIB_DIR?=/usr/local/lib
INCLUDE_PATH=-I $(INCLUDE_DIR) 
LIB_PATH=-L $(LIB_DIR)
LIBS=
TARGET=ll

all: lib$(TARGET).a testbench

testbench: testbench.c lib$(TARGET).o
	$(CC) $^ $(CFLAGS) $(MACROS) $(INCLUDE_PATH) $(LIB_PATH) $(LIBS) -o $@ 

lib$(TARGET).a: lib$(TARGET).o 
	ar rcs $@ $^

lib$(TARGET).o: ll.c ll.h
	$(CC) -c $< $(CFLAGS) $(VISIBILITY) $(MACROS) $(INCLUDE_PATH) -o $@
	objcopy --localize-hidden --strip-unneeded $@

clean:
	rm -rf ./*.o ./*.a server client

doc: 
	doxygen

install: lib$(TARGET).a gsc.h
	sudo cp lib$(TARGET).a $(LIB_DIR)/
	sudo cp $(TARGET).h $(INCLUDE_DIR)/$(TARGET).h

uninstall:
	sudo rm $(LIB_DIR)/lib$(TARGET).a
	sudo rm $(INCLUDE_DIR)/$(TARGET).h

# List all non file name targets as PHONY
.PHONY: all clean doc install uninstall

# Variables 
# $^ 	Will expand to be all the sensitivity list
# $< 	Will expand to be the frist file in sensitivity list
# $@	Will expand to be the target name (the left side of the ":" )
# -c 	gcc will compile but not try and link 
