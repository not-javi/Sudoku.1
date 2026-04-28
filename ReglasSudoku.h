#pragma once
#include <fstream>
#include "tablero.h"
using namespace std;

const int MAXCELDAS = MAX * MAX;
const int MAX_VALORES = 16;


typedef int arrValores[MAX_VALORES];


typedef struct {
	bool posible;
	int celdas_que_afectan;
} tValor;


typedef struct {
	int nFilas;
	int nColumnas;
	tValor valores[MAX][MAX][MAX];
} tValores;


typedef struct {
	int f;
	int c;
} tCoordenada;


class tReglasSudoku {
private:
	tTablero tablero;
	int contador_ocup;
	int contador_bloq;
	tCoordenada* bloq[MAXCELDAS];
	tValores valores_celda;
	arrValores cuantas_celdas;

	bool v_valido(int v) const;												// revisa si v es un valor permitido para el sudoku
	bool coord_valida(int f, int c) const;									// revisa si una la coordenada es valida para el sudoku
	int unico_valor(int f, int c) const;									// funcion auxiliar que devuelve el unico  valor valido de una celda
	bool set_bloq(int f, int c);											// bloquea una celda
	void quitar_celda_bloqueada(int f, int c);								// saca una celda del array bloqueado cuando deja de estar bloqueada
	int unica_celda_Vpos(int f, int c) const;								// devuelve v si es la unica casilla en su matriz con valor posible v (AUXILAR PARA AUTOCOMPLETAR)
	int unica_celda_fila(int f, int c) const;
	int unica_celda_columna(int f, int c) const;
	void inicializar_valores_celda(int d);
	void rellena_arrValores();

public:
	tReglasSudoku(const tReglasSudoku& sudoku);
	tReglasSudoku();
	~tReglasSudoku();
	tReglasSudoku& operator =(const tReglasSudoku& sudoku);
	int dame_dimension() const;												// devuelve la dimensión del tablero
	tCelda dame_celda(int f, int c) const;									// devuelve la celda en la posición (f,c)
	bool terminado() const;													// true si y sólo si el Sudoku está resuelto
	bool bloqueo() const;													// true si el Sudoku tiene celdas bloqueadas
	int dame_num_celdas_bloqueadas() const;									// devuelve el número de celdas bloqueadas
	int dame_num_celdas_vacias() const;										// devuelve el número de celdas vacías
	void dame_celda_bloqueada(int p, int& f, int& c) const;					// devuelve en (f,c) la celda bloqueada en la posición p
	bool es_valor_posible(int f, int c, int v) const;						// true si y sólo si v se puede colocar en (f,c)
	int posibles_valores(int f, int c) const;								// devuelve el número de posibles valores para (f,c)
	bool pon_valor(int f, int c, int v);									// pone v en (f,c)
	bool quita_valor(int f, int c);											// pone la celda (f,c) a VACIA
	void reset();															// recupera el Sudoku original
	void autocompletar();													// rellena todas las celdas con un único valor posible
	void carga_sudoku(ifstream& archivo);									// carga un Sudoku original de un archivo
	int cuantas_celdas_pueden_tener(int n_valores) const;
	bool operator<(const tReglasSudoku& s2) const;
	bool operator==(const tReglasSudoku& s2) const;
};