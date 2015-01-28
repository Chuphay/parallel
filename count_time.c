#include <stdio.h>
#include <sys/time.h>

//I cannot get this program to work!

double timedifference_msec(struct timeval t0, struct timeval t1)
{
  return  (t1.tv_usec - t0.tv_usec) / 100000.0f; //(t1.tv_sec - t0.tv_sec)* 1000.0f +
}

int main ()
{

   struct timeval t0;
   struct timeval t1;
   double elapsed;

   gettimeofday(&t0, 0);
   /* ... YOUR CODE HERE ... */

  int i;
  int a = 1;
  for(i = 0; i< 100*1024*1024; i++) a *= 3;


   gettimeofday(&t1, 0);

 elapsed = timedifference_msec(t0, t1);

  printf("elapsed: %f\n", elapsed);
  
  return(0);
}
