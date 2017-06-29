#include "fft.h"
#include <assert.h>

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: fft.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

static int permute(int i,int n)
{
	int j;
	for (j=0;n!=1;n>>=1,i>>=1)
		j=(j<<1)|(i&1);
	return(j);
}

static int intlog2(int x)
{
	int y;

	if (x<=0)
		return(-1);
	
	for (y=0;!(x&1);x>>=1,y++)
		;
	if (x&~1)
		return(-1);
	return(y);
}

static complex root(int i,int n)
{
   double fac;
   fac=2*M_PI*(double)i/n;
   return complex(cos(fac),sin(fac));
}

static void fft(complex *X,int incr,int n,bool inverse,int step)
{
	int i,index;
	complex even,odd,term;
	
	if (n==1)
		return;
	fft(X,incr*2,n/2,inverse,step);
	fft(X+incr*step,incr*2,n/2,inverse,step);
	for (i=0;i<n/2;i++) {
		even=X[2*i*incr*step];
		odd=X[(2*i+1)*incr*step];
		index=permute(i,n/2);
		term=root(inverse?-index:index,n)*odd;
		X[2*i*incr*step]=even+term;
		X[(2*i+1)*incr*step]=even-term;
	}
}


static void replace(complex *X,int n,int step)
{
	int i,j;
	complex z;

	for (i=0;i<n;i++) {
		j=permute(i,n);
		if (i<j) {
			z=X[i*step];
			X[i*step]=X[j*step];
			X[j*step]=z;
		}
	}
}

void Fft(complex *X,int n,bool inverse,int step)
{
	assert(intlog2(n)>=0); // Puissance de deux

	fft(X,1,n,inverse,step);
    replace(X,n,step);
	double s=sqrt(n);
	for (int i=0;i<n;i++)
		X[i*step]=X[i*step]/s;
}
