#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

struct timespec start,end;

void read_file(char* filename)
{
  FILE *fp;
  fp = fopen(filename, "r");

  if (fp == NULL)
  {
    puts("Error reading file");
    return;
  }

   float *Array,x,percentage;
   Array=(float*)calloc(15000000*3,sizeof(float));
   long size =(15000000*30),size2;
   char *buffer,c[9];
   buffer=(char*)calloc(size,sizeof(char));
   int flag=0,k=0,i=0,count1=0,count2=0;

 clock_gettime(CLOCK_MONOTONIC, &start);
  fread(buffer, size, 1, fp);
    for(i=0;i<size;i++)
    { if(buffer[i]!=' ' && buffer[i]!='\n')
      {
        c[k]=buffer[i];
        k++;
      }
      else
      {
        Array[flag]=atof(c);
        flag++;
        k=0;
      }
    }
  count1=flag;
 #pragma omp parallel for firstprivate(count1) reduction(+:count2)
  for (i=0;i<count1;i++)
  {
   if (Array[i]>=12 && Array[i]<=30)
    count2++;
  }

  free(buffer);
  free(Array);

  clock_gettime(CLOCK_MONOTONIC, &end);
  fclose(fp);
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

int main()
{
    read_file("data");
    print_time();
}
