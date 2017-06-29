/* 
 * Private header file for mini (Minimization) library
 * 
 * $Log: miniP.h,v $
 * Revision 1.12  1996/01/11 17:58:54  devernay
 * added error handling to slatec
 *
 * Revision 1.11  1995/12/04 09:10:14  zeller
 * Nested calls to miniNLSMinpackLocal().
 *
 * Revision 1.10  1995/03/14 14:12:01  zzhang
 * Added miniNoiseInit, miniNoiseGauss, miniNoiseUniform
 *
 * Revision 1.9  1995/03/03  17:00:47  devernay
 * added support for minimizing objective functions
 *
 * Revision 1.8  1995/03/02  16:47:03  devernay
 * added another tn.f prototype
 *
 * Revision 1.7  1995/03/02  11:35:30  devernay
 * added prototype of tn_()
 *
 * Revision 1.6  1995/02/28  17:07:42  laveau
 * Added a few const in the prototypes.
 *
 * Revision 1.5  1995/02/28  12:51:29  devernay
 * mainly typos
 *
 * Revision 1.4  1995/02/28  12:23:34  devernay
 * bug fixes again
 *
 * Revision 1.3  1995/02/28  12:12:57  zzhang
 * add miniTranspose
 *
 * Revision 1.2  1995/02/27  16:41:14  devernay
 * changed headers
 *
 */
#ifndef _miniP_h
#define _miniP_h

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: miniP.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

/*
 * calculate gradients
 */
extern void miniGradient(const int m, const int n, const double *fvecc, 
			 const double *fjacc, const int ljc, double *g);
/*
 * dotprod
 */
extern double miniDotprod(const double *v1, const double *v2, const int dim);
/*
 * transpose of a dim1 x sim2 matrix `mIn' to `mOut'
 */
extern void miniTranspose(double *mIn, double *mOut, int dim1, int dim2);
/*
 * default print routine if user does not supply one
 */
extern void miniDefaultPrint(int m, int n, const double *x,
			     const double *fvec, const double *fjac);
/*
 * initialize the random sequence using pid as the seed 
 */
extern void miniNoiseInit ();
/*
 * generate Gaussian noise from uniformly distributed noise in [0,1] 
 * the formula is                                                    
 *        g = mean + sigma * sum_{i=1}^n (u_i - 1/2) * sqrt(12/n)    
 * when n = 48, the formula is quite simple                          
 */
extern double miniNoiseGauss (double mean, double sigma);
/*
 * generate uniform noise 
 */
extern double miniNoiseUniform (double low, double high);

/* Macros for stacks */
#define MINI_STACK_CARDINAL_MAX 10
#define MINI_STACK_PUSH(element, stack, stack_cardinal)\
{\
     *(stack + stack_cardinal) = (element);\
     (stack_cardinal)++;\
}
#define MINI_STACK_PULL(element, stack, stack_cardinal)\
{\
     (stack_cardinal)--;\
     (element) = *(stack + stack_cardinal);\
}

#endif /* !_miniP_h */
