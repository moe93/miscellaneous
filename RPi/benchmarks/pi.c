#include <stdio.h>
#include <time.h>

#define RES 	100000000

// To compile:
//   > g++ -o OUTPUT_NAME FILE.c
double piApprox( int num )
{
	double pi 	= 4.0;
	double k  	= 1.0;
	double est 	= 1.0;
	
	while( 1<num )
	{
		k = k+2;
		est = est - (double)(1./k) + (double)1./(k+2);
		num = num - 1;
		k = k+2;
	}
	
	return( pi*est );
}

int main (void)
{	
	double p = 0;
	printf( "\n#############################\n" );
	printf( "            C CODE           \n" );
	printf( "#############################\n\n" );
	printf( "Approximating pi using %d iterations\n", RES );
	clock_t start = clock(), diff;
	p = piApprox(RES);
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
	printf( "pi = %.6lf\n\n", p );
	return 0;
}
