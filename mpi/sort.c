#include <stdio.h>
#include <stdlib.h>

int compare(const void *a_p, const void  *b_p){
   int a = *((int*)a_p);
   int b = *((int*)b_p);

   if (a < b)
      return -1;
   else if (a == b)
      return 0;
   else
      return 1;
}

 
int main(){
  int a[10] = {2,3,5,1,10,-4,21,-22,0,9};

  qsort(a, 10, sizeof(int), compare);

  int i;
  for(i =0; i<10; i++)
    printf("%d\n", a[i]);

  return 0;
}

