#include "image.h"
#include "dimage.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: image.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

image::image()
{
	g=0;
}

image::image(int _w,int _h)
{
	alloc(_w,_h);
}

image::image(const char* name)
{
	winLoadGreyImage((char*)name,g,w,h);
}

void image::alloc(int _w,int _h)
{
	w=_w;h=_h;
	g=new byte[w*h];
}

void image::kill()
{
	if (g) {
		delete[] g;
		g=0;
	}
}

image::image(const image& I)
{
	alloc(I.w,I.h);
	for (int i=0;i<w*h;i++)
		g[i]=I.g[i];
}


image::image(const dimage& I,bool rescale)
{
	alloc(I.width(),I.height());
	if (!rescale) 
		for (int i=0;i<w;i++)
			for (int j=0;j<h;j++) {
				if (I(i,j)<0)
					g[i+w*j]=0;
				else if (I(i,j)>255)
					g[i+w*j]=255;
				else
					g[i+w*j]=byte(I(i,j));
			}
	else {
		double m,M;
		I.range(m,M);
		if (m==M)
			M++;
		for (int i=0;i<w;i++)
			for (int j=0;j<h;j++) {
				g[i+w*j]=byte(255*(I(i,j)-m)/(M-m));
			}
	}
}

image::~image(void)
{
	kill();
}


const image& image::operator=(const image& I)
{
	kill();
	alloc(I.w,I.h);
	for (int i=0;i<w*h;i++)
		g[i]=I.g[i];
	return *this;	
}

void image::display(int x,int y) const
{
	winGreyImage(g,w,h,x,y);
}

image image::Harris(double alpha) const
{
	image A, B, C(w,h), H(w,h);
	double k = 0.04;

	A=deriche(1,0,0);
	B=deriche(0,1,0);
	
	for (int i=0; i<w; i++)
	{
		for (int j=0; j<h; j++)
		{
			C(i,j)=A(i,j)*B(i,j);
		}
	}
	
	for (i=0; i<(*this).width(); i++)
	{
		for (int j=0; j<(*this).height(); j++)
		{
			A(i,j)=A(i,j)*A(i,j);
		}
	}

	for (i=0; i<(*this).width(); i++)
	{
		for (int j=0; j<(*this).height(); j++)
		{
			B(i,j)=B(i,j)*B(i,j);
		}
	}

    A=A.deriche(0,0,alpha);
	B=B.deriche(0,0,alpha);
	C=C.deriche(0,0,alpha);

	for ( i=0; i<(*this).width(); i++)
	{
		for (int j=0; j<(*this).height(); j++)
		{
			H(i,j)=A(i,j)*B(i,j)-C(i,j)*C(i,j)-k*(A(i,j)+B(i,j));
		}
	}

	return H;
}

void image::Harris2(double taille, double seuil, double alpha, pvector& har) const {
	
	har.clear();

	pic q;

	image res(w,h);
	image temp(w,h);

	temp = Harris(alpha);

	double coul;
	int k,l;
	int b1,b2;
	
	for (int i =0; i<w; i++){
		for (int j =0; j<h; j++){
			coul = temp(i,j);
			if (coul>seuil){
				l=max(0,j-taille); 
				b1=min(h,j+taille);
				while (coul==temp(i,j) && l<=b1){
					k=max(0,i-taille);
					b2=min(w,i+taille);
					while (coul==temp(i,j) && k<=b2){
						coul = max(coul,temp(k,l));
						k++;
					}
					l++;
				}
				if (coul==temp(i,j)){
					har.push_back(pic(i,j));
				}
			}
		}
	}
}
