#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <unistd.h>

#define limit1 12
#define limit2 30

struct timespec start,end;

/*Η conflictstest είναι συνάρτηση που αφορά τον έλεγχο του πρώτου ορίσματος δηλαδή τον αριθμό συντεταγμένων.
  Αν δωθεί 0 τερματίζει το πρόγραμμα και ενημερώνει τον χρήστη.
  Αν δωθεί -1 εκτελεί ένα παράδειγμα με 150000 συντεταγμένες καθώς δεν υπάρχει ένα συγκεκριμένο όριο.
  Αν δωθεί οποιοσδήπωτε άλλος αριθμός τρέχει κανονικά το πρόγραμμα.*/
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

/*Η timetest είναι συνάρτηση που αφορά τον έλεγχο του δεύτερου ορίσματος δηλαδή τον χρόνο εκτέλεσης.
  Αν δωθεί 0 τερματίζει το πρόγραμμα και ενημερώνει τον χρήστη.
  Αν δωθεί -1 εκτελεί το πρόγραμμα μέχρι σύμφωνα με άλλα ορίσματα.
  Αν δωθεί οποιοσδήπωτε άλλος αριθμός τρέχει το πρόγραμμα και το τερματίζει μετά απο το δοθέν διάστημα.
*/
int timetest(int maxtime)
{  if(maxtime==0)
   {printf("The program will exit \nbecause the maxtime is 0 seconds.\n");
    return 0;
    exit(0);
   }
   else if(maxtime==-1)
     return maxtime;
   else
   {printf("The program will exit after sometime.\n");
    return maxtime;
    exit(0);
     }
}

/*Η filetest είναι συνάρτηση που αφορά τον έλεγχο του τρίτου ορίσματος δηλαδή την ύπαρξη του αρχείου.
  Αν δεν βρεθεί τον αρχείο με το συγκεκριμένο όνομα τερματίζει το πρόγραμμα.*/
void filetest(char *filename)
{
if( access( filename, F_OK ) == -1 )
 {
    printf("The program will exit \nbecause this file doesn't exist.\n");
    exit(0);
 }
}

/*Η threadstest είναι συνάρτηση που αφορά τον έλεγχο του τέταρτου ορίσματος.
  Αν δωθεί 0 ή αρνητικός αριθμός τερματίζει το πρόγραμμα και ενημερώνει τον χρήστη.
  Αν δωθεί -1 εκτελεί το πρόγραμμα με τον μέγιστο αριθμό threads.
  Αν δωθεί οποιοσδήπωτε άλλος αριθμός τρέχει το πρόγραμμα είτε με αυτόν τον αριθμό threads
  είτε με τον μέγιστο δυνατό αν αυτός ο αριθμός υπερβαίνει το αριθμό διαθέσιμων threads.*/
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

/*Η procstest είναι συνάρτηση που αφορά τον έλεγχο του πέμπτου ορίσματος.
  Αν δωθεί 0 ή αρνητικός αριθμός τερματίζει το πρόγραμμα και ενημερώνει τον χρήστη.
  Αν δωθεί -1 εκτελεί το πρόγραμμα με τον μέγιστο αριθμό processes.
  Αν δωθεί οποιοσδήπωτε άλλος αριθμός τρέχει το πρόγραμμα είτε με αυτόν τον αριθμό processes
  είτε με τον μέγιστο δυνατό αν αυτός ο αριθμός υπερβαίνει το αριθμό διαθέσιμων processes.*/
