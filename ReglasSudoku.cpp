#include <iostream>
#include <fstream>
#include <cmath>
#include "ReglasSudoku.h"

using namespace std;

tReglasSudoku::tReglasSudoku() : contador_bloq(0), contador_ocup(0) {}

tReglasSudoku::tReglasSudoku(const tReglasSudoku& sudoku) {
	tablero = sudoku.tablero;
	contador_bloq = sudoku.contador_bloq;
	contador_ocup = sudoku.contador_ocup;
	for (int f = 0; f < tablero.dame_Dim(); f++) {
		for (int c = 0; c < tablero.dame_Dim(); c++) {
			for (int v = 1; v <= tablero.dame_Dim(); v++) {
				valores_celda.valores[f][c][v] = sudoku.valores_celda.valores[f][c][v];
			}
		}
	}
	for (int i = 0; i < sudoku.contador_bloq; i++) {
		bloq[i] = new tCoordenada;
		bloq[i]->f = sudoku.bloq[i]->f;
		bloq[i]->c = sudoku.bloq[i]->c;
	}
}

tReglasSudoku::~tReglasSudoku() {
	for (int i = 0; i < contador_bloq; i++) {
		delete bloq[i];
	}
}

tReglasSudoku& tReglasSudoku::operator =(const tReglasSudoku& sudoku) {

	if (this != &sudoku) {
		for (int i = 0; i < contador_bloq; i++) {
			delete bloq[i];
		}
		tablero = sudoku.tablero;
		contador_bloq = sudoku.contador_bloq;
		contador_ocup = sudoku.contador_ocup;
		for (int f = 0; f < tablero.dame_Dim(); f++) {
			for (int c = 0; c < tablero.dame_Dim(); c++) {
				for (int v = 1; v <= tablero.dame_Dim(); v++) {
					valores_celda.valores[f][c][v] = sudoku.valores_celda.valores[f][c][v];
				}
			}
		}
		for (int i = 0; i < sudoku.contador_bloq; i++) {
			bloq[i] = new tCoordenada;
			bloq[i]->f = sudoku.bloq[i]->f;
			bloq[i]->c = sudoku.bloq[i]->c;
		}
	}
	return *this;
}


int tReglasSudoku::dame_dimension() const { return tablero.dame_Dim(); }

tCelda tReglasSudoku::dame_celda(int f, int c) const { return tablero.dame_Elem(f, c); }

bool tReglasSudoku::terminado() const {
	bool term = false;
	if (contador_ocup == tablero.dame_Dim() * tablero.dame_Dim()) term = true;
	return term;
}

bool tReglasSudoku::set_bloq(int f, int c) {
	bool ok = false;
	bool ya_existe = false;
	for (int i = 0; i < contador_bloq; i++) {
		if ((bloq[i]->f == f) && (bloq[i]->c == c)) ya_existe = true;
	}
	if ((coord_valida(f, c)) && (tablero.dame_Elem(f, c).es_vacia()) && (posibles_valores(f, c) == 0) && (!ya_existe)) {
		bloq[contador_bloq] = new tCoordenada;
		bloq[contador_bloq]->f = f;
		bloq[contador_bloq]->c = c;
		contador_bloq++;
		ok = true;
	}
	return ok;
}

bool tReglasSudoku::bloqueo() const {
	bool bloq = false;
	if (contador_bloq > 0) bloq = true;
	return bloq;
}

int tReglasSudoku::dame_num_celdas_bloqueadas() const { return contador_bloq; }

int tReglasSudoku::dame_num_celdas_vacias() const { return (tablero.dame_Dim() * tablero.dame_Dim()) - contador_ocup; }

void tReglasSudoku::dame_celda_bloqueada(int p, int& f, int& c) const {
	f = bloq[p]->f;
	c = bloq[p]->c;
}

void tReglasSudoku::quitar_celda_bloqueada(int f, int c) {
	if (coord_valida(f, c)) {
		bool encontrada = false;
		int i = 0;
		while ((!encontrada) && (i < contador_bloq)) {
			if ((bloq[i]->f == f) && (bloq[i]->c == c)) encontrada = true;
			if (!encontrada) i++;
		}
		if (encontrada) {
			for (int j = i; j < contador_bloq - 1; j++) {
				bloq[j]->f = bloq[j + 1]->f;
				bloq[j]->c = bloq[j + 1]->c;
			}
			contador_bloq--;
		}
	}
}

