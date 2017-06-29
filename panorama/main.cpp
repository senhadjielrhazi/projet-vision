#include <win.h>
#include "../common/image.h"
#include "../common/dimage.h"
#include "../common/cimage.h"
#include "../common/fft.h"
#include <fstream.h>
#include "../common/vect.h"
#include "../common/mat.h"
#include "../common/mat.h"
#include "../common/camera.h"
#include "../common/lmeds.h"
#include "../common/pic.h"
#include "../common/inverse.h"
#include "../common/lmeds.h"

pic *H1;
pic *H2;

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: main.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

mat fondam(const camera& C1, const camera& C2){
	vect e2=C2*C1.getC();
	mat E2;
	E2(0,0)=E2(1,1)=E2(2,2)=0;
	E2(1,0)=e2(2); 
	E2(2,0)=-e2(1);
	E2(0,1)=-e2(2);
	E2(1,2)=-e2(0);
	E2(0,2)=e2(1);
	E2(2,1)=e2(0);
	return E2*C2.getP()*C1.getPmoins1();
	
}

vect epipolaire(int a, int b, const mat& fond){
	return fond*vect(a,b,1);
}



int ordonn(int abs, vect dir){
	if (dir(1)!=0)	return (int)(-(dir(2)+abs*dir(0))/dir(1));
	else return 1000;
}

double correl(int a, int b, int abs, int ord, int taille, const image& I1, const image& I2){
	double moy1 =0;
	for(int i=-taille; i<=taille; i++){
		for(int j=-taille; j<=taille; j++){
			moy1=moy1+I1(a+i,b+j);
		}
	}
	moy1=moy1/(2*taille+1)*(2*taille+1);
	
	
	double moy2 = 0;
	
	for(i=-taille; i<=taille; i++){
		for(int j=-taille; j<=taille; j++){
			moy2=moy2+I2(abs+i,ord+j);
		}
	}
	moy2=moy2/(2*taille+1)*(2*taille+1);
	
	
	double cor = 0;
	for(i=-taille; i<=taille; i++){
		for(int j=-taille; j<=taille; j++){
			cor= cor + (I1(a+i,b+j)-moy1)*(I2(abs+i,ord+j)-moy2);
		}
	}
	cor=cor/(2*taille+1)*(2*taille+1);
	return cor;
}

double corr_norm(int a, int b, int abs, int ord, int taille, const image& I1, const image& I2){
	return (correl(a, b, abs, ord, taille, I1, I2)/sqrt(
		correl(a, b, a,  b, taille, I1, I1)*correl( abs, ord, abs, ord, taille, I2, I2)));
}

pic Homog(pic p1,const double *H){
	pic p2;
	p2.abs=(H[0]*p1.abs+H[1]*p1.ord+1)/(H[2]*p1.abs+H[3]*p1.ord+1);
	p2.ord=(H[4]*p1.abs+H[5]*p1.ord+1)/(H[6]*p1.abs+H[7]*p1.ord+1);
	return p2;
}

