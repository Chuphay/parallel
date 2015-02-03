#include <stdio.h>

int main(){
  int w,z;
  int q, u[256] , s;
  printf("address of closely spaced: %p %p\n",&w, &z);
  int *p = &w;
  int *r = &z;
  printf("difference %d\n", r - p);
  printf("address of padded: %p %p\n",&q, &s);
  int *pp = &q;
  int *rr = &s;
  printf("difference %d\n", rr - pp);

  return 0;
}