bool tReglasSudoku::es_valor_posible(int f, int c, int v) const {
	bool posible = false;
	if ((coord_valida(f, c)) && (tablero.dame_Elem(f, c).es_vacia())) {
		posible = valores_celda.valores[f][c][v].posible;
	}
	return posible;
}

int tReglasSudoku::unico_valor(int f, int c) const {
	int v = -1;
	for (int i = 1; i <= dame_dimension(); i++) {
		if (es_valor_posible(f, c, i)) v = i;
	}
	return v;
}

int tReglasSudoku::posibles_valores(int f, int c) const {
	int cont = 0;
	for (int i = 1; i <= dame_dimension(); i++) {
		if (es_valor_posible(f, c, i)) cont++;
	}
	return cont;
}

bool tReglasSudoku::v_valido(int v) const {
	bool ok = false;
	if ((0 < v) && (v <= dame_dimension())) ok = true;
	return ok;
}

bool tReglasSudoku::coord_valida(int f, int c) const {
	bool ok = false;
	if ((0 <= f) && (0 <= c) && (f < tablero.dame_Dim()) && (c < tablero.dame_Dim())) ok = true;
	return ok;
}

bool tReglasSudoku::pon_valor(int f, int c, int v) {
	bool ok = false;
	if ((coord_valida(f, c)) && (v_valido(v)) && (tablero.dame_Elem(f, c).es_vacia()) && (valores_celda.valores[f][c][v].posible)) {
		int b = contador_bloq;
		int subDim = sqrt(dame_dimension());
		int f1 = f / subDim;
		int c1 = c / subDim;
		ok = true;
		tCelda celda;
		celda.set_ocupada();
		celda.set_valor(v);
		tablero.colocar(f, c, celda);
		contador_ocup++;
		for (int i = 0; i < tablero.dame_Dim(); i++) {
			valores_celda.valores[f][i][v].celdas_que_afectan++;
			if ((tablero.dame_Elem(f, i).es_vacia()) && (valores_celda.valores[f][i][v].posible)) {
				valores_celda.valores[f][i][v].posible = false;
				if (posibles_valores(f, i) == 0) set_bloq(f, i);
			}
		}
		for (int i = 0; i < tablero.dame_Dim(); i++) {
			valores_celda.valores[i][c][v].celdas_que_afectan++;
			if ((tablero.dame_Elem(i, c).es_vacia()) && (valores_celda.valores[i][c][v].posible)) {
				valores_celda.valores[i][c][v].posible = false;
				if (posibles_valores(i, c) == 0) set_bloq(i, c);
			}
		}
		for (int i = c1 * subDim; i < (c1 + 1) * subDim; i++) {
			for (int j = f1 * subDim; j < (f1 + 1) * subDim; j++) {
				valores_celda.valores[j][i][v].celdas_que_afectan++;
				if ((tablero.dame_Elem(j, i).es_vacia()) && (valores_celda.valores[j][i][v].posible)) {
					valores_celda.valores[j][i][v].posible = false;
					if (posibles_valores(j, i) == 0) set_bloq(j, i);
				}
			}
		}
		rellena_arrValores();
	}
	return ok;
}

