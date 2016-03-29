#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void saveFile(char* filename, float* cords[3],int n)
{
    FILE *fp;
    fp = fopen(filename,"w");
    if (!fp)
    {
        puts("Error opening file\n");
        return;
    }
    int i;
     for(i=0;i<n;i++)
     {
         int k;
         for(k=0;k<3;k++)
         {   if(k<2)
             {   if(cords[i][k]<10)
                 fprintf(fp,"%d%.6f ",0,cords[i][k]);
                 else
                 fprintf(fp,"%.6f ",cords[i][k]);
             }
             else
             {   if(cords[i][k]<10)
                 fprintf(fp,"%d%.6f\n",0,cords[i][k]);
                 else
                 fprintf(fp,"%.6f\n",cords[i][k]);
             }
         }
     }
    fclose(fp);
}

int main(int argc, char *argv[])
{
    if (argc<1 || argc>3)
    {
      printf("Error");
      return 0;
    }
    char* filename=argv[1];
    int coordinate_index=atoi(argv[2]);
    printf("%s %d\n",filename,coordinate_index );
    int utime;
    long int ltime;
    int i,j;
    float **cords;
    cords=calloc(coordinate_index,sizeof(float*));
    for(j=0;j<coordinate_index;j++)
    {
      cords[j]=calloc(3,sizeof(float));
    }
    ltime = time(NULL);
    utime = (unsigned int) ltime/2;
    srand(utime);
    for(i=0;i<coordinate_index;i++)
    {
      cords[i][0]=(float)34*rand()/(RAND_MAX-1);
      cords[i][1]=(float)34*rand()/(RAND_MAX-1);
      cords[i][2]=(float)34*rand()/(RAND_MAX-1);
    }
    saveFile(filename,cords,coordinate_index);
    for (j=0;j<coordinate_index;j++)
    {
      free(cords[j]);
    }
    free(cords);
}
