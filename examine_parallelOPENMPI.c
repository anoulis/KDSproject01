#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>


#define limit1 12
#define limit2 30
#define number 15000000
#define FILESIZE number*30

struct timespec start,end;



int read_file(char* filename,int argc, char* argv[])
{
  int rank, size, bufsize, nints,sz,count2=0,i,k=0,count1=0;
  float *Array,percentage,x;
  Array=(float*)calloc(number*3,sizeof(float));
  MPI_File myfile;
  MPI_Status status;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  bufsize=FILESIZE/size;
  nints=bufsize/sizeof(char);
  char *buffer,c[9];
  buffer=(char*)calloc(nints,sizeof(char));
  MPI_File_open (MPI_COMM_WORLD, filename, MPI_MODE_RDONLY,MPI_INFO_NULL, &myfile);
  clock_gettime(CLOCK_MONOTONIC, &start);
  MPI_File_read_at(myfile, rank*bufsize,buffer, nints, MPI_CHAR, &status);

  for (i=0;i<nints;i++)
  {
    printf("%d->%c\n",i,buffer[i]);
  }

for(i=0;i<nints;i++)
    { if(buffer[i]!=' ' && buffer[i]!='\n')
      {
        c[k]=buffer[i];
        k++;
      }
      else
      {
       Array[count1]=atof(c);
        count1++;
        k=0;
      }
    }
#pragma omp parallel for firstprivate(count1) reduction(+:count2)
  for (i=0;i<count1;i++)
  {
   if (Array[i]>=limit1 && Array[i]<=limit2)
    count2++;
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  MPI_File_close(&myfile);
  MPI_Finalize();

  printf("Total coordinates read: %d\n",count1);
  printf("Total coordinates inside interval: %d\n",count2);
  percentage = ((float)count2/(float)count1)*100;
  printf("Percentage of coordinates inside interval: %f%c\n", percentage,37);

}

void print_time()
{
  const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
  long timeElapsed_s = end.tv_sec -start.tv_sec;
  long timeElapsed_n = end.tv_nsec -start.tv_nsec;
  if ( timeElapsed_n < 0 )
  {
    timeElapsed_n = DAS_NANO_SECONDS_IN_SEC + timeElapsed_n;
    timeElapsed_s--;
  }
  printf("Time: %ld.%09ld secs \n",timeElapsed_s,timeElapsed_n);
}

int main(int argc, char* argv[])
{
    read_file("data",argc,argv);
   print_time();
    exit(0);
}
