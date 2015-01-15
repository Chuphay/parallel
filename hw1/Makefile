INCS=
LIBS=
OPTS=
#OPTS= -O3
#OPTS= -g

all : bilateralfilter

bilateralfilter : main.cpp BilateralFilter.cpp BilateralFilter.h 
	g++ $(OPTS) -o bilateralfilter main.cpp BilateralFilter.cpp $(LIBS)

clean:
	rm -f *~
	rm -rf *.dSYM
	rm -f bilateralfilter

