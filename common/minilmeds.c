#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <malloc.h>
#include <memory.h>
#include "lmeds.h"
#include "miniP.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: minilmeds.c
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

#define PROBABILITY_CLOSE_TO_ONE 0.991       
/* seems to work in most cases. It is only used if the number of sets
is not given by the user. */
#define EXPECTED_PROPORTION_OF_OUTLIERS 0.4  
/* 40%, that is */
#define MINIMUM_NUMBER_OF_CONFIGURATIONS 6
/* If we have less than the number of combinations is less than 6
   times the number of sets needed to reach the probability of
   PROBABILITY_CLOSE_TO_ONE, then generate all possibilities */

/* this array of precomputed values corresponds to the inverse
   cumulative function for a normal distribution. For more information
   consult the litterature (Robust Regression for Outlier Detection,
   rouseeuw-leroy) or the Maple routine stats/statevalf[icdf,normald] 
   The values are computed for each 5% */
static double ICDF[21] = {1.4e16, 15.94723940, 7.957896558, 5.287692054, 
			  3.947153876, 3.138344200, 2.595242369, 2.203797543, 
			  1.906939402, 1.672911853, 1.482602218, 1.323775627, 
			  1.188182950, 1.069988721, 0.9648473415, 0.8693011162, 
			  0.7803041458, 0.6946704675, 0.6079568319,0.5102134568,
			  0.3236002672};
/*
 * compare two integers
 */

static int iCompare(const void *i, const void *j)
{
    return(*(int*)i - *(int*)j);
}

/*
 * compare two doubles
 */
static int dCompare(const void *i, const void *j)
{
    double diff ;
    diff = *(double*)i - *(double*)j ;
    if (diff < 0.0) return -1 ;
    else if (diff == 0.0) return 0 ;
    else return 1 ;
}

/*
 * generate one subset
 */
static void 
miniLMedSOneSubset (const double l,
	      const double h,
	      const int sizeSet,
	      int subset[]
    )
{
    int i, j, in ;

    for (i = 0; i < sizeSet; ) {
	subset[i] = (int)miniNoiseUniform (l, h) ;
	in = 0 ;
	for (j = 0; j < i && in == 0; j++) 
	    if (subset[i] == subset[j]) in = 1 ;
	if (in == 0) i++ ;
    }
}

/*
 * generate random subsets
 *
 * returns the number of random sets really generated
 */
static int
miniLMedSRandomSets (const int sizeSet,
	       const int nbData,
	       const int nbSets,
	       int **sets
    )
{
    int in, out ;
    register int i, j, k ;
    double l, h ;
    int tries;

    miniNoiseInit () ;

    l = 0.0 ;
    h = nbData - 0.5 ;
    tries = 0;
    for (i = 0; i < nbSets; ) {
	miniLMedSOneSubset (l, h, sizeSet, sets[i]) ;
	qsort((char *)sets[i], sizeSet, sizeof(int), iCompare) ;
	/* verify whether the subset is already retained in sets */
	in = 0 ;
	for (j = 0; j < i && in == 0; j++) {
	    out = 0 ;
	    for (k = 0; k < sizeSet && out == 0; k++)
		if (sets[i][k] != sets[j][k]) 
		    out = 1 ;
	    if (out == 0) in = 1 ;
	}
	if (in == 0) i++ ;

        /* premature exit, too many tries failed */
	if (++tries > 3*nbSets/2) {
	    /*fprintf (stderr,"Warning: premature exit, too many tries failed !\n");*/
            break ;
	}
    }

    return i;
}

/*
 * generate combinations recursively
 */
static void miniGenCombRec(const int sizeSet,
		      const int ns[],
		      int is[],
		      int depth,
		      int *nbComb,
		      int **sets
    )
{
    if (depth == sizeSet - 1) {
	register int i;
	is[depth] = (depth == 0) ? 0 : is[depth-1] + 1;
	for (; is[depth] < ns[depth]; (is[depth])++) {
	    for (i = 0; i < sizeSet; i++)
		sets[*nbComb][i] = is[i];
	    (*nbComb)++;
	}
    }
    else {
	is[depth] = (depth == 0) ? 0 : is[depth-1] + 1;
	for (; is[depth] < ns[depth]; (is[depth])++)
	    miniGenCombRec(sizeSet, ns, is, depth+1, nbComb, sets);
    }
}
    
/*
 * generate all combinations,
 * returns the number of combinations
 */
