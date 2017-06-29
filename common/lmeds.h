#ifndef _mini_h
#define _mini_h

#ifdef __cplusplus
extern "C" {
#endif

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: lmeds.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

extern void
miniSetRepeatableRandom (
    const int flag);  /* flag value (TRUE or FALSE) */

extern void
miniCovFromR(const int n,	/* I: number of parameters, or dimension of matrix */
	     const double *R,	/* I: upper triangular matrix R */
	     const int m,	/* I: number of observations or data */
	     const double S,	/* I: sum of squared residuals */
	     double *cov);	/* O: the covariance matrix */


/*
 * number of tries, i.e. m, according to
 * 1 - (1 - (1-e)^p)^m > P
 */
extern int
miniLMedSNumbTries(const int p,	/* I: size of a subsample */
		   const double e, /* I: expected fraction of outliers */
		   const double P /* I: probability to have a good subsample */
    );


/*
 * allocate necessary subsets for LMedS
 */
extern int **
miniLMedSAllocSets(int sizeSet, /* I: size of a subset */
		   int nbSets	/* I: number of subsets */
    );

/*
 * free allocated sets
 */
extern void
miniLMedSFreeSets(int **sets);

/* 
 * Least median squares technique
 *
 * return 1 if success, 0 if fail
 */
extern int
miniLMedS(const int nbData,	/* I: the number of Data, which is ordered from 0 */
	  const int sizeSet,	/* I: size of each randomly selected subset of data */
	  int **sets,		/* I: randomly selected subsets, can be set to NULL,
				* then a default routine will be used to generate the subsets */
	  int nbSets,		/* I: the number of subsets, not used if sets == NULL */
	  void (*residual)(const double *x, /* I: parameter vector */
			   const int nb, /* I: number of residuals to be computed */
			   const int *indexes, /* I: indexes of data to be used */
			   double *resid /* O: computed residuals */
	      ),		/* I: function which calculates the residuals */
	  int (*estimator)(double *x, /* O: estimated parameter vector */
			   const int nb, /* I: number of data to be used */
			   const int *indexes /* I: indexes of data to be used */
	      ),		/* I: function which estimate the parameters
				 * returns the number of solutions */
	  const int isResidualSqr, /* I: set 1 if `residual' computes the squared residuals */
	  const int verbose,	/* I: verbose mode */
	  const int maxNbSol,	/* I: maximum number of solutions given by "estimator" */
	  const double gate,	/* I: gate value for outlier detection, a value in [2, 4] */
	  const double prematureResidual,
				/* I: if > 0.0, LMedS stops when a subset gives a median
				 * residual less than `prematureResidual'.
				 * The specified value must be in accordance with `residual' */
	  const int dim,	/* I: dimension of the parameter vector */
	  const double percentageOfGoodData, /* I: the percentage
	  (between epsilon and 1.0) of good data. An appropriate
	  choice is often 0.5 */
	  double *estimate,	/* O: the corresponding estimate of parameters */
	  int *bestSet,		/* O: best subset retained, can be set to NULL */
	  int *outliers,	/* O: the outliers detected by LMedS */
	  int *nbOutliers	/* O: the number of detected outliers */
    );

#ifdef __cplusplus
}
#endif

#endif /* !_mini_h */
