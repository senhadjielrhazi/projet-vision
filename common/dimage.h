#ifndef _DIMAGE
#define _DIMAGE

#include "pic.h"

#include <list>
#include <vector>
typedef std::vector<pic> pvector;

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: dimage.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class image;
class cimage;
class dimage
{
	double *g;
	int w,h;
	
	void kill() ;
	void alloc(int _w,int _h);
public:
	dimage();
	dimage(int _w,int _h);
	dimage(const image& ima);
	dimage(const dimage& ima);
	dimage(const cimage& ima,bool phase=false);
	~dimage();

	const dimage& operator=(const dimage& I);

	inline int width() const { return w;}
	inline int height() const { return h;}
	
	inline double operator()(int i,int j) const { return g[i+w*j]; }
	inline double& operator()(int i,int j) { return g[i+w*j]; }
	
	void range(double &m,double &M) const;
	void display(int x,int y,bool rescale=true) const;

	dimage lowFilter(double r0,double alpha) const;
	dimage lowRectFilter(int wf,int hf) const;
	dimage highFilter(double r0,double alpha) const;

	double dimage::diff(int i,int j,int var=1);
	dimage dimage::beta1(double dt);
	image dimage::zeros(dimage* R=0, double seuil=0) const;
	image dimage::contour(double seuil,double lissage);
	void dimage::cercle(int x0,int y0,double r);

	dimage reduce() const;
	dimage reduceFilter() const;
	dimage reduceMean() const;

	dimage enlarge() const;
	dimage enlargeInterpol() const;
	
	dimage convolve(const dimage& K,int u0,int v0) const;
	dimage sharpen(double alpha) const;

	dimage deriche(int dx,int dy,double alpha) const;

	dimage Harris(double alpha) const;
	void Harris2(double taille, double seuil, double alpha, pvector& har) const;

};

#endif