static int
miniLMedSCombinations (const int maxInt, /* maximum integer */
		 const int sizeSet, /* size of a combination */
		 int **sets	/* Output: combinations generated */
    )
{
    int *is, *ns, nbComb;
    int i;

    if ((is = (int *)malloc(sizeSet*sizeof(int))) == NULL ||
	(ns = (int *)malloc(sizeSet*sizeof(int))) == NULL) {
	fprintf(stderr, "Error: Not enough memory in `miniLMedSCombinations'!\n");
	exit (1);
    }
    for (i = 0; i < sizeSet; i++) 
	ns[i] = maxInt - sizeSet + i + 1;

    nbComb = 0;
    miniGenCombRec(sizeSet, ns, is, 0, &nbComb, sets);

    /* free memory */
    free(is);
    free(ns);

    return nbComb;
}

/*
 * number of combination
 */
static int
numbComb(int n, int m)
{
  double nb;
  int i, nbInt;
  if (m < 0 || m > n) {
    fprintf(stderr, "Error in argument m of `numbComb'!\n");
    return (0);
  }
  nb = 1.0;
  for (i = 0; i < m; i++, n--)
    nb *= n;
  while (m > 1)
    nb /= m--;
  nbInt = (nb>INT_MAX)?(INT_MAX):((int)(nb));
  return nbInt;
}

/*
 * number of tries, i.e. m, according to
 * 1 - (1 - (1-e)^p)^m > P
 */
int
miniLMedSNumbTries(const int p,	/* I: size of a subsample */
		   const double e, /* I: expected fraction of outliers */
		   const double P /* I: probability to have a good subsample */
    )
{
    double E, e1;
    int i;
    if (e < 0.0 || e > 1.0 || p <= 0 || P >= 1.0 || P <= 0.0) {
	fprintf (stderr, "Error: Wrong with arguments of `miniNumbTries'!\n");
	exit (1);
    }
    E = 1.0;
    e1 = 1.0 - e;
    for (i = 0; i < p; i++)
	E *= e1;
    
    return (int) ceil(log(1.0 - P) / log(1.0 - E));
}

/*
 * allocate necessary subsets for LMedS
 */
int **
miniLMedSAllocSets(int sizeSet, /* I: size of a subset */
		   int nbSets	/* I: number of subsets */
    )
{
    int **sets;
    int i;
    if ((sets = (int **)malloc(nbSets * sizeof(int *))) == NULL) {
	fprintf(stderr, "Error: Not enough memory in `miniLMedSAllocSets'!\n");
	exit (1);
    }
    if ((sets[0] = (int *)malloc(sizeSet * nbSets * sizeof(int))) == NULL) {
	fprintf(stderr, "Error: Not enough memory in `miniLMedSAllocSets'!\n");
	exit (1);
    }
    for (i = 1; i < nbSets; i++)
	sets[i] = sets[i-1] + sizeSet;
    return sets;
}

/*
 * free allocated sets
 */
void
miniLMedSFreeSets(int **sets)
{
    if (sets == NULL)
	return;
    free(sets[0]);
    free(sets);
}

/* 
 * Least median squares technique
 *
 * return 1 if success, 0 if fail
 */