int procstest(int processes)
{if(processes==-1)
     return omp_get_num_procs();
  else if(processes==0 || processes<0)
         {printf("The program will exit \nbecause the number of processes is not right.\n");
          exit(0);
         }
  else
  { if(processes<=omp_get_num_procs())
      {
       return processes;
      }
      else
      return omp_get_num_procs();
  }
}

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
και γενικά εδώ πέρα γίνεται η υλοποίηση του OPENMP και του OPENMPI
και των ελέγχων.*/
int read_file(int argc, char* argv[])
{
    /*Δημιουργία μεταβλητών και
    ανάθεση τιμών σύμφωνα με τα δοθέν ορίσματα.*/
       int conflicts=atoi(argv[1]);
       int maxtime=atoi(argv[2]);
       char* filename=argv[3];
       int threads=atoi(argv[4]);

   /*Κλήση των συναρτήσεων ελέγχου του κάθε ορίσματος
   και ανέθεση τιμών.*/
  int number = conflictstest(conflicts);
  filetest(filename);
  threads=threadstest(threads);
  maxtime=timetest(maxtime);

  /*Αρχικοποίση του OPENMPI,δημιουργία διάφορων μεταβλητών
  και ανάθεση τιμών σε κάποιες απ'αυτές.*/
  MPI_Init(&argc, &argv);
  MPI_File myfile;
  MPI_Status status;
  int i,k=0,rank, size, bufsize,count1=0,count1_gloabl,count2=0,count2_gloabl;
  float percentage,x;
  MPI_Offset start2;
  MPI_Offset end2;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

/*Εδώ πέρα ουσιαστικά γίνεται η δημιουργία του bufsize.
Η λογική είναι η εξής:
Χωρίζουμε το διάβασμα του αρχείου ανά σειρές ανάλογα του αριθμού των διεργασιών.
Αν για παράδειγμα εχουμε 12 σειρές και 4 διεργασίες τότε προχωράμε στην πρώτη συνθήκη.
Δημιουργούμε bufsize μεγέθους=Αριθμός σειρών*30.Όπου 30 είναι ο αριθμός χαρακτήρων κάθε σειράς.
Κάθε συντεταγμένη αποτελέιται από 9 χαρακτήρες(00.000000)άρα 3*9=18 για κάθε σειρά,
2 χαρακτήρες τα ενδιάμεσα κενά  και 1 χαρακτήρας η αλλαγή σειράς.
Μετά βάζουμε κάθε δείκτη να δείχνει σε σημεία ανάλογα του αριθμού διεργασίας μέσα στον πίνακα bufsize.
Για παράδειγμα,η διεργασία 0 έχει start2=0 και end2=bufsize-1 και η 1 start2=1*bufsize και end2=start2 + bufsize-1.

Αν όμως ο αριθμός σειρών δεν διαιρείται απόλυτα με τον αριθμό διεργασιών,παίρνουμε το υπόλοιπο σειρών που απομένει,
το αφαιρούμαι από το αρχικό,μοιράζουμε κανονικά τις σειρές σε όλες τις διεργασίες και απλά προσθέτουμε και τις υπολοιπόμενες
στην τελευταία διεργασία.Για παράδειγμα,με αριθμό σειρών 5 και αριθμό διεργασιών 4,
θα πάρουν όλες οι διεργασίες από 1 σειρά και η τελευταία διεργασία θα πάρει και την τελευταία που απομένει.
*/
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

/*Δυναμική διμιουργία πίνακα χαρακτήρων μεγέθους bufsize και έλεγχος δημιουργίας του.
Επίσης δημιουργία και ενός πίνακα χαρακτήρων 9 θέσεων όπου θα γεμίζει με τους χαρακτήρες
των πραγματικών αριθμών.*/
     char *buffer,c[9];
     buffer=(char*)calloc(bufsize,sizeof(char));
     if(buffer==NULL)
       {
        printf("Error! memory not allocated.");
        exit(0);
       }

/*Άνοιγμα,διάβασμα και κλείσιμο αρχείου σύμφωνα με τον OPEMPI.
Επίσης έναρξης χρονομέτρησης από την διεργασία 0 πριν το διάβασμα.*/
   MPI_File_open (MPI_COMM_WORLD, filename, MPI_MODE_RDONLY,MPI_INFO_NULL, &myfile);
 if(rank==0)
 clock_gettime(CLOCK_MONOTONIC, &start);
   MPI_File_read_at_all(myfile, start2, buffer, bufsize, MPI_CHAR, &status);
   MPI_File_close(&myfile);

/*Σε αυτό εδώ το σημείο κάνουμε την σύγκριση για κάθε συντεταγμένη με την βοήθεια του OPENMP.
Ουσιαστικά διαβάζουμε τον πίνακα χαρακτήρων και τους 9 χαρακτήρες που είναι ενδιάμεσα στα κενά
τους βάζουμε στον πίνακα c και τα μετατρέπουμε σε πραγματικό με την atοf και κάνουμε την σύγκριση.
Εδώ υπολογίζουμε το count1 που είναι ο αριθμός των πραγματικών και count2 ο αριθμός αυτών των
πραγματικών που βρίσκονται μέσα στα όρια.*/
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

/*Εδώ ελευθερώνουμε τον πίνακα χαρακτήρων που δημιρουγίσαμε και σταματάμε την χρονομέτρηση.*/
  free(buffer);
  if(rank==0)
  clock_gettime(CLOCK_MONOTONIC, &end);

/*Αθροοίζουμε συνολικά το πλήθος των συντεταγμένων και των αριθμών που βρίσκονται μέσα στα όρια
Και επίσης τερματίζουμε την MPI λειτουρία.*/
   MPI_Reduce(&count1, &count1_gloabl, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Reduce(&count2, &count2_gloabl, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
   MPI_Finalize();

/* Εδώ κάνουμε τις εκτυπώσεις σχετικά με τον συνολικό αριθμό πραγματικών αριθμό που εξετάσθηκαν,
αυτών που ήταν μέσα στα όρια,το ποσοστό επιτυχίας και με την κλήση της print_time εκτυπώνουμε
και τον χρόνο εκτέλεσης του προγράμματος*/
if(rank==0)
{
  printf("Total coordinates read: %d\n",count1_gloabl);
  printf("Total coordinates inside interval: %d\n",count2_gloabl);
  percentage = ((float)count2_gloabl/(float)count1_gloabl)*100;
  printf("Percentage of coordinates inside interval: %f%c\n", percentage,37);
  print_time();
}

/*Πραγματοποιείται έξοδος απ το πρόγραμμα αφού έχουν ολοκληρωθεί τα πάντα.  */
exit(0);
}

/*Αυτή είναι η main συνάρτηση όπου γίνεται έλεγχος αν είναι σωστός αριθμός των παραμέτρων εισαγωγής.
Επίσης γίνεται και η κλήση της read_file όπου πραγματοποιούνται όλα όσα απαιτούνται.   */
int main(int argc, char* argv[])
{
  if (argc<1 || argc>5)
    {
      printf("Error");
      return 0;
    }
  read_file(argc,argv);
  return 0;
}
