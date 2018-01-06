all:	predictors

predictors:	reddy_prog1.cpp
	g++  reddy_prog1.cpp -g -o predictors


clean:
	rm -f *.o predictors

