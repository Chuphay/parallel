#include <stdio.h>
#include <omp.h>

//gcc -g -Wall -fopenmp -o sort sort.c

void print_ls(int *ls){
  int i;
  for(i= 0 ; i < 10; i ++){
    (i<9)? printf("%d, ",ls[i]):  printf("%d ",ls[i]);
  }
  printf("\n");
}

void bubble_sort(int (*ls)[], int length){
  int list_length, i;
  for(list_length = length; list_length > 1; list_length--){
    for(i = 0; i < list_length - 1; i++){
      if((*ls)[i]> (*ls)[i+1]){
	int temp = (*ls)[i];
	(*ls)[i] = (*ls)[i+1];
	(*ls)[i+1] = temp;
      }
    }
  }
}

void swap(int (*a), int (*b)){
  int tmp = *b;
  *b = *a;
  *a = tmp;
}

void serial_even_odd(int a[], int length){
  int phase, i;
# pragma omp parallel default(none) shared(a, length) private(phase, i)
  for (phase = 0; phase<length; phase++){
    if(phase%2 == 0){
# pragma omp for
      for(i =0; i< length; i += 2)
	if(a[i-1] > a[i]) swap(&a[i-1], &a[i]);
    } else {
# pragma omp for
      for(i = 0; i < length; i += 2)
	if(a[i]>a[i+1]) swap(&a[i], &a[i+1]);
    }
  }
}


int main(){

  int ls[10] = {3,6,1,10,85,49,2,13,0,65};

#pragma omp parallel
  {
    printf("yo\n");
  }

  print_ls(ls);
  //bubble_sort(&ls, 10);
  print_ls(ls);
  serial_even_odd(ls,10);
  print_ls(ls);


  return 0;
}
