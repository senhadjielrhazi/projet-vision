#include "rec.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: rec.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

static void recursiveFilter2(double *input,
			       void *output,
			       int len,int inc,int incOut,
			       double *yp,double *ym,
			       double *np,double *nm,double *d,
			       double kNorm, /* normalization coeff */
			       int type) /* 0: odd filter, 1: even */
{
  register int      k;
  register double    d0,d1,n0,n1,in0,in1;
  register double    *inp2,*inp1;
  register double    out0,out1,out,*outp;
  register double    *inp;
  register double *outputFloat;

  
  /*    Initialization of the recursive constants.  */
  d0 = (double)*d++;
  d1 = (double)*d;
  
  /*    Initialization of filter's constants.   */
  n0 = (double)*np++;
  n1 = (double)*np;
  
  /*    Side initialisation.        */
  inp  = input;
  in1  = (double)*inp;
  inp += inc;
  in0  = (double)*inp;
  inp += inc; 
  /*
   *    Normally here we have:
   *
   *          in0  = input[inc]; (in0=input[k])
   *          in1  = input[0];   (in1=input[k-1])
   *         out0  = output[0];  (out0=output[k-2])
   *         out1  = output[inc]; (out1=output[k-1])
   *
   */
  
  outp = yp;
  out0 = *outp++ = n0*in1;
  out1 = *outp++ = n0*in0 + n1*in1 - d0*out0;
  
  /*    Computation of the causal part of the filter.   */
  for (k=2;k<len;k++) {
	in1  = in0;
	in0  = (double)*inp;
	out  = *outp++ = n0*in0 + n1*in1 - d0*out1 - d1*out0;
	out0 = out1;
	out1 = out;
	inp += inc;
  }
  
  /*    Computation of the anti-causal part of the filter.  */
  
  /*    Initialisation of filter's constants.   */
  n0 = (double)*nm++;
  n1 = (double)*nm;
  
  /*    Side initialisation.        */
  
  /*
   *    Normally here we have:
   *
   *          in0  = input[len(input)-inc]; (input[k+2])
   *          in1  = input[len(input)-2*inc];(input[k+1])
   *          out1 = output[k+2]
   *          out0 = output[k+1]
   *    So Only inp needs to be initialized.
   *
   */
  
  inp  = inp-3*inc;
  outp = &ym[len-1];
  out1 = *outp-- = 0;
  out0 = *outp-- = n0*in0;
  
  /*    Computation of the recursive part of the filter.        */
  
  for (k=len-3;k>=0;k--) {
	out  = *outp-- = n0*in1 + n1*in0 - d0*out0 - d1*out1;
	in0  = in1;
	in1  = (double)*inp;
	out1 = out0;
	out0 = out;
	inp -= inc;
  }

  /*    Compute the result by adding causal and anticausal parts.    */
  
  outputFloat = (double *)output;
  inp2  = yp;
  inp1 = ym;
  if (type==1)
	for(k=0;k<len;k++) {
	  *outputFloat=(double)kNorm*((*inp2++)-(*inp1++));
	  outputFloat += incOut;
	}
  else
	for(k=0;k<len;k++) {
	  *outputFloat=(double)(*inp2++)+(*inp1++);
	  outputFloat += incOut;
	}
  
  
}


  

	  
static void ExpoFilter2(const int ord, /* ordre de derivation */
						double alpha,
			double *np, /* coefficients du filtre */
			double *nm,
			double *d,
			double *kNorm) /* facteur de normalisation */
{
  register double expa,exp2a;
  register double k;
  register double t1,t2,t4,t7;
  
  expa=exp(-alpha);
  exp2a=expa*expa;
  
  d[0]= -2.0*expa;
  d[1]=  exp2a;
  
  switch (ord) {
  case 0:
    k=1-expa;
    k=k*k/(1.0+2.0*alpha*expa-exp2a);
	
    np[0]= k;
    np[1]= k*(alpha-1.0)*expa;
	
    nm[0]=  k*expa*(alpha+1.0);
    nm[1]= -k*exp2a;
    *kNorm=1.0;
    break;
  case 1:
    np[0]= 0.0;
    np[1]= 1.0;
	
    nm[0]= 1.0;
    nm[1]= 0.0;
    t1=exp(alpha);
    t2 = t1+1.0;
    t4 = t1*t1;
    t7 = 1/(t4*t1-3.0*t4+3.0*t1-1.0);
    *kNorm = -expa/(t1*t2*t7+t1/(-1.0+t1)
					+t4*t2*t7-2.0*t4/(1.0-2.0*t1+t4));
    break;
  case 2:
    np[0]=exp(alpha);         /* Constant defined by Deriche */
    np[1]=np[0]*np[0];      /*  in its implementation so that */
    nm[0]=np[0]*np[1];      /* the second derivative of a quadratic */
    nm[1]=(1.-exp2a)/(2.*alpha*expa); /* function is equal to 2. */
    np[0]=2*(nm[0]-3*(np[1]-np[0])-1.)/(nm[0]+3.*(np[1]+np[0])+1.); 
    np[1]= np[0]*(nm[1]*alpha+1.)*expa; 
    nm[0]= np[0]*(-1.+nm[1]*alpha)*expa; 
    nm[1]= np[0]*exp2a; 
    np[0]*= -1.;
    *kNorm=1.0;
    /*   np[0]=(1.-exp2a)/(2.*alpha*expa);*/ /* Constants defined in the     */
    /*   np[1]=-(np[0]*alpha+1)*expa; */     /* article so that the response */
    /*   nm[0]= (1.-np[0]*alpha)*expa;   */  /* to a constant signal is null.*/
    /*   nm[1]= -exp2a; */
    /*   np[0]=1.; */
	
    break;
  }
}
#define HORIZ 0
#define VERT 1
void RecFilter1(double *in,/* image initiale */
			   int w,int h,
			   int order,/*ordre de derivation*/
			   int dir,/*direction HORIZ ou VERT*/
			   double alpha,
			   double* out)/*image apres derivation*/
{
  void *yp,*ym;   /* Workspaces */
  double np[2],nm[2],d[2]; /* coefficients du filtre */
  double kNorm;            /* normalisation du filtre */
  int dim,n;
  int k;
  int localLen,localIncIn,localIncOut;
  int smoothLen,smoothIncIn,smoothIncOut;
  int type=order%2;
  
  switch (dir) {
  case HORIZ:
    dim=w;
    localLen=h;
    localIncIn=w;
    localIncOut=w;
    smoothLen=dim;
    smoothIncIn=1;
    smoothIncOut=1;
    break;
  case VERT:
    dim=h;
    localLen=w;
    localIncIn=1;
    localIncOut=1;
    smoothLen=dim;
    smoothIncIn=w;
    smoothIncOut=w;
    break;
  }

  /* Allocation des buffer */
  
  n=dim*sizeof(double);
  if ((yp = (double *)malloc(2*n))==NULL) {
	fprintf(stderr,"Pb of memory allocation in recFilter\n");
	exit(1);
  }
  ym=&((double *)yp)[dim];

  /* Derivation et filtrage */
  if (alpha<=0)
	alpha=.1;
  
  ExpoFilter2(order,1/sqrt(alpha),np,nm,d,&kNorm);

  for(k=0; k<localLen; k++,in+=localIncIn) {
	  recursiveFilter2(in,out,
			       smoothLen,smoothIncIn,smoothIncOut,
			       (double*)yp,(double*)ym,np,nm,d,kNorm,type);
	  out+=localIncOut;
  }
  free(yp);
}