void residual(const double *H, /* I: parameter vector */
			   const int nb, /* I: number of residuals to be computed */
			   const int *indexes, /* I: indexes of data to be used */
			   double *resid /* O: computed residuals */
	      )		/* I: function which calculates the residuals */
{
	int i;
	double x1,x2,y1,y2;
	pic temp;
	for(int j=0;j<nb;j++){
		i=indexes[j];
		x1=H1[i].abs;
		y1=H1[i].ord;
		temp=Homog(H2[i],H);
		x2=temp.abs;
		y2=temp.ord;
		resid[j]=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
}


	int estimator(double H[8], /* O: estimated parameter vector */
			   const int nb, /* I: number of data to be used */
			   const int *indexes /* I: indexes of data to be used */
	      )		
{
	if (nb!=4)
		return 0;
	else{
		double A[64];
		double invA[64];
		for (int k=0; k<64;k++){A[k]=0;}

		A[0]=H1[0].abs;
		A[1]=H1[0].ord;
		A[12]=H1[0].abs;
		A[13]=H1[0].ord;
		A[16]=H1[1].abs;
		A[17]=H1[1].ord;
		A[28]=H1[1].abs;
		A[29]=H1[1].ord;
		A[32]=H1[2].abs;
		A[33]=H1[2].ord;
		A[44]=H1[2].abs;
		A[45]=H1[2].ord;
		A[48]=H1[3].abs;
		A[49]=H1[3].ord;
		A[60]=H1[3].abs;
		A[61]=H1[3].ord;
		A[2]=-H1[0].abs*H2[0].abs;
		A[3]=-H1[0].ord*H2[0].abs;
		A[14]=-H1[0].abs*H2[0].ord;
		A[15]=-H1[0].ord*H2[0].ord;
		A[18]=-H1[1].abs*H2[1].abs;
		A[19]=-H1[1].ord*H2[1].abs;
		A[30]=-H1[1].abs*H2[1].ord;
		A[31]=-H1[1].ord*H2[1].ord;
		A[34]=-H1[2].abs*H2[2].abs;
		A[35]=-H1[2].ord*H2[2].abs;
		A[46]=-H1[2].abs*H2[2].ord;
		A[47]=-H1[2].ord*H2[2].ord;
		A[50]=-H1[3].abs*H2[3].abs;
		A[51]=-H1[3].ord*H2[3].abs;
		A[62]=-H1[3].abs*H2[3].ord;
		A[63]=-H1[3].ord*H2[3].ord;

		inverse(A,invA,8);

	
		double B[8] ={(H2[0].abs)-1,(H2[0].ord)-1,
		(H2[1].abs)-1,(H2[1].ord)-1,(H2[2].abs)-1,(H2[2].ord)-1,
		(H2[3].abs)-1,(H2[3].ord)-1};

		for (int i=0;i<8;i++){
			H[i]=0;
			for (int j=0;j<8;j++){
				H[i]=H[i]+invA[i*8+j]*B[j];
			}
		}
	
	return 1;
	}
}
	
int main()
{
	image I("../data/face00.gif");
	
	image J("../data/face01.gif");
	winInit(I.width()+J.width(),I.height());
	camera C1("../data/face00.txt.mat");
	camera C2("../data/face01.txt.mat");
	
			
	I.display(0,0);
	J.display(I.width(),0);
	
	
	dimage D(I);
	int x,y;
	while (winInput(x,y)==1) {
		vect droit=epipolaire(x,y,fondam(C1,C2));
		int y1=ordonn(1,droit);
		int y2=ordonn(511,droit);
		cout<<' '<<y1<<' '<<y2<<endl;
		winLine(513,y1,1023,y2,blue,1);
		
		//Boucle finale
		
		int taille = 5;
		double cormax=-1;
		int afin=taille, bfin=taille;
		double corloc;
		int ord;
		
		
		
		for(int abs=taille; abs<=512-taille; abs++){
			
			ord=ordonn(abs, droit);
			corloc = corr_norm(x, y, abs, ord, taille, I, J);
			
			if (corloc>cormax){
				cormax=corloc;
				afin=abs;
				bfin=ord;
			}
		}
		
		winLine(afin+512,bfin-4,afin+512,bfin+4,red,1);
	}
	
	double H[8];
	int* outliers=new int[12];
	int nbOutliers;
	pvector har1,har2;
	double taille=10;
	double seuil=5;
	double alpha=0.01;
	((image) I).Harris2(taille,seuil,alpha,har1);
	J.Harris2(taille,seuil,alpha,har2);
	H1=new pic[har1.size()];
	H2=new pic[har2.size()];
	for(int i=0;i<har1.size();i++){
		H1[i]=har1[i];
	}
	for(int j=0;j<har2.size();j++){
		H2[i]=har2[i];
	}
	int ret=miniLMedS(12,	/* I: the number of Data, which is ordered from 0 */
						4,	/* I: size of each randomly selected subset of data */
						0,		/* I: randomly selected subsets, can be set to NULL,
									* then a default routine will be used to generate the subsets */
						0,		  /* I: the number of subsets, not used if sets == NULL */
						residual, /* I: function which calculates the residuals */
						estimator,
						1, /* I: set 1 if `residual' computes the squared residuals */
						0,	/* I: verbose mode */
						1,	/* I: maximum number of solutions given by "estimator" */
						3,	/* I: gate value for outlier detection, a value in [2, 4] */
						0,	/* I: if > 0.0, LMedS stops when a subset gives a median
								* residual less than `prematureResidual'.
								* The specified value must be in accordance with `residual' */
						8,	/* I: dimension of the parameter vector */
						.5, /* I: the percentage
								(between epsilon and 1.0) of good data. An appropriate
									choice is often 0.5 */
						H,	/* O: the corresponding estimate of parameters */
						0,		/* O: best subset retained, can be set to NULL */
						outliers,	/* O: the outliers detected by LMedS */
						&nbOutliers	/* O: the number of detected outliers */
    );

	winClick();
	
	winExit();
	return 0;
}
