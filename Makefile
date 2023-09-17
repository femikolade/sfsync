CC=gcc
CXX=g++
RM=rm -f
OBJS=*.o *.out
SRCS=main.cc
FUNCS=config.c
CFLAGS=-w

all: 
	$(CC) $(SRCS) $(FUNCS) -o main.out -lssh $(CFLAGS)

clean:
	$(RM) $(OBJS)
