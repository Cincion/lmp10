#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

/* UWAGA: liczbę używanych f. bazowych można ustawić przez wartość
          zmiennej środowiskowej APPROX_BASE_SIZE
*/

/*
 * Funkcje bazowe: n - liczba funkcji a,b - granice przedzialu aproksymacji i
 * - numer funkcji x - wspolrzedna dla ktorej obliczana jest wartosc funkcji
 */

double
fi(double a, double b, int n, int i, double x)
{	
	int 	j;
	double  h[n];
	if ( i > 1 ) {
		h[0] = 1;
		h[1] = 2 * x;
		for ( j = 2; j <= n; j++ ) {
			h[j] = 2 * x * h[ j - 1 ] - 2 * ( j - 1 ) * h[ j - 2 ];
		}
		return h[i];
	} else if ( i = 0 ) {
		return 1;
	} else if ( i = 1 ) {
		return 2 * x;
	}
	


}



/* Pierwsza pochodna fi */
double
dfi(double a, double b, int n, int i, double x)
{
	int j;
	double h[n];
        double h1[n];	
	
        if ( i > 1 ) {
                h[0] = 1;
                h[1] = 2;
                h1[0] = 0;
                h1[1] = 2;
                for ( j = 2; j <= i; j++ ) {
                        h[j] = 2 * x * h[ j - 1 ] - 2 * ( j - 1 ) * h[ j - 2 ];
                }
                for ( j = 2; j <= i; j++ ) {	
                        h1[j] = 2 * h[ j - 1 ] + 2 * x * h1[ j - 1 ] - 2 * ( j - 1 ) * h1[ j - 2 ];
                }
		return h1[i];
        } else if ( i = 0 ) {
		return 0;
	} else if ( i = 1 ) {
		return 2;
	}


}

/* Druga pochodna fi */
double
d2fi(double a, double b, int n, int i, double x)
{	
	int j;
        double h[n];
        double h1[n];
	double h2[n];
        if ( i > 1 ) {
                h[0] = 1;
                h[1] = 2;
                h1[0] = 0;
                h1[1] = 2;
		h2[0] = 0;
		h2[1] = 1;
                for ( j = 2; j <= i; j++ ) {
                        h[j] = 2 * x * h[ j - 1 ] - 2 * ( j - 1 ) * h[ j - 2 ];
                }
                for ( j = 2; j <= i; j++ ) {
                        h1[j] = 2 * h[ j - 1 ] + 2 * x * h1[ j - 1 ] - 2 * ( j - 1 ) * h1[ j - 2 ];
                }
		for ( j = 2; j <= i; j++ ) {
                        h2[j] = 4 * h1[ j - 1 ] + 2 * x * h2[ j - 1 ] - 2 * ( j - 1 ) * h2[ j - 2 ];
                }
		return h2[i];
	} else if ( i = 0 ) {
                return 0;
        } else if ( i = 1 ) {
                return 0;
        }

	

}

/* Trzecia pochodna fi */
double
d3fi(double a, double b, int n, int i, double x)
{	
	int j;
        double h[n];
        double h1[n];
        double h2[n];
	double h3[n];
        if ( i > 1 ) {
                h[0] = 1;
                h[1] = 2;
                h1[0] = 0;
                h1[1] = 2;
                h2[0] = 0;
                h2[1] = 1;
		h3[0] = 0;
    		h3[1] = 0;
                for ( j = 2; j <= i; j++ ) {
                        h[j] = 2 * x * h[ j - 1 ] - 2 * ( j - 1 ) * h[ j - 2 ];
                }
                for ( j = 2; j <= i; j++ ) {
                        h1[j] = 2 * h[ j - 1 ] + 2 * x * h1[ j - 1 ] - 2 * ( j - 1 ) * h1[ j - 2 ];
                }
                for ( j = 2; j <= i; j++ ) {
                        h2[j] = 4 * h1[ j - 1 ] + 2 * x * h2[ j - 1 ] - 2 * ( j - 1 ) * h2[ j - 2 ];
                }
		for ( j = 2; j <= i; j++ ) {
                        h3[j] = 4 * h2[ j - 1 ] + 2 * h3[ j - 1 ] + 2 * x * h3[ j - 1 ] - 2 * ( j - 1 ) * h3[ j - 2 ];
                }

                return h3[i];
        } else if ( i = 0 ) {
                return 0;
        } else if ( i = 1 ) {
                return 0;
        }

}

