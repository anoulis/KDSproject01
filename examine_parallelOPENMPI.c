#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"

#define FILESIZE 15000000*30
#define BUFSIZE 15000000*3
#define FILENAME "data"
/*
int main(int argc, char *argv[])
{
MPI_File myfile;
int rank, size;
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &size);
printf ("Hello world! I am %d of %d \n", rank, size);
printf("paok");
MPI_File_open (MPI_COMM_WORLD, FILENAME, MPI_MODE_CREATE | MPI_MODE_WRONLY,MPI_INFO_NULL, &myfile);


MPI_File_close(&myfile);
MPI_Finalize();
return 0;

//int MPI_File_open(MPI_Comm comm, const char *filename,int amode, MPI_Info info,MPI_File *fh)
}*/




int main(int argc, char* argv[])
{
  int rank, size, bufsize, nints,sz;
  MPI_File myfile;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  nints=BUFSIZE/sizeof(float);
  float *buf;
  buf=(float*)calloc(nints,sizeof(float));
  MPI_File_open (MPI_COMM_WORLD, FILENAME, MPI_MODE_RDONLY,MPI_INFO_NULL, &myfile);
  MPI_File_read_at(myfile, rank*BUFSIZE,buf, nints, MPI_FLOAT, &status);


  MPI_File_close(&myfile);
  MPI_Finalize();
  exit(0);
}


