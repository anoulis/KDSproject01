#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define limit1 12
#define limit2 30
#define number 15000000

struct timespec start,end;

/*Η print_time είναι συνάρτηση για την εκτύπωση του χρόνου εκτέλεσης του προγράμματος.
  Είναι υλοποιημένη σύμφωνα με τις οδηγίες των διαλέξεων.*/
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

/*Η read_file είναι συνάρτηση για την ανάγνωση του αρχείου η οποία καλείται στην main
και γενικά εδώ πέρα γίνεται η της σειριακής εξέτασης και των ελέγχων.*/
void read_file(char* filename)
{
/*Εδώ κάνουμε τον άνοιγμα του αρχείου και τον έλεγχο επιτυχίας του,
έτσι ώστε αν για κάποιο λόγο δεν ανοίξει να το καταλάβουμε.    */
  FILE *fp;
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    puts("Error reading file");
    return;
  }

/*Δημιουργία μεταβλητών και ανάθεσh τιμών σε μερικέ απ'αυτές.
Δημιουργούμε size μεγέθους=Αριθμός σειρών*30.Όπου 30 είναι ο αριθμός χαρακτήρων κάθε σειράς.
Κάθε συντεταγμένη αποτελέιται από 9 χαρακτήρες(00.000000)άρα 3*9=18 για κάθε σειρά,
2 χαρακτήρες τα ενδιάμεσα κενά  και 1 χαρακτήρας η αλλαγή σειράς.
Buffer είναι ο πίνακας όπου θα αποθηκευτούν οι χαρακτήρες απ'το διάβασμα.
Count1 είναι ο αριθμός των πραγματικών και count2 ο αριθμός των πραγματικών που βρίσκονται μέσα στα όρια.
Τέλος k,i απλοί μετρητές.*/
  float percentage,x;
  long size =(number*30);
  char *buffer,c[9];
  buffer=(char*)calloc(size,sizeof(char));
  int k=0,i=0,count1=0,count2=0;

/*Διάβασμα και κλείσειμου του αρχείου.
Επίσης έναρξης χρονομέτρησης πριν το διάβασμα.*/
 clock_gettime(CLOCK_MONOTONIC, &start);
 fread(buffer, size, 1, fp);
 fclose(fp);

/*Σε αυτό εδώ το σημείο κάνουμε την σύγκριση για κάθε συντεταγμένη.
Ουσιαστικά διαβάζουμε τον πίνακα χαρακτήρων και τους 9 χαρακτήρες που είναι ενδιάμεσα στα κενά
τους βάζουμε στον πίνακα c και τα μετατρέπουμε σε πραγματικό με την atοf και κάνουμε την σύγκριση.
Εδώ υπολογίζουμε το count1 που είναι ο αριθμός των πραγματικών και count2 ο αριθμός αυτών των
πραγματικών που βρίσκονται μέσα στα όρια.*/
   for(i=0;i<size;i++)
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

/*Εδώ ελευθερώνουμε τον πίνακα χαρακτήρων που δημιουγήσαμε και σταματάμε την χρονομέτρηση.*/
 free(buffer);
 clock_gettime(CLOCK_MONOTONIC, &end);

/* Εδώ κάνουμε τις εκτυπώσεις σχετικά με τον συνολικό αριθμό πραγματικών αριθμό που εξετάσθηκαν,
αυτών που ήταν μέσα στα όρια,το ποσοστό επιτυχίας και με την κλήση της print_time εκτυπώνουμε
και τον χρόνο εκτέλεσης του προγράμματος*/
  printf("Total coordinates read: %d\n", count1);
  printf("Total coordinates inside interval: %d\n", count2);
  percentage = ((float)count2/(float)count1)*100;
  printf("Percentage of coordinates inside interval: %f%c\n", percentage, 37);
  print_time();

}

/*Αυτή είναι η main συνάρτηση όπου γίνεται και η κλήση της read_file,
όπου πραγματοποιούνται όλα όσα απαιτούνται.*/
int main()
{
  read_file("data");
  return 0;
}
