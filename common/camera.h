#ifndef _CAMERA
#define _CAMERA

#include <fstream.h>
#include "vect.h"
#include "mat.h"
//#include "../common/vect.h"
//#include "../common/mat.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: camera.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

class camera{
	mat P,Pmoins1;
	vect ptilde,C;
public:
	camera(char* nom_de_fichier);
inline vect getC() const {
	return C;
}

inline mat getP() const {
	return P;
}

inline mat getPmoins1() const {
	return Pmoins1;
}

vect operator*(const vect&M) const{
	return P*M+ptilde;
}

};

#endif