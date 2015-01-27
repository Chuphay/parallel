//
// BilateralFilter.cpp
//

//g++ -fopenmp -o bilateral_omp main.cpp bilateral_omp.cpp

#include "BilateralFilter.h"
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <omp.h>

int max(int num1, int num2){
  if(num1>num2)
    return num1;
  else
    return num2;
}

unsigned char BilateralFilter::getValue(int i,int j)
{
  unsigned char pixelValue = image[cols*i+j];
  return pixelValue;
}


BilateralFilter::BilateralFilter(unsigned char *_image, 
				 int _rows, int _cols,
				 double sigmaD, double sigmaR)
  : rows(_rows), cols(_cols)
{
		
  image = new unsigned char [rows*cols];
  memcpy ( image, _image, rows*cols );

  int sigmaMax = max(sigmaD, sigmaR);
  this->kernelRadius = ceil((double)2 * sigmaMax);

  
  double twoSigmaRSquared = 2 * sigmaR * sigmaR;

  int kernelSize = this->kernelRadius * 2 + 1;

  kernelD = new double*[kernelSize];
  for(int i=0; i<kernelSize;i++){
    kernelD[i] = new double[kernelSize];
  }

  int center = (kernelSize - 1) / 2;

  for (int x = -center; x < -center + kernelSize; x++) {
    for (int y = -center; y < -center + kernelSize; y++) {
      kernelD[x + center][y + center] = this->gauss(sigmaD, x, y);
    }
  }

		
  gaussSimilarity = new double[256];
  for (int i = 0; i < 256; i++) {
    gaussSimilarity[i] = exp((double)-((i) / twoSigmaRSquared));
  }

  rimage = new unsigned char [rows*cols];
  memcpy ( rimage, image, rows*cols );
}

BilateralFilter::~BilateralFilter()
{
  if ( image )
    delete [] image;
  if ( rimage )
    delete [] rimage;
}


unsigned char * BilateralFilter::runFilter(){

#pragma omp parallel for            
  for(int i=0;i<rows;i++){
    //printf("num threads: %d\n", omp_get_num_threads());
    for (int j=0;j<cols;j++){
			
      apply(i,j);
			
    }
  }
    
  return rimage;

}

double BilateralFilter::getSpatialWeight(int m, int n,int i,int j){
  return kernelD[(int)(i-m + kernelRadius)][(int)(j-n + kernelRadius)];
}


void BilateralFilter::apply(int i, int j) {// ~i=y j=x

  if(i>0 && j>0 && i<rows && j< cols){
    double sum = 0;
    double totalWeight = 0;
    int intensityCenter = getValue(i,j);


    int mMax = i + kernelRadius;
    int nMax = j + kernelRadius;
    double weight;

#pragma omp parallel for
    for (int m = i-kernelRadius; m < mMax; m++) {
      for (int n = j-kernelRadius; n < nMax; n++) {

	if (this->isInsideBoundaries(m, n)) {
	  int intensityKernelPos = getValue(m,n);
	  weight = getSpatialWeight(m,n,i,j) * similarity(intensityKernelPos,intensityCenter);
	  totalWeight += weight;
	  sum += (weight * intensityKernelPos);
	}
      }
    }
    unsigned char newvalue=(unsigned char)floor(sum / totalWeight);
		
    rimage[cols*i+j] = newvalue;
  }

}

double BilateralFilter::similarity(int p, int s) {
  // this equals: Math.exp(-(( Math.abs(p-s)) /  2 * this->sigmaR * this->sigmaR));
  // but is precomputed to improve performance
  return this->gaussSimilarity[abs(p-s)];
}


double BilateralFilter::gauss (double sigma, int x, int y) {
  return exp(-((x * x + y * y) / (2 * sigma * sigma)));
}

	
bool BilateralFilter::isInsideBoundaries(int m,int n){
  if(m>-1 && n>-1 && m<rows && n<cols)
    return true;
  else 
    return false;
}
