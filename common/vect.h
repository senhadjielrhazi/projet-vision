#ifndef _vecth
#define _vecth
#include <iostream.h>

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: vect.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class vect
{
	double x[3];
public:
	inline vect() {}
	inline vect(double a,double b,double c) { x[0]=a; x[1]=b; x[2]=c; }
	inline ~vect() {}
	
	inline double operator()(int i)const { return x[i]; }
	inline double& operator()(int i) { return x[i]; }

	friend ostream& operator<<(ostream& f, const vect& v)
	{
	  return f << v.x[0] << ',' << v.x[1] << ',' << v.x[2];
	}
	friend istream& operator>>(istream& f, vect& v)
	{
	  return f >> v.x[0] >> v.x[1] >> v.x[2];
	}

	inline double operator*(const vect&v) const {
		return x[0]*v.x[0]+x[1]*v.x[1]+x[2]*v.x[2];
	}
	inline vect operator*(double l) const { 
		return vect(x[0]*l,x[1]*l,x[2]*l); 
	}
	inline vect operator+(const vect& v) const { 
		return vect(x[0]+v.x[0],x[1]+v.x[1],x[2]+v.x[2]);
	}
};
#endif
