#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

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

void write_circle_data(FILE *fp, int n, double radius, double sigma){
  if(n%2 != 0){
    printf("n has to be divisible by 2\n");
    exit(1);
  }

  for(int i = 0; i<n/2; i++){   
    double x = radius*(2*uniform()-1);
    double y = sqrt(radius*radius - x*x);
    if(uniform()>0.5) y *= -1;
    fprintf(fp, "%f %f\n", x, y);
  }
  for(int i = 0; i<n/2; i++){   
    double y = radius*(2*uniform()-1);
    double x = sqrt(radius*radius - y*y);
    if(uniform()>0.5) x *= -1;
    fprintf(fp, "%f %f\n", x, y);
  }


}


void write_normal_data(FILE *fp, int n, double mu1, double sigma1, double mu2, double sigma2){

  for(int i = 0; i<n; i++) 
    fprintf(fp, "%f %f\n", fake_normal(mu1, sigma1), fake_normal(mu2, sigma2));

}


int main(){
  srand(time(NULL));

  FILE *fp = fopen("data.txt", "w");

  //write_normal_data(fp, 100, 4, 1, 2, 1);
  //write_normal_data(fp, 100, -10, 1, -5, 2);
  write_circle_data(fp, 100, 10, 1);


  int closed = fclose(fp);

  FILE *classes = fopen("classes.txt" ,"w");
  //for(int i = 0; i< 100; i++) fprintf(classes, "%d\n", 1);
  //for(int i = 0; i< 100; i++) fprintf(classes, "%d\n", 2);
  for(int i = 0; i< 100; i++) fprintf(classes, "%d\n", 3);

  printf("%s\n", closed == 0 ? "success": "fail");
  return 0;
} 
