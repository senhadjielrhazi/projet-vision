#ifndef _mat
#define _mat
#include "vect.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: mat.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class mat
{
	double M[3][3];
public:
	mat() {}
	~mat() {}

	inline double operator()(int i,int j)const { return M[i][j]; }
	inline double& operator()(int i,int j) { return M[i][j]; }

	inline vect line(int i) const { return vect(M[i][0],M[i][1],M[i][2]); }
	inline void setline(int i,const vect& v) { M[i][0]=v(0);M[i][1]=v(1);M[i][2]=v(2); }

	friend ostream& operator<<(ostream& f, const mat& A)
	{
	  return f << A.M[0][0] << ',' << A.M[0][1] << ',' << A.M[0][2] << '\n'
		       << A.M[1][0] << ',' << A.M[1][1] << ',' << A.M[1][2] << '\n'
			   << A.M[2][0] << ',' << A.M[2][1] << ',' << A.M[2][2] ;
	}

	inline vect operator*(const vect& v) const {
		return vect(line(0)*v,line(1)*v,line(2)*v);
	}

	mat inverse() const;
	mat operator*(const mat& B) const;
};
#endif
