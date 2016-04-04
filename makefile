CC= gcc
CFLAGS= -Wall -g

all: generate examine examine_parallel

generate: generator.o
	$(CC) generator.o -o generator
	./generator data 15000000

examine: examine.o
	$(CC) examine.o -o examine
	./examine

examine_parallel: examine_parallel.o
	$(CC) -fopenmp examine_parallel.o -o examine_parallel
	./examine_parallel

generator.o: generator.c
	$(CC) $(CFLAGS) -c generator.c

examine.o: examine.c
	$(CC) $(CFLAGS) -c examine.c

examine_parallel.o: examine_parallel.c
	$(CC) -fopenmp $(CFLAGS) -c examine_parallel.c
	
clean:
	rm -f generator examine examine_parallel *.o
