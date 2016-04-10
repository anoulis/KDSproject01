CC= gcc
CC2= mpicc
CFLAGS= -Wall -g

all: generate examine examine_parallel 

generate: generator.o
	$(CC) generator.o -o generator
	./generator data 15000000

examine: examine.o
	$(CC) examine.o -o examine
	./examine

examine_parallel: examine_parallel.o
	$(CC2) -fopenmp examine_parallel.o -o examine_parallel
	mpirun -np 4 ./examine_parallel 15000000 -1 data -1 -1

generator.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

examine.o: examine.c
	$(CC) $(CFLAGS) -c examine.c

examine_parallel.o: examine_parallel.c
	$(CC2) -fopenmp $(CFLAGS) -c examine_parallel.c
	
clean:
	rm -f generator examine examine_parallel *.o
