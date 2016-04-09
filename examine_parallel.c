#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <unistd.h>


#define limit1 12
#define limit2 30
struct timespec start,end;

int conflictstest(int conflicts)
{
  if(conflicts==0)
   {printf("The program will exit \nbecause the number of conflicts is 0.\n");
    return 0;
    exit(0);
   }
   else if(conflicts==-1)
     {printf("The program will run an example of 150000 conflicts \nas you didn't set a limit and will exit.\n");
      return 150000;
     }
     else
     return conflicts;
}
/*
int timetest(int maxtime)
{  if(maxtime==0)
   {printf("The program will exit \nbecause the maxtime is 0 seconds.\n");
    return 0;
    exit(0);
   }
   else if(maxtime==-1)
     return maxtime;
   else
     return 0;
}
*/
void filetest(char *filename)
{
if( access( filename, F_OK ) == -1 )
 {
    printf("The program will exit \nbecause this file doesn't exist.\n");
    exit(0);
 }
}

int threadstest(int threads)
{
  if(threads==-1)
     return omp_get_max_threads();
  else if(threads==0 || threads<0)
         {printf("The program will exit \nbecause the number of threads is not right.\n");
          exit(0);
         }
  else
  { if(threads<=omp_get_max_threads())
      {omp_set_dynamic(0);
       omp_set_num_threads(threads);
       return threads;
      }
      else
      return omp_get_max_threads();
  }
}
/*
int procstest(int processes)
{if(processes==-1)
     return omp_get_max_threads();
  else if(processes==0 || processes<0)
         {printf("The program will exit \nbecause the number of processes is not right.\n");
          exit(0);
         }
  else
  { if(processes<=)
      {
       return processes;
      }
      else
      return
  }


}
*/
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
int read_file(int argc, char* argv[])
{      int conflicts=atoi(argv[1]);
       int maxtime=atoi(argv[2]);
       char* filename=argv[3];
       int threads=atoi(argv[4]);
       int processes=atoi(argv[5]);

  int number = conflictstest(conflicts);
  filetest(filename);
  threads=threadstest(threads);

  MPI_Init(&argc, &argv);
  MPI_File myfile;
  MPI_Status status;
 // int m = MPI_UNIVERSE_SIZE;
  //printf(" m = %d\n",m);
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
if(buffer==NULL)
    {
        printf("Error! memory not allocated.");
        exit(0);
    }



   MPI_File_open (MPI_COMM_WORLD, filename, MPI_MODE_RDONLY,MPI_INFO_NULL, &myfile);
 if(rank==0)
 clock_gettime(CLOCK_MONOTONIC, &start);
   MPI_File_read_at_all(myfile, start2, buffer, bufsize, MPI_CHAR, &status);
   MPI_File_close(&myfile);

        int omp_size,omp_id;


#pragma omp nowait parallel for private(i,bufsize) reduction(+:count1) reduction(+:count2)
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
  if (argc<1 || argc>6)
    {
      printf("Error");
      return 0;
    }
  read_file(argc,argv);
  return 0;
}
