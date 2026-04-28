#include <iostream>
#include "tablero.h"

using namespace std;

tTablero::tTablero() : dimension(MAX) {}
tTablero::tTablero(int d) : dimension(d) {}
int tTablero::dame_Dim() const { return dimension; }
tCelda tTablero::dame_Elem(int f, int c) const {
	return tablero[f][c];
}
void tTablero::colocar(int f, int c, tCelda celda) {
	tablero[f][c] = celda;
}