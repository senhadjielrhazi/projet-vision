#include "camera.h"
#include "../common/mat.h"
#include <fstream.h>

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: camera.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

camera::camera(char* nom_de_fichier){
	
	ifstream f(nom_de_fichier);
	vect l;	

	for(int i=0;i<3;i++){
	f>>l>>ptilde(i);
	;
	P.setline(i,l);
	}
	f.close();
	Pmoins1 = P.inverse();
	C=(Pmoins1*ptilde)*-1;

}

