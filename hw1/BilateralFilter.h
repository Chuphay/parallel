// 
// BilateralFilter.h
//

#pragma once

class BilateralFilter{
public:
	unsigned char * image;
	unsigned char * rimage ;

	int rows;
	int cols;

	double kernelRadius;
	double ** kernelD;
	double *gaussSimilarity;

	BilateralFilter(unsigned char *image,int rows, int cols,
			double sigmaD, double sigmaR);
	~BilateralFilter();
	unsigned char * runFilter();
	void apply(int i,int j);
	bool isInsideBoundaries(int m,int n);
	double similarity(int p,int s);
	double gauss(double sigma,int x, int y);
	double getSpatialWeight(int m, int n,int i,int j);
	unsigned char getValue(int i, int j);
};
