# Makefile for mem memory system, CSE374 22WI

# Compiler:
CC = gcc

# Compiler Flags:  
CFLAGS = -Wall -std=c11

TARGET = bench

all: $(TARGET)

OBJS = bench.o getmem.o freemem.o mem_utils.o 

# Recipes for debug and testing
noassert: CFLAGS += -D NDEBUG
noassert: bench

debug: CFLAGS += -g
debug: bench

test: debug
	./bench 10 50

clean:
	rm -f *.o ./bench

#executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o bench $(OBJS)

# source files
bench.o: bench.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c bench.c

getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c getmem.c

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c freemem.c

mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CFLAGS) -c mem_utils.c
