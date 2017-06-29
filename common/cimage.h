#ifndef _CIMAGE
#define _CIMAGE

#include "complex.h"
class dimage;

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: cimage.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class cimage
{
	complex *g;
	int w,h;
	
	void kill() ;
	void alloc(int _w,int _h);
public:
	cimage();
	cimage(int _w,int _h);
	cimage(const dimage& ima);
	cimage(const cimage& ima);
	~cimage();

	const cimage& operator=(const cimage& I);

	inline int width() const { return w;}
	inline int height() const { return h;}
	
	inline complex operator()(int i,int j) const { return g[i+w*j]; }
	inline complex& operator()(int i,int j) { return g[i+w*j]; }

	void display(int x,int y,bool phase=false) const;

	cimage fft(bool inverse=false) const;
	void logDisplay(int x,int y) const;

};

#endif
