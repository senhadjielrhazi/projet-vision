//#include <iostream.h>
#ifndef _PIC_H
#define _PIC_H

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: pic.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class pic{
public:
	double abs;
	double ord;
	pic() {}
	pic(double a,double b) { abs=a; ord=b; }
/*
	friend ostream& operator<<(ostream& f, const point& p)
	{
	  return f << p.abs << ',' << p.ord ;
	}
	friend istream& operator>>(istream& f, point& p)
	{
	  return f >> p.abs>>p.ord;
	}
*/
};

#endif