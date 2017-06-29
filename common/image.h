#ifndef _IMAGE
#define _IMAGE

#include "C:\WinLib\win.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: image.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class dimage;

class image
{
	byte *g;
	int w,h;

	void kill();
	void alloc(int _w,int _h);

public:
	image();
	image(int _w,int _h);	
	image(const char* name);
	image(const image& I);
	image(const dimage& I,bool rescale=false);
	~image();

	const image& operator=(const image& I);

	inline int width() const { return w;}
	inline int height() const { return h;}
	inline byte operator()(int i,int j) const { return g[i+w*j]; }
	inline byte& operator()(int i,int j) { return g[i+w*j]; }

	void display(int x,int y) const;
};

#endif
