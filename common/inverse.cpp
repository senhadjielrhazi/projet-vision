#include <iostream.h>
#include <math.h>
#define SWAP(a,b) {double temp=(a);(a)=(b);(b)=temp;}

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: inverse.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

// resoud Ax=B m fois
// en sortie, A vaut invA et b les solutions
static void gaussj(double *a, int n, double *b, int m)
{
	int *indxc,*indxr,*ipiv;
	int i,icol,irow,j,k,l,ll;
	double big,dum,pivinv;
	indxc=new int[n];
	indxr=new int[n];
	ipiv=new int[n];
	for (j=0;j<n;j++) ipiv[j]=-1;
	for (i=0;i<n;i++) {
		big=0.0;
		for (j=0;j<n;j++)
			if (ipiv[j] != 0)
				for (k=0;k<n;k++) {
					if (ipiv[k] == -1) {
						if (fabs(a[j*n+k]) >= big) {
							big=fabs(a[j*n+k]);
							irow=j;
							icol=k;
						}
					} else if (ipiv[k] > 0) {
						cerr << "GAUSSJ: Singular Matrix-1\n";
						return;
					}
				}
		++(ipiv[icol]);
		if (irow != icol) {
			for (l=0;l<n;l++) SWAP(a[irow*n+l],a[icol*n+l])
			for (l=0;l<m;l++) SWAP(b[irow*m+l],b[icol*m+l])
		}
		indxr[i]=irow;
		indxc[i]=icol;
		if (a[icol*n+icol] == 0.0) {
			cerr << "GAUSSJ: Singular Matrix-2\n";
			return;
		}
		pivinv=1.0/a[icol*n+icol];
		a[icol*n+icol]=1.0;
		for (l=0;l<n;l++) a[icol*n+l] *= pivinv;
		for (l=0;l<m;l++) b[icol*m+l] *= pivinv;
		for (ll=0;ll<n;ll++)
			if (ll != icol) {
				dum=a[ll*n+icol];
				a[ll*n+icol]=0.0;
				for (l=0;l<n;l++) a[ll*n+l] -= a[icol*n+l]*dum;
				for (l=0;l<m;l++) b[ll*m+l] -= b[icol*m+l]*dum;
			}
	}
	for (l=n-1;l>=0;l--) {
		if (indxr[l] != indxc[l])
			for (k=0;k<n;k++)
				SWAP(a[k*n+indxr[l]],a[k*n+indxc[l]]);
	}
	delete[]ipiv;
	delete[]indxr;
	delete[]indxc;
}

void inverse(double *A,double *invA,int n)
{
	for (int i=0;i<n*n;i++)
		invA[i]=A[i];
	gaussj(invA,n,NULL,0);
}

