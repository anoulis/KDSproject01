#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>



struct timespec start,end;


void read_file(char* filename,float Array[])
{
  FILE *fp;
  fp = fopen(filename, "r");

  if (fp == NULL){
    puts("Error reading file");
    return;
  }

  int count1=0;
  int count2=0;
  int i=0;
  float x;
  float percentage;
clock_gettime(CLOCK_MONOTONIC, &start);


while (!feof(fp))
  { fscanf(fp,"%f ",&x);
    Array[count1]=x;
    count1++;
  }


  #pragma omp parallel for firstprivate(count1) reduction(+:count2)
  for (i=0;i<count1;i++)
  {
   if (Array[i]>=12 && Array[i]<=30)
    count2++;
  }


  clock_gettime(CLOCK_MONOTONIC, &end);
  fclose(fp);
  printf("Total coordinates read: %d\n",count1);
  printf("Total coordinates inside interval: %d\n",count2);
  percentage = ((float)count2/(float)count1)*100;
  printf("Percentage of coordinates inside interval: %f\n", percentage);
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
    float *Array;
    Array=(float*)calloc(450000,sizeof(float));
    read_file("data",Array);
    print_time();
    free(Array);
}