bool tReglasSudoku::quita_valor(int f, int c) {
	bool ok = false;
	if (tablero.dame_Elem(f, c).es_ocupada() && coord_valida(f, c)) {
		int subDim = sqrt(dame_dimension());
		int f1 = f / subDim;
		int c1 = c / subDim;
		int v = tablero.dame_Elem(f, c).dame_valor();
		tCelda temp;
		temp.set_vacia();
		tablero.colocar(f, c, temp);
		contador_ocup--;
		for (int i = 0; i < tablero.dame_Dim(); i++) {
			if ((tablero.dame_Elem(f, i).es_vacia()) && (valores_celda.valores[f][i][v].celdas_que_afectan > 0)) {
				valores_celda.valores[f][i][v].celdas_que_afectan--;
				if (valores_celda.valores[f][i][v].celdas_que_afectan == 0) {
					valores_celda.valores[f][i][v].posible = true;
					quitar_celda_bloqueada(f, i);
				}
			}
		}
		for (int i = 0; i < tablero.dame_Dim(); i++) {
			if ((tablero.dame_Elem(i, c).es_vacia()) && (valores_celda.valores[i][c][v].celdas_que_afectan > 0)) {
				valores_celda.valores[i][c][v].celdas_que_afectan--;
				if (valores_celda.valores[i][c][v].celdas_que_afectan == 0) {
					valores_celda.valores[i][c][v].posible = true;
					quitar_celda_bloqueada(i, c);
				}
			}
		}
		for (int i = c1 * subDim; i < (c1 + 1) * subDim; i++) {
			for (int j = f1 * subDim; j < (f1 + 1) * subDim; j++) {
				if ((tablero.dame_Elem(j, i).es_vacia()) && (valores_celda.valores[j][i][v].celdas_que_afectan > 0)) {
					valores_celda.valores[j][i][v].celdas_que_afectan--;
					if (valores_celda.valores[j][i][v].celdas_que_afectan == 0) {
						valores_celda.valores[j][i][v].posible = true;
						quitar_celda_bloqueada(j, i);
					}
				}
			}
		}
		ok = true;
	rellena_arrValores();
	}
	return ok;
}

void tReglasSudoku::reset() {
	for (int i = 0; i < tablero.dame_Dim(); i++) {
		for (int j = 0; j < tablero.dame_Dim(); j++) {
			if (!tablero.dame_Elem(i, j).es_original()) quita_valor(i, j);
		}
	}
	rellena_arrValores();
}

void tReglasSudoku::autocompletar() {
	int k;
	int q;
	int r;
	for (int i = 0; i < tablero.dame_Dim(); i++) {
		for (int j = 0; j < tablero.dame_Dim(); j++) {
			if (tablero.dame_Elem(i, j).es_vacia()) {
				k = unica_celda_Vpos(i, j);
				q = unica_celda_columna(i, j);
				r = unica_celda_fila(i, j);
				if (posibles_valores(i, j) == 1) {
					pon_valor(i, j, unico_valor(i, j));
				}
				else if (k != 0) {
					pon_valor(i, j, k);
				}
				else if (q != 0) {
					pon_valor(i, j, q);
				}
				else if (r != 0) {
					pon_valor(i, j, r);
				}
			}
		}
	}
	rellena_arrValores();
}