void RecDericheFilter(double *in,         /* image initiale */
						int w,int h,
						int dx,int dy,          /*ordre de derivation*/
						double alpha,
						double* out)       /*image apres derivation*/
{
	int off=10+3*int(alpha); // offset
	int nw=w+2*off;
	int nh=h+2*off;

	double *tmp=(double*)malloc(sizeof(double)*nw*nh);
	double *tin=(double*)malloc(sizeof(double)*nw*nh);
	double *tout=(double*)malloc(sizeof(double)*nw*nh);

	int i,j;
	for (i=0;i<nw*nh;i++)
		tin[i]=128;
	

	for (i=0;i<w;i++)
		for (j=0;j<h;j++)
			tin[(off+i)+nw*(off+j)]=in[i+w*j];
	for (j=off;j<h+off;j++) {
		for (i=0;i<off;i++)
			tin[i+nw*j]=tin[2*off-i+nw*j];
		for (i=w+off;i<w+2*off;i++)
			tin[i+nw*j]=tin[2*off+2*w-2-i+nw*j];
	}
	for (i=0;i<nw;i++) {
		for (j=0;j<off;j++)
			tin[i+nw*j]=tin[i+(2*off-j)*nw];
		for (j=h+off;j<h+2*off;j++)
			tin[i+nw*j]=tin[i+(2*off+2*h-2-j)*nw];
	}

	RecFilter1(tin,nw,nh,dx,HORIZ,alpha,tmp);
	RecFilter1(tmp,nw,nh,dy,VERT,alpha,tout);
	
	for (i=0;i<w;i++)
		for (j=0;j<h;j++)
			out[i+w*j]=tout[(i+off)+nw*(j+off)];

	free(tmp);
	free(tin);
	free(tout);
}
