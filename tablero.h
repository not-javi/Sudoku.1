#pragma once
#include "celda.h"

const int MAX = 20;

class tTablero {
private:
	int dimension;
	tCelda tablero[MAX][MAX];
public:
	tTablero();										 // Pone la dimension del array al tamaño maximo 
	tTablero(int d);								 // Pone la dimension del array al tamaño dado
	int dame_Dim() const;
	tCelda dame_Elem(int f, int c) const;
	void colocar(int f, int c, tCelda celda);
};