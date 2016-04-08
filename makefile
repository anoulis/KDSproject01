CC= gcc
#CC2= mpicc
CFLAGS= -Wall -g

all: generate examine examine_parallel 

generate: generator.o
	$(CC) generator.o -o generator
	./generator data 15000000

examine: examine.o
	$(CC) examine.o -o examine
	./examine

examine_parallel: 
#	$(CC2) examine_parallel.o -o examine_parallel
	mpicc -openmp examine_parallel.c -o examine_parallel	
	mpirun -np 4 ./examine_parallel

generator.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

examine.o: examine.c
	$(CC) $(CFLAGS) -c examine.c

#examine_parallel.o: examine_parallel.c
#	$(CC2) -openmp $(CFLAGS) -c examine_parallel.c
	
clean:
	rm -f generator examine examine_parallel *.o
