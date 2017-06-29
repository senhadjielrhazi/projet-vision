#ifndef _FFT_H
#define _FFT_H

#include "complex.h"

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: fft.h
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

void Fft(complex *X,int n,bool inverse=false,int step=1);

#endif
