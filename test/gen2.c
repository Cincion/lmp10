#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double fi( double x, double i ) {

	if ( i == 0 ) {
                return  1;
        } else if ( i == 1 ) {
                return  x;
        } else {
                return  pow(x, i);
        }

}

int main( int argc, char **argv ) {
				int n= argc > 1 ? atoi( argv[1] ) : 100;
				double a= argc > 2 ? atof( argv[2] ) : -3;
				double b= argc > 3 ? atof( argv[3] ) : 2;
				double c= argc > 4 ? atof( argv[4] ) : 0;
				FILE *out= argc > 5 ? fopen( argv[5], "w" ) : stdout;

				srand( argc > 6 ? atoi(argv[6]) : time(NULL) );

				int i;
				double dx = (b-a)/(n-1);

				for( i =0; i < n; i++ ) {
								fprintf( out, "%g %g\n", a+i*dx, fi(a+i*dx, c) );
				}
				fclose( out );

				return 0;
}