/* Pomocnicza f. do rysowania bazy */
/*
double
xfi(double a, double b, int n, int i, FILE *out)
{
	double		h = (b - a) / (n - 1);
	double		h3 = h * h * h;
	int		hi         [5] = {i - 2, i - 1, i, i + 1, i + 2};
	double		hx      [5];
	int		j;

	for (j = 0; j < 5; j++)
		hx[j] = a + h * hi[j];

	fprintf( out, "# nb=%d, i=%d: hi=[", n, i );
	for( j= 0; j < 5; j++ )
		fprintf( out, " %d", hi[j] );
	fprintf( out, "] hx=[" );
	for( j= 0; j < 5; j++ )
		fprintf( out, " %g", hx[j] );
	fprintf( out, "]\n" );
}
*/
void
make_spl(points_t * pts, spline_t * spl)
{

	matrix_t       *eqs= NULL;
	double         *x = pts->x;
	double         *y = pts->y;
	double		a = x[0];
	double		b = x[pts->n - 1];
	int		i, j, k;
	int 		nb;
/*	int		nb = pts->n - 3 > 10 ? 10 : pts->n - 3;
  char *nbEnv= getenv( "APPROX_BASE_SIZE" );

	if( nbEnv != NULL && atoi( nbEnv ) > 0 )
		nb = atoi( nbEnv );
*/	nb = 3;

	eqs = make_matrix(nb, nb + 1);

#ifdef DEBUG
#define TESTBASE 500
	{
		FILE           *tst = fopen("debug_base_plot.txt", "w");
		double		dx = (b - a) / (TESTBASE - 1);
		for( j= 0; j < nb; j++ )
			xfi( a, b, nb, j, tst );
		for (i = 0; i < TESTBASE; i++) {
			fprintf(tst, "%g", a + i * dx);
			for (j = 0; j < nb; j++) {
				fprintf(tst, " %g", fi  (a, b, nb, j, a + i * dx));
				fprintf(tst, " %g", dfi (a, b, nb, j, a + i * dx));
				fprintf(tst, " %g", d2fi(a, b, nb, j, a + i * dx));
				fprintf(tst, " %g", d3fi(a, b, nb, j, a + i * dx));
			}
			fprintf(tst, "\n");
		}
		fclose(tst);
	}
#endif

	for (j = 0; j < nb; j++) {
		for (i = 0; i < nb; i++)
			for (k = 0; k < pts->n; k++)
				add_to_entry_matrix(eqs, j, i,j == i ? exp(-2 * x[k]) * fi(a, b, nb, i, x[k]) * fi(a, b, nb, j, x[k]) : 0);

		for (k = 0; k < pts->n; k++)
			add_to_entry_matrix(eqs, j, nb, exp(-2 * x[k]) * y[k] * fi(a, b, nb, j, x[k]));
	}
        write_matrix(eqs, stdout);
#ifdef DEBUG
	write_matrix(eqs, stdout);
#endif

	if (piv_ge_solver(eqs)) {
		spl->n = 0;
		return;
	}
        write_matrix(eqs, stdout);

#ifdef DEBUG
	write_matrix(eqs, stdout);
#endif

	if (alloc_spl(spl, nb) == 0) {
		for (i = 0; i < spl->n; i++) {
			double xx = spl->x[i] = a + i*(b-a)/(spl->n-1);
			xx+= 10.0*DBL_EPSILON;  // zabezpieczenie przed ulokowaniem punktu w poprzednim przedziale
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++) {
				double		ck = get_entry_matrix(eqs, k, nb);
				spl->f[i]  += ck * fi  (a, b, nb, k, xx);
				spl->f1[i] += ck * dfi (a, b, nb, k, xx);
				spl->f2[i] += ck * d2fi(a, b, nb, k, xx);
				spl->f3[i] += ck * d3fi(a, b, nb, k, xx);
			}
		}
	}

#ifdef DEBUG
	{
		FILE           *tst = fopen("debug_spline_plot.txt", "w");
		double		dx = (b - a) / (TESTBASE - 1);
		for (i = 0; i < TESTBASE; i++) {
			double yi= 0;
			double dyi= 0;
			double d2yi= 0;
			double d3yi= 0;
			double xi= a + i * dx;
			for( k= 0; k < nb; k++ ) {
							yi += get_entry_matrix(eqs, k, nb) * fi(a, b, nb, k, xi);
							dyi += get_entry_matrix(eqs, k, nb) * dfi(a, b, nb, k, xi);
							d2yi += get_entry_matrix(eqs, k, nb) * d2fi(a, b, nb, k, xi);
							d3yi += get_entry_matrix(eqs, k, nb) * d3fi(a, b, nb, k, xi);
			}
			fprintf(tst, "%g %g %g %g %g\n", xi, yi, dyi, d2yi, d3yi );
		}
		fclose(tst);
	}
#endif

}
