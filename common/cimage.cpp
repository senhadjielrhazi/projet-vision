#include "cimage.h"
#include "dimage.h"
#include "image.h"
#include "fft.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: cimage.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

cimage::cimage()
{
	g=0;
}

void cimage::alloc(int _w,int _h)
{
	w=_w;h=_h;
	g=new complex[w*h];
}

cimage::cimage(int _w,int _h)
{
	alloc(_w,_h);
}

cimage::cimage(const dimage& ima)
{
	alloc(ima.width(),ima.height());
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=complex(ima(i,j),0);
}

cimage::cimage(const cimage& ima)
{
	alloc(ima.w,ima.h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=ima(i,j);
}

void cimage::kill()
{
	if (g) {
		delete[] g;
		g=0;
	}
}

cimage::~cimage(void)
{
	kill();
}

const cimage& cimage::operator=(const cimage& ima)
{
	kill();
	alloc(ima.w,ima.h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=ima(i,j);
	return *this;
}

void cimage::display(int x,int y,bool phase) const
{
	dimage D(*this,phase);
	D.display(x,y);
}

void cimage::logDisplay(int x,int y) const
{
	dimage D(w,h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			D((i+w/2)%w,(j+h/2)%h)=log((*this)(i,j).module());
	D.display(x,y);
}

cimage cimage::fft(bool inverse) const
{
	cimage F(*this);
	// Colonnes
	for (int i=0;i<w;i++)
		Fft(F.g+i,h,inverse,w);
	// Lignes
	for (int j=0;j<h;j++)
		Fft(F.g+j*w,w,inverse,1);
	return F;
}
