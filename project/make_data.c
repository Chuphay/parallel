#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double uniform(){
  return rand()/((double)RAND_MAX + 1.0);
}

double fake_normal(double mu, double sigma){

  double once = (2*uniform() - 1);
  double twice = (2*uniform() - 1);
  double thrice = (2*uniform() - 1);
  double quatro = (2*uniform() - 1);
 
  return sigma*(once+twice+thrice+quatro) + mu;
}

void write_data(FILE *fp, int n, double mu1, double sigma1, double mu2, double sigma2){

  for(int i = 0; i<n; i++) 
    fprintf(fp, "%f %f\n", fake_normal(mu1, sigma1), fake_normal(mu2, sigma2));

}


int main(){
  srand(time(NULL));

  FILE *fp = fopen("data.txt", "w");

  write_data(fp, 100, 4, 1, 2, 1);
  write_data(fp, 100, -10, 1, -5, 2);


  int closed = fclose(fp);

  FILE *classes = fopen("classes.txt" ,"w");
  for(int i = 0; i< 200; i++) fprintf(classes, "%d\n", i<100? 1 : 2);
  printf("%s\n", closed == 0 ? "success": "fail");
  return 0;
} 
