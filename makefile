CC= gcc
CFLAGS= -g -Wall

all: generate examine examine_parallel

generate: generate.c
	$(CC) $(CFLAGS) generate.c -o generate
	./generate data 15000000

examine: examine.c
	$(CC) $(CFLAGS) examine.c -o examine
	./examine	
	
examine_parallel: examine_parallel.c
	$(CC) $(CFLAGS) examine_parallel.c -o examine_parallel
	./examine_parallel

clean: 
	rm-f generate examine examine_parallel *.o
