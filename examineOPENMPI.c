#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>


#define limit1 12
#define limit2 30
#define number 15000000
#define FILESIZE number*30

struct timespec start,end;

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
int read_file(char* filename,int argc, char* argv[])
{
  MPI_Init(&argc, &argv);
  MPI_File myfile;
  MPI_Status status;
  int rank, size, bufsize,count2=0,count2_gloabl,i,k=0,count1=0,count1_gloabl;
  float percentage,x;
  MPI_Offset start2;
  MPI_Offset end2;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);


  if(number%size==0)
  { bufsize=(number/size)*30;
    if(rank==size-1)
       {
          start2 = rank * bufsize;
          end2   = start2 + bufsize;
       }
       else
       {
         start2 = rank * bufsize;
         end2   = start2 + bufsize - 1;
       }
  }
  else
  {
     if(rank==size-1)
       {   bufsize=((number-(number%size))/size)*30+(number%size)*30;
          start2 = rank * ((number-(number%size))/size)*30;
          end2   = start2 + bufsize ;
       }
       else
       {  bufsize=((number-(number%size))/size)*30;
         start2 = rank * bufsize;
         end2  = start2 + bufsize - 1;
       }
  }

  char *buffer,c[9];
  buffer=(char*)calloc(bufsize,sizeof(char));

   MPI_File_open (MPI_COMM_WORLD, filename, MPI_MODE_RDONLY,MPI_INFO_NULL, &myfile);
 if(rank==0)
 clock_gettime(CLOCK_MONOTONIC, &start);
   MPI_File_read_at_all(myfile, start2, buffer, bufsize, MPI_CHAR, &status);
   MPI_File_close(&myfile);

for(i=0;i<bufsize;i++)
    { if(buffer[i]!=' ' && buffer[i]!='\n')
      {
        c[k]=buffer[i];
        k++;
      }
      else
      {
       x=atof(c);
       count1++;
         if (x>=limit1 && x<=limit2)
            count2++;
       k=0;
      }
    }

  free(buffer);
  if(rank==0)
  clock_gettime(CLOCK_MONOTONIC, &end);


   MPI_Reduce(&count1, &count1_gloabl, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Reduce(&count2, &count2_gloabl, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);

   MPI_Finalize();

if(rank==0)
{
  printf("Total coordinates read: %d\n",count1_gloabl);
  printf("Total coordinates inside interval: %d\n",count2_gloabl);
  percentage = ((float)count2_gloabl/(float)count1_gloabl)*100;
  printf("Percentage of coordinates inside interval: %f%c\n", percentage,37);
  print_time();
}
exit(0);
}





int main(int argc, char* argv[])
{
  read_file("data",argc,argv);
  return 0;
}
