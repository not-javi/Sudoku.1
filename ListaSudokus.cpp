#include <iostream>
#include "ListaSudokus.h"

using namespace std;

tListaSudokus::tListaSudokus() {
	cont = 0;
	dim = 0;
	sudokus = nullptr;
}

tListaSudokus::~tListaSudokus() {
	for (int i = 0; i < cont; i++) {
		delete sudokus[i];
	}
	delete[] sudokus;
	cont = 0;
	dim = 0;
}

tReglasSudoku& tListaSudokus::operator[](int indice) {
	return *sudokus[indice];
}

int tListaSudokus::dame_num_elems() const {
	return cont;
}

const tReglasSudoku& tListaSudokus::dame_sudoku(int i) const {
	return *sudokus[i];
}

void tListaSudokus::insertar(const tReglasSudoku& sudoku) {
	if (cont == dim) {
		expandir(dim + 10);
	}
	sudokus[cont] = new tReglasSudoku(sudoku);
	cont++;
}

void tListaSudokus::eliminar(int pos) {
	delete sudokus[pos];
	for (int i = pos; i < cont - 1; i++) {
		sudokus[i] = sudokus[i + 1];
	}
	sudokus[cont - 1] = nullptr;
	cont--;
}

void tListaSudokus::expandir(int dimension) {
	tReglasSudoku** aux = new tReglasSudoku * [dimension];
	for (int i = 0; i < dimension; i++) {
		if (i < cont) {
			aux[i] = sudokus[i];
		}
		else {
			aux[i] = nullptr;
		}
	}
	delete[] sudokus;
	sudokus = aux;
	dim = dimension;
}

