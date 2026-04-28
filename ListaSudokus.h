#pragma once
#include "ReglasSudoku.h"

class tListaSudokus {
private:
	int cont;
	tReglasSudoku** sudokus;
	int dim;

	void expandir(int dimension);


public:
	tListaSudokus();
	~tListaSudokus();
	tReglasSudoku& operator[](int indice);
	int dame_num_elems() const;
	const tReglasSudoku& dame_sudoku(int i) const;
	void insertar(const tReglasSudoku& sudoku);
	void eliminar(int pos);												// elimina el elemento de la posición pos											// no es una funcion constante porque ordena la lista al inicio
};
