#ifndef rech
#define rech

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: rec.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

void RecDericheFilter(double *in,         /* image initiale */
						int w,int h,
						int dx,int dy,          /*ordre de derivation*/
						double alpha,
						double* out);       /*image apres derivation*/
#endif