void tReglasSudoku::carga_sudoku(ifstream& archivo) {
	int d = 0;
	int v;
	tCelda temp;
	archivo >> d;
	if (d != 0) {
		inicializar_valores_celda(d);
		int subDim = sqrt(d);
		tablero = tTablero(d);
		for (int f = 0; f < d; f++) {
			for (int c = 0; c < d; c++) {
				int f1 = f / subDim;
				int c1 = c / subDim;
				archivo >> v;
				if (v == 0) temp.set_vacia();
				else {
					if (v > 0) {
						temp.set_original();
						temp.set_valor(v);
					}
					else if (v < 0) {
						temp.set_ocupada();
						v = -1 * v;
						temp.set_valor(v);
					}
					contador_ocup++;
					for (int i = 0; i < d; i++) {
						valores_celda.valores[f][i][v].celdas_que_afectan++;
						if ((tablero.dame_Elem(f, i).es_vacia()) && (valores_celda.valores[f][i][v].posible)) {
							valores_celda.valores[f][i][v].posible = false;
						}
					}
					for (int i = 0; i < d; i++) {
						valores_celda.valores[i][c][v].celdas_que_afectan++;
						if ((tablero.dame_Elem(i, c).es_vacia()) && (valores_celda.valores[i][c][v].posible)) {
							valores_celda.valores[i][c][v].posible = false;
						}
					}
					for (int i = c1 * subDim; i < (c1 + 1) * subDim; i++) {
						for (int j = f1 * subDim; j < (f1 + 1) * subDim; j++) {
							valores_celda.valores[j][i][v].celdas_que_afectan++;
							if ((tablero.dame_Elem(j, i).es_vacia()) && (valores_celda.valores[j][i][v].posible)) {
								valores_celda.valores[j][i][v].posible = false;
							}
						}
					}
				}
				tablero.colocar(f, c, temp);
			}
		}
	}
	else tablero = tTablero(0);
	rellena_arrValores();
}
int tReglasSudoku::unica_celda_Vpos(int f, int c) const {
	int subDim = sqrt(dame_dimension());
	int f1 = f / subDim;
	int c1 = c / subDim;
	int unico = 0;
	bool encontrado = false;
	int v = 1;
	while ((v <= dame_dimension()) && (unico == 0)) {
		encontrado = false;
		if (es_valor_posible(f, c, v)) {
			for (int i = f1 * subDim; i < (f1 + 1) * subDim; i++) {
				for (int j = c1 * subDim; j < (c1 + 1) * subDim; j++) {
					if (((i != f) || (j != c)) && (es_valor_posible(i, j, v))) encontrado = true;
				}
			}
			if (encontrado == false) unico = v;
			else v++;
		}
		else v++;
	}
	return unico;
}
int tReglasSudoku::unica_celda_fila(int f, int c) const {
	int unico = 0;
	bool encontrado = false;
	int v = 1;
	while ((v <= dame_dimension()) && (unico == 0)) {
		encontrado = false;
		if (es_valor_posible(f, c, v)) {
			for (int j = 0; j < dame_dimension(); j++) {
				if ((j != c) && (es_valor_posible(f, j, v))) encontrado = true;

			}
			if (encontrado == false) unico = v;
			else v++;
		}
		else v++;
	}
	return unico;
}
int tReglasSudoku::unica_celda_columna(int f, int c) const {
	int unico = 0;
	bool encontrado = false;
	int v = 1;
	while ((v <= dame_dimension()) && (unico == 0)) {
		encontrado = false;
		if (es_valor_posible(f, c, v)) {
			for (int i = 0; i < dame_dimension(); i++) {
				if ((i != f) && (es_valor_posible(i, c, v))) encontrado = true;
			}
			if (encontrado == false) unico = v;
			else v++;
		}
		else v++;
	}
	return unico;
}
void tReglasSudoku::inicializar_valores_celda(int d) {
	for (int i = 0; i < d; i++) {
		for (int j = 0; j < d; j++) {
			for (int v = 0; v <= d; v++) {
				valores_celda.valores[j][i][v].celdas_que_afectan = 0;
				valores_celda.valores[j][i][v].posible = true;
			}
		}
	}
}
int tReglasSudoku::cuantas_celdas_pueden_tener(int n_valores) const {
	int n = 0;
	int contador = 0;
	for (int c = 0; c < tablero.dame_Dim(); c++) {
		for (int f = 0; f < tablero.dame_Dim(); f++) {
			for (int v = 1; v <= tablero.dame_Dim(); v++) {
				if (es_valor_posible(f, c, v)) n++;
			}
			if (n_valores == n) contador++;
			n = 0;
		}
	}
	return contador;
}
void tReglasSudoku::rellena_arrValores() {
	for (int v = 1; v <= tablero.dame_Dim(); v++) {
		cuantas_celdas[v] = cuantas_celdas_pueden_tener(v);
	}
}
bool tReglasSudoku::operator<(const tReglasSudoku& s2) const {
	bool ok = false;
	bool sigue = true;
	if (dame_dimension() < s2.dame_dimension()) ok = true;
	else if (dame_dimension() == s2.dame_dimension()) {
		if (contador_ocup > s2.contador_ocup) ok = true;
		else if (contador_ocup == s2.contador_ocup) {
			for (int v = 1; v <= s2.dame_dimension() && sigue; v++) {
				if (cuantas_celdas[v] > s2.cuantas_celdas[v]) {
					ok = true;
					sigue = false;
				}
				else if (cuantas_celdas[v] < s2.cuantas_celdas[v]) {
					sigue = false;
				}
			}
		}
	}
	return ok;
}
bool tReglasSudoku::operator==(const tReglasSudoku& s2) const {
	bool ok = true;
	if (!(s2.dame_dimension() == dame_dimension())) ok = false;
	else {
		if (!(contador_ocup == s2.contador_ocup) && ok) ok = false;
		else {
			for (int v = 1; v <= s2.dame_dimension() && ok; v++) {
				if (cuantas_celdas[v] != s2.cuantas_celdas[v]) ok = false;
			}
		}
	}
	return ok;
}
