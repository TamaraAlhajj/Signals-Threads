#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct thread_data {
   int n;
   int result; } thread_data;

/* Inefficiently, compute successive prime numbers.  
   Return the nth prime number.  */
void* compute_prime (void * arg) {
   /* dereference the parameter */
   thread_data * tdata = (thread_data *)arg;
   int n = tdata->n;
   int candidate = 2;

   while (true) {
      int factor;
      int is_prime = true;

      /* test primality by successive division.  */
      for (factor = 2; factor < candidate; ++factor)
         if (candidate % factor == 0) {
            is_prime = false;
            break;
         }
      /* is prime number we're looking for?  */
      if (is_prime) {
         if (--n == 0) {
            /* define the result  */
		    tdata->result = candidate;
            pthread_exit(NULL);
         }
      }
      ++candidate;
   }
}

int main ()
{
   pthread_t i_tid;
   pthread_t j_tid;
   thread_data i_tdata;
   thread_data j_tdata;
   time_t t;

   /* intialize random number generator */
   srand((unsigned) time(&t));
   i_tdata.n=rand() % 5000;
   j_tdata.n=rand() % 5000;
   printf("i is %d, j is %d\n", i_tdata.n, j_tdata.n);

   /* start the thread, up to "tdata.n" */
   pthread_create (&i_tid, NULL, compute_prime, (void *)&i_tdata);
   pthread_create (&j_tid, NULL, compute_prime, (void *)&j_tdata);

   /* wait for the thread to complete  */
   pthread_join (i_tid, NULL);
   pthread_join (j_tid, NULL);

   /* print the computed prime */
   printf("pi is %d, pj is %d.\n", i_tdata.result, j_tdata.result);
   printf("their product is %d.\n", i_tdata.result * j_tdata.result);
}
