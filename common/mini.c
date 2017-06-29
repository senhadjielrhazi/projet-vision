#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <sys/types.h>
#include <limits.h>
/*#include <unistd.h>*/

#include "lmeds.h"
#include "miniP.h"
#include <time.h>
#ifdef WIN32
#define RANDOM (rand()/(double)(RAND_MAX))
#define SRANDOM(i) srand(i)
#else
#define RANDOM drand48()
#define SRANDOM(i) srand48(i)
#endif
#define TRUE 1
#define FALSE 0

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: mini.c
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
    
 * calculate gradients
 */
void miniGradient(const int m, const int n, const double *fvecc, 
		  const double *fjacc, const int ljc, double *g)
{
    register i, j ;
    double sum ;
    for (j = 0; j < n; j++) {
        sum = 0.0 ;
        for (i = 0; i < m; i++)
            sum += fjacc[j*ljc+i] * fvecc[i] ;
        g[j] = 2.0 * sum ;
    }
}

/*
 * dotprod
 */
double miniDotprod(const double *v1, const double *v2, const int dim)
{
    double sum = 0;
    register int i;
    for (i = 0; i < dim; i++, v1++, v2++)
	sum += *v1 * *v2;
    return sum;
}

/*
 * transpose of a dim1 x sim2 matrix `mIn' to `mOut'
 */
void miniTranspose(double *mIn, double *mOut, int dim1, int dim2)
{
    double    *maux, *mauxbis;
    int        i, j;
    if (mOut == mIn) {
	maux = (double *) malloc (sizeof (double) * dim1 * dim2);
	mauxbis = maux;
	for (i = 0; i < dim2; i++)
	    for (j = 0; j < dim1; j++)
		*maux++ = mIn[dim2 * j + i];
	maux = mauxbis;
	for (i = 0; i < dim1; i++)
	    for (j = 0; j < dim2; j++)
		*mOut++ = *maux++;
	free (mauxbis);
    } else
	for (i = 0; i < dim2; i++)
	    for (j = 0; j < dim1; j++)
		*mOut++ = mIn[dim2 * j + i];
}

/*
 * default print routine if user does not supply one
 */
void miniDefaultPrint(int m, int n, const double *x, const double *fvec, 
		      const double *fjac)
{
    register i;
    if (fjac == NULL) {
	printf ("Solution: ");
	for (i = 0; i < n; i++)
	    printf ("%g ", x[i]);
	printf ("\n Sumsq: %g\n", miniDotprod(fvec, fvec, m));
    }
    else {
	double *g;
	if ((g = (double *) malloc(n * sizeof(double))) == NULL) {
	    fprintf(stderr, "Error: Not enought memory in `miniDefaultPrint'!\n");
	    exit(0);
	}
	miniGradient(m, n, fvec, fjac, m, g);
	printf ("*Solution: ");
	for (i = 0; i < n; i++)
	    printf ("%12.5e ", x[i]);
	printf ("\n Gradient: ");
	for (i = 0; i < n; i++)
	    printf ("%12.5e ", g[i]);
	printf ("\n Sumsq: %g\n", miniDotprod(fvec, fvec, m));
	free(g);
    }
}

/*
  set the flag telling if random has to be initialized with fixed seed. 
  If flag is set to TRUE (default), several calls to miniLMedS will yield the
  same result (otherwise not).
 */
static int staticMiniRepeatableNoise = TRUE;
void miniSetRepeatableRandom (
    const int flag)   /* flag value (TRUE or FALSE) */
{
    staticMiniRepeatableNoise = flag;
}

/*
 * initialize the random sequence using pid as the seed 
 */
static int noiseRandomInitialized = FALSE;
void miniNoiseInit ()
{
    if (staticMiniRepeatableNoise == TRUE)
		SRANDOM(0L);
    else
	if (noiseRandomInitialized == FALSE) {
	    SRANDOM(time(NULL)) ;
	    noiseRandomInitialized = TRUE;
	}
}

/*
 * generate Gaussian noise from uniformly distributed noise in [0,1] 
 * the formula is                                                    
 *        g = mean + sigma * sum_{i=1}^n (u_i - 1/2) * sqrt(12/n)    
 * when n = 48, the formula is quite simple                          
 */
double
miniNoiseGauss (double mean, double sigma)
{
    double s ;
    register int i ;

    for (i = 0, s = 0; i < 48; i++)
        s += RANDOM ;

    return (mean + sigma * (s - 24.0) * 0.5) ;
}

/*
 * generate uniform noise 
 */
double
miniNoiseUniform (double low, double high)
{
    return low + RANDOM * (high - low);
}

/* 
 * calculate covariance matrix from an upper triangular matrix R
 */
void
miniCovFromR(const int n,	/* I: number of parameters, or dimension of matrix */
	     const double *R,	/* I: upper triangular matrix R */
	     const int m,	/* I: number of observations or data */
	     const double S,	/* I: sum of squared residuals */
	     double *cov)	/* O: the covariance matrix */
{
    double *tmp, *d, s;
    int i,j,k;

    /* allocation */
    if ((tmp=(double *)malloc(n * n * sizeof(double))) == NULL) {
	(void)fprintf(stderr,
		      "miniCovFromR: Not enough memory!\n");
	exit(1);
    }

    /* inverse of R */
    for (i = 0; i < n; i++) {
	*(tmp + i * n + i) = 1 / *(R + i * n + i);
	for (j = i + 1; j < n; j++) {
	    s = 0;
	    for (k = i; k < j; k++) s -= *(R + i * n + k) * *(R + k * n + j);
	    *(tmp + i * n + j) = s / *(R + j * n + j);
	}
    }
    /* calculate R^-1 R^-T */
    d = cov;
    for (i = 0; i < n; i++)
	for (j = 0; j < n; j++, d++) {
	    *d = 0;
	    for (k = 0; k < n; k++)
		*d += *(tmp + i * n + k) * *(tmp + j * n + k);
    }
    /* calculate S / (m - n) R^-1 R^-T */
    d = cov;
    s = S / (double)(m - n);
    for (i = 0; i < n; i++)
	for (j = 0; j < n; j++, d++) {
	    *d *= s;
	}

    /* free */
    free (tmp);
}
