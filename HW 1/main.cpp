// Basic Bilateral Filter.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "BilateralFilter.h"

using namespace std;

void
pgmWriteHeader ( ostream &f, int width, int height, int maxval )
{
  //  fprintf ( f, "P5\n%d %d\n%d\n", width, height, maxval );
  f << "P5\n" << width << ' ' << height << endl << maxval << endl;
}

unsigned char *
pgmReadFile ( string filename, int &rows, int &cols )
{
  ifstream f;
  f.open(filename);
  if (!f) {
    cerr << "cannot open " << filename << endl;
    exit(1);
  }
  // read magic number P5 for raw pgm file
  string magic;
  getline(f, magic);
  // consume any comment lines
  string line;
  do {
    getline(f, line);
  } while (line[0]=='#');
  // read the width and height
  stringstream ss (line);
  ss >> cols >> rows;
  // read the max value (not used)
  int maxval;
  getline(f,line);
  ss.str(line);
  ss >> maxval;
  // now the image data
  unsigned char *data = new unsigned char [rows*cols];
  f.read ( (char*)data, rows*cols );
  f.close();
  return data;
}

void
pgmWriteFile ( string filename, unsigned char *image, int rows, int cols )
{
  ofstream f;
  f.open(filename);
  if (!f) {
    cerr << "can't open " << filename << endl;
    exit(2);
  }
  pgmWriteHeader ( f, cols, rows, 255 );
  f.write( (char*)image, rows * cols );
  f.close();
} 

int main ( int argc, char **argv)

{
  string filename;
#ifdef __WIN32__
  filename = ".\\images\\cat.pgm";
#else
  filename = "images/cat.pgm";
#endif


  if (argc == 2)
    filename = argv[1];

  int rows, cols;
  unsigned char *image = pgmReadFile(filename,rows, cols);
    
  double spacestdv = 10;
  double rangestdv = 6;
  BilateralFilter * bilateral = new BilateralFilter(image, rows, cols,
						    spacestdv,rangestdv);

  unsigned char * bilateralImage = bilateral->runFilter();

  pgmWriteFile(filename+".out.pgm", bilateralImage, rows, cols);
	
  return 0;
}