int
miniLMedS(const int nbData,	/* I: the number of Data, which is ordered from 0 */
	  const int sizeSet,	/* I: size of each randomly selected subset of data */
	  int **sets,		/* I: randomly selected subsets, can be set to NULL,
				* then a default routine will be used to generate the subsets */
	  int nbSets,		/* I: the number of subsets, set to 0 if you have no idea */
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
    )
{
    double *sols;
    double *resGood, *resNew, *resTmp;
    double medGood, medNew, sigma;
    int medPos, intTmp;
    int nbSol;
    int *indexes;
    int freeSets = 0;
    int best = -1;
    int i, j, k;

    /* memory allocation */
    if ((sols = (double *)malloc(maxNbSol*dim*sizeof(double))) == NULL ||
	(resGood = (double *)malloc(3 * nbData * sizeof(double))) == NULL ||
	(indexes = (int *)malloc(nbData * sizeof(int))) == NULL) {
	fprintf(stderr, "Error: Not enough memory in `miniLMedS'!\n");
	exit (1);
    }

    resNew = resGood + nbData;
    resTmp = resNew + nbData;
    for (i = 0; i < nbData; i++)
	indexes[i] = i;

    /* if sets == NULL, use the default routine to generate random subsets */
    if (sets == NULL) {
	int allComb;
	freeSets = 1;
	allComb = numbComb(nbData, sizeSet);
	if (nbSets==0)
		/* 20 x plus pour stitch... : RK ... */
	  nbSets = 200*miniLMedSNumbTries(sizeSet, EXPECTED_PROPORTION_OF_OUTLIERS,
				      PROBABILITY_CLOSE_TO_ONE);

	
	if (allComb < MINIMUM_NUMBER_OF_CONFIGURATIONS*nbSets) 
	  { /* generate all combinations */
	    sets = miniLMedSAllocSets(sizeSet, allComb);
	    nbSets = miniLMedSCombinations (nbData, sizeSet, sets);
	  }
	else 
	  {
	    /* randomly generate a few subsets */
	    sets = miniLMedSAllocSets(sizeSet, nbSets);
	    nbSets = miniLMedSRandomSets (sizeSet, nbData, nbSets, sets);
	  }
    }

    /* position of the median. If set to zero (meaningless) set it to
       something standard: 0.5 idem if the number is inconsistent */
    if ((percentageOfGoodData <= 0.0)||(percentageOfGoodData >= 1.0))
      {
	fprintf(stderr,	"Bad argument in MiniLMedS:\n"); 
	fprintf(stderr,	"percentageOfGoodData must be between 0.0 and 1.0\n");
	exit(1);
      }

    /* do not consider the first sizeSet elements */
    medPos = (int)((nbData+sizeSet)*percentageOfGoodData); 
    if (medPos < sizeSet - 1) medPos = sizeSet - 1;

    for (i = 0; i < nbSets; i++) {
	/* estimate the parameters for this subset */
	nbSol = estimator (sols, sizeSet, sets[i]);
	/* now test the solutions on the whole set of data */
	for (k = 0; k < nbSol; k++) {
	    residual(sols + k*dim, nbData, indexes, resNew);
	    /* we use the absolute residuals */
	    for (j = 0; j < nbData; j++)
		resNew[j] = fabs(resNew[j]);
	    memcpy((char *)resTmp, (char *)resNew, nbData*sizeof(double));
	    /* sort the set of residuals */
	    qsort((char *)resTmp, nbData, sizeof(double), dCompare) ;
	    /* find the median */
	    medNew = resTmp[medPos] ;
	    /* save the result */
	    if (best == -1 || medNew < medGood) {
		medGood = medNew ;
		best = i ;
		memcpy((char *)resGood, (char *)resNew, nbData*sizeof(double));
		memcpy((char *)estimate, (char *)(sols+k*dim), dim*sizeof(double));
	    }
	    if (medGood <= prematureResidual)
		i = nbSets;	/* premature termination */
	}
    }

    if (best == -1) {		/* fail */
      printf ("LMedS failed. Among all trials, no solution is found!\n");
      /* Usually it does not happen, but it does happen to Diane, la pauvre */
      /* free memory */
      free(sols);
      free(resGood);
      if (freeSets)
	miniLMedSFreeSets(sets);
      free(indexes);
      return (0);
    }

    if (verbose) {
	printf ("LMedS result:\n Median = %g\n", medGood) ;
	printf (" Subset %d : ", best) ;
	for (i = 0; i < sizeSet; i++)
	    printf ("%d ", sets[best][i]);
	printf ("\n Estimation: ");
	for (i = 0; i < dim; i++)
	    printf ("%g ", estimate[i]);
	printf ("\n");
    }

    if (bestSet)
	*bestSet = best;

    /* compute the robust standard deviation estimate  */
    sigma = ICDF[(int)(/*rint*/(percentageOfGoodData*20.0))] 
      * (1 + 5.0 / (double)(nbData - sizeSet)) ;
    if (isResidualSqr)
	sigma *= sigma ;
    sigma *= gate * medGood ;
    if (isResidualSqr)
	sigma *= gate;

    /* Now detect the outliers (use sqrt(KSQR) sigma) */
    j = 0 ;
    for (i = 0; i < nbData; i++) {
	if (resGood[i] > sigma) {
	    /* arrange the outliers in the decreasing order of residual */
	    /* a very simple sorting */
	  /* find out where the new outlier should be. they are in
             decreasing order*/
	    for (k = 0; (k < j)&&(resGood[i] > resTmp[k]) ; k++) 
	      ;
	    intTmp = k ;
	    for (k = j; k > intTmp; k--) {
		outliers[k] = outliers[k-1] ;
		resTmp[k] = resTmp[k-1] ;
	    }
	    resTmp[intTmp] = resGood[i] ;
	    outliers[intTmp] = i ;
	    if (verbose)
		printf (" Data %d is outlier !\n", i) ;
	    j++ ;
	}
    }
    *nbOutliers = j;

    /* free memory */
    free(sols);
    free(resGood);
    if (freeSets)
	miniLMedSFreeSets(sets);
    free(indexes);

    return (1);			/* success */
}

