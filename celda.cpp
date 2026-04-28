#include <iostream>
#include "celda.h"

using namespace std;

tCelda::tCelda() : valor(0), est(VACIA) {}

tCelda::tCelda(int v, tEstado estado) : valor(v), est(estado) {}

bool tCelda::es_vacia() const { return (est == VACIA); }

bool tCelda::es_original() const { return (est == ORIGINAL); }

bool tCelda::es_ocupada() const { return (est == OCUPADA); }

int tCelda::dame_valor() const { return valor; }

void tCelda::set_ocupada() { est = OCUPADA; }

void tCelda::set_valor(int v) { valor = v; }

void tCelda::set_vacia() {
	est = VACIA;
	set_valor(0);
}

void tCelda::set_original() { est = ORIGINAL; }