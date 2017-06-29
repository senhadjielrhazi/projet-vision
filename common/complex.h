#ifndef _COMPLEX_H
#define _COMPLEX_H

#include <math.h>

// Inexistants avec le vieux Visual 6: Pi et les complexes
#ifndef M_PI
#define M_PI 3.14159265359
#endif

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: complex.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

#include <iostream.h>
struct complex {
	double a,b;
	complex() {}
	complex(double A,double B=0) { a=A; b=B; }
	complex operator+(const complex& z) const { return complex(a+z.a,b+z.b); }
	complex operator-(const complex& z) const { return complex(a-z.a,b-z.b); }
	complex operator*(const complex& z) const { return complex(a*z.a-b*z.b,a*z.b+b*z.a); }
	complex operator/(double l) const { return complex(a/l,b/l); }
	friend ostream& operator<<(ostream &f,const complex& z) {
		return f<<z.a<<(z.b<0?" ":" +")<<z.b<<" i";
	}
	double module() const { return sqrt(a*a+b*b); }
	double phase() const { 
		if (a==0)
			return (b>0)?M_PI/2:-M_PI/2;
		return (a>0)?atan(b/a):atan(b/a)+M_PI;
	}
};

// -----------

#endif
