#include <stdio.h>
#include <stdlib.h>

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
     for(i=0;i<n;i++){
         fprintf(fp,"%f %f %f\n",cords[i][0],cords[i][1],cords[i][2]);
     }
    fclose(fp);
}


int main(int argc, char *argv[]) {
    if (argc<1 || argc>3) {
      printf("Error");
      return;
    }
    char* filename=argv[1];
    int coordinate_index=atoi(argv[2]);
    printf("%s %d\n",filename,coordinate_index );
    int utime;
    long int ltime;
    int i,j;
    float **cords;
    cords=calloc(coordinate_index,sizeof(float*));
    for(j=0;j<coordinate_index;j++) {
      cords[j]=calloc(3,sizeof(float));
    }
    ltime = time(NULL);
    utime = (unsigned int) ltime/2;
    srand(utime);
    for(i=0;i<coordinate_index;i++){
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
