
CC = gcc
CFLAGS = --std=gnu99 -W -Wall -Wextra -O3 --fast-math
ARCH = -march=haswell
DEBUG = -g
LDFLAGS = -lm
PROGS = prova
CFILES = simulatore.c

all: 
	$(CC) $(CFLAGS) $(CFILES) -o $(PROGS) $(LDFLAGS)

arch:
	$(CC) $(ARCH) $(CFLAGS) $(CFILES) -o $(PROGS) $(LDFLAGS)

debug:
	$(CC) $(DEBUG) $(CFLAGS) $(CFILES) -o $(PROGS) $(LDFLAGS)

archdbg:
	$(CC) $(ARCH) $(DEBUG) $(CFLAGS) $(CFILES) -o $(PROGS) $(LDFLAGS)

