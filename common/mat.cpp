#include "mat.h"
#include "inverse.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: mat.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

mat mat::inverse() const
{
	mat inv;
	::inverse((double*)M,(double*)(inv.M),3);
	return inv;
}

mat mat::operator*(const mat& B) const
{
	mat C;
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++) {
			C.M[i][j]=0;
			for (int k=0;k<3;k++)
				C.M[i][j]+=M[i][k]*B.M[k][j];
		}
	return C;
}