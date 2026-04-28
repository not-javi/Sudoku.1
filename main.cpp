#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include "colors.h"
#include "ListaSudokus.h"

using namespace std;

/*

LOS ARCHIVOS A INCLUIR SE LLAMAN:

Sudoku_"El numero que toque".txt
Partida_"El numero que toque".txt
cantidad_sudokus.txt

*/
		
// En el archivo cantidad_sudokus.txt el orden de los numeros es PR PA

//Funciones y metodos
void menu(int& opcion);
void mostrar_tablero(const tReglasSudoku& Sudoku);
void mostrar_bloq(const tReglasSudoku Sudoku);
void victoria();
void iniciar_lista_sudokus(tListaSudokus& Lista, int INIoPREDET, int cant);
int Elige_sudoku(int lim);
bool Partida(tReglasSudoku& sudoku);
void cant_sudokus(int& PR, int& PA);
void resumen(const tReglasSudoku& sudoku);
void guarda_partida(tReglasSudoku& sudoku, int indice);
void guardar_partidas(tListaSudokus &lista, int PR);
bool resolver_sudoku(tReglasSudoku& sudoku);
void ordenar(tListaSudokus& lista);
ostream& operator<<(ostream& o, tListaSudokus& sudokus);

int main() {
	char NCA = 'L';
	while (NCA != 'A') {
		int seleccionado;
		tListaSudokus ListaPR;
		tListaSudokus ListaPA;
		int PR, PA;
		bool terminado;
		cant_sudokus(PR, PA);
		iniciar_lista_sudokus(ListaPR, 0, PR);				//Envio un 0 o un 1 para que la funcion saque los sudokus de los archivos Sudoku.txt o Partida.txt	
		iniciar_lista_sudokus(ListaPA, 1, PA);
		while ((NCA != 'N') && (NCA != 'C') && (NCA != 'A') && (NCA != 'R')) {
			if (ListaPA.dame_num_elems() != 0) {
				cout << "Estos son los sudokus que tienes sin terminar" << endl;
				cout << ListaPA << endl;
			}
			cout << "Partida nueva (N), continuar partida (C), reiniciar las partidas guardads (R) o abandonar la aplicacion (A)? " << endl;
			cin >> NCA;
			if (cin.fail() || ((NCA != 'N') && (NCA != 'C') && (NCA != 'A') && (NCA != 'R'))) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Entrada invalida. Por favor introduce una letra permitida." << endl << endl;
				NCA = 'L';
			}
		}
		if (NCA == 'N') {
			cout <<  ListaPR  << endl;
			seleccionado = Elige_sudoku(ListaPR.dame_num_elems());
			terminado = Partida(ListaPR[seleccionado]);
			if (!terminado) ListaPA.insertar(ListaPR[seleccionado]);
			NCA = 'L';
		}
		if (NCA == 'C') {
			if (ListaPA.dame_num_elems() != 0) {
				cout << ListaPA << endl;
				seleccionado = Elige_sudoku(ListaPA.dame_num_elems());
				terminado = Partida(ListaPA[seleccionado]);
				if (terminado) ListaPA.eliminar(seleccionado);
			}
			else cout << "No tienes ninguna partida iniciada" << endl;
			NCA = 'L';
		}
		if (NCA == 'R') {
			if (ListaPA.dame_num_elems() != 0) {
				ofstream arch_cant;
				arch_cant.open("cantidad_sudokus.txt");
				arch_cant << PR << " " << 0;
				arch_cant.close();
				for (int i = ListaPA.dame_num_elems() - 1; i >= 0 ; i--) {
					ListaPA.eliminar(i);
				}
			}
			NCA = 'L';
		}
		
		guardar_partidas(ListaPA, PR);
		
	}

	return 0;
}

void mostrar_tablero(const tReglasSudoku& Sudoku) {
	int dim = Sudoku.dame_dimension();
	int subDim = sqrt(dim);

	string separador = "  +";
	for (int i = 0; i < subDim; i++) {
		for (int j = 0; j < subDim * 3; j++) {
			separador += "-";
		}
		separador += "+";
	}

	cout << endl << "   ";
	for (int c = 0; c < dim; c++) {
		if (c + 1 < 10) cout << " " << c + 1 << " ";
		else cout << " " << c + 1 << "";

		if ((c + 1) % subDim == 0 && c != dim - 1) cout << " ";
	}
	cout << endl;

	cout << separador << endl;

	for (int f = 0; f < dim; f++) {
		if (f + 1 < 10) cout << " " << f + 1 << "|";
		else cout << f + 1 << "|";

		for (int c = 0; c < dim; c++) {
			tCelda celda = Sudoku.dame_celda(f, c);

			if (celda.es_original()) cout << ORANGE;

			if (celda.es_vacia()) {
				cout << " . ";
			}
			else {
				int val = celda.dame_valor();
				if (val < 10) cout << " " << val << " ";
				else cout << " " << val << "";
			}

			if (celda.es_original()) cout << RESET;

			if ((c + 1) % subDim == 0) {
				cout << "|";
			}
		}
		cout << endl;

		if ((f + 1) % subDim == 0) {
			cout << separador << endl;
		}
	}
	cout << endl;
}
void menu(int& op) {
	cout << "--- MENU SUDOKU ---" << endl;
	cout << "1. Ver posibles valores" << endl;
	cout << "2. Poner valor" << endl;
	cout << "3. Quitar valor" << endl;
	cout << "4. Reiniciar el tablero" << endl;
	cout << "5. Rellenar valores unicos" << endl;
	cout << "6. Autocompletar" << endl;
	cout << "7. Salir" << endl;
	cout << "Elige una opcion: ";
	cin >> op;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(10000, '\n');
		cout << "Entrada invalida. Por favor introduce un numero." << endl << endl;
		op = 0;
	}
}
void mostrar_bloq(const tReglasSudoku Sudoku) {
	int n = Sudoku.dame_num_celdas_bloqueadas();
	if (Sudoku.bloqueo()) {
		int fB;
		int cB;
		cout << endl << endl << "Tienes " << n << " celdas bloqueadas, quita valores para desbloquearlas, las celdas bloqueadas son las siguientes" << endl;
		for (int i = 0; i < n; i++) {
			Sudoku.dame_celda_bloqueada(i, fB, cB);
			cout << " Celda bloqueada numero " << i + 1 << "	Fila: " << fB + 1 << "	Columna: " << cB + 1 << endl << endl;
		}
	}
}
void victoria() {
	cout << endl << endl;
	cout << YELLOW;
	cout << "  * * * * * * * *" << endl;
	cout << "      * * * * * * * " << endl;
	cout << R"(
     _   _    _    ____     ____    _    _   _    _    ____   ___  
    | | | |  / \  / ___|   / ___|  / \  | \ | |  / \  |  _ \ / _ \ 
    | |_| | / _ \ \___ \  | |  _  / _ \ |  \| | / _ \ | | | | | | |
    |  _  |/ ___ \ ___) | | |_| |/ ___ \| |\  |/ ___ \| |_| | |_| |
    |_| |_/_/   \_\____/   \____/_/   \_\_| \_/_/   \_\____/ \___/ 
    )";
	cout << endl;
	cout << "      * * * * * * * " << endl;
	cout << "  * * * * * * * *" << endl;
	cout << RESET;
	cout << endl << endl;
}

void iniciar_lista_sudokus(tListaSudokus& Lista, int INIoPREDET,int cant) {
	string SDK;
	if (INIoPREDET == 0) {
		SDK = "Sudoku_";
	}
	else if (INIoPREDET == 1) {
		SDK = "Partida_";
	}
	string txt = ".txt";
	for (int i = 0; i < cant; i++) {
		ifstream archivo;
		tReglasSudoku sudoku;
		string ArchSudoku = SDK + to_string(i) + txt;
		archivo.open(ArchSudoku);
		if (archivo.is_open()) {
			sudoku.carga_sudoku(archivo);
			if (sudoku.dame_dimension() != 0) {
				Lista.insertar(sudoku);
			}
			archivo.close();
		}
		else {
			cout << endl << "Hubo un error al cargar el sudoku " << i + 1 << endl;
		}
	}
}

int Elige_sudoku(int lim) {
	int Eleccion_Sudoku = 0;
	int resultado;
	cout << "Cual sudoku quieres jugar, tienes para elegir desde el sudoku 1 hasta el sudoku " << lim << endl;
	while ((Eleccion_Sudoku < 1) || (Eleccion_Sudoku > lim)) {
		cin >> Eleccion_Sudoku;
		resultado = Eleccion_Sudoku;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Entrada invalida. Por favor introduce un numero." << endl << endl;
			Eleccion_Sudoku = 0;
			resultado = 0;
		}
		else if ((Eleccion_Sudoku < 1) || (Eleccion_Sudoku > lim)) cout << "No tenemos ese sudoku en stock D:" << endl;
	}
	return resultado - 1;
}

bool Partida(tReglasSudoku& sudoku) {
	int f, c, v;
	int opcion = -1;
	bool ok;
	bool terminado = false;
	while (opcion != 7) {
		mostrar_tablero(sudoku);
		mostrar_bloq(sudoku);
		menu(opcion);
		switch (opcion) {
		case 1: {																													// Ver posibles valores
			c = -1;
			f = -1;
			cout << "De que casilla quieres ver los posibles valores?" << endl << "FILA:" << endl;
			cin >> f;
			cout << "COLUMNA" << endl;
			cin >> c;
			cout << "Esa casilla tiene " << sudoku.posibles_valores(f - 1, c - 1) << " posibles valores" << endl;
			break;
		}
		case 2: {																													// Poner valor
			ok = false;
			v = -1;
			f = -1;
			c = -1;
			cout << "Que valor quiere poner?" << endl;
			cin >> v;
			cout << "En que casilla quiere ponerlo?" << endl << "FILA:" << endl;
			cin >> f;
			cout << "COLUMNA" << endl;
			cin >> c;
			ok = sudoku.pon_valor(f - 1, c - 1, v);
			if (!ok) cout << "Error al poner el valor " << v << " en la casilla (" << f << "," << c << ")" << endl;
			break;
		}
		case 3: {																													// Quitar valor
			ok = false;
			f = -1;
			c = -1;
			cout << "Que casilla quiere quitar?" << endl << "FILA:" << endl;
			cin >> f;
			cout << "COLUMNA" << endl;
			cin >> c;
			ok = sudoku.quita_valor(f - 1, c - 1);
			if (!ok) cout << "Error al intentar quitar la casilla (" << f << "," << c << ")" << endl;
			break;
		}
		case 4: {																													// Reiniciar tablero
			sudoku.reset();
			break;
		}
		case 5: {																													// Autocompletar
			sudoku.autocompletar();
			break;
		}
		case  6: {
			resolver_sudoku(sudoku);
			break;
		}
		case 7: {																													//Cerrar
			cout << "Hasta la proxima!" << endl;
			break;
		}
		}
		if (sudoku.terminado()) {
			mostrar_tablero(sudoku);
			victoria();
			terminado = true;
			opcion = 7;
		}
	}
	return terminado;
}
void cant_sudokus(int& PR, int& PA) {
	ifstream arch_cant;
	arch_cant.open("cantidad_sudokus.txt");
	if (arch_cant.is_open()) {
		arch_cant >> PR;
		arch_cant >> PA;
	}
	else {
		cout << "Hubo un error al cargar el archivo cantidad_sudokus.txt" << endl;
	}
}
void resumen(const tReglasSudoku& sudoku) {
	cout << "Sudoku con " << sudoku.dame_num_celdas_vacias() << " casillas vacias" << endl;
	int dim = sudoku.dame_dimension();
	int n = 0;
	for (int v = 1; v < 10 ; v++) {
		for (int f = 0; f < dim; f++) {
			for (int c = 0; c < dim; c++) {
				if (sudoku.posibles_valores(f, c) == v) n++;
			}
		}
		cout << "	Celdas con" << v << " valores posibles:" << n << endl;
		n = 0;
	}
	
}

void guarda_partida(tReglasSudoku& sudoku, int indice) {
	ofstream archivo;
	int v;
	string partida = "Partida_";
	string txt = ".txt";
	partida = partida + to_string(indice) + txt;
	archivo.open(partida);
	archivo << sudoku.dame_dimension() << endl;
	for (int f = 0; f < sudoku.dame_dimension(); f++) {
		for (int c = 0; c < sudoku.dame_dimension(); c++) {
			v = sudoku.dame_celda(f, c).dame_valor();
			if (sudoku.dame_celda(f, c).es_ocupada()) v = -v;
			if (sudoku.dame_celda(f, c).es_vacia()) archivo << 0 << " ";
			else archivo << v << " ";
		}
		archivo << endl;
	}
	archivo.close();
}

void guardar_partidas(tListaSudokus &lista, int PR) {
	ofstream arch_cant;
	arch_cant.open("cantidad_sudokus.txt");
	arch_cant << PR << " " << lista.dame_num_elems();
	arch_cant.close();
	for (int i = 0; i < lista.dame_num_elems(); i++) {
		guarda_partida(lista[i], i);
	}
}

bool resolver_sudoku(tReglasSudoku& sudoku) {
	bool ok = false;
	sudoku.autocompletar();
	if (sudoku.bloqueo()) ok = false;
	else if (sudoku.terminado()) ok = true;
	else {
		int f_vacia = -1;
		int c_vacia = -1;
		bool encontrada = false;
		for (int f = 0; f < sudoku.dame_dimension() && !encontrada; f++) {
			for (int c = 0; c < sudoku.dame_dimension() && !encontrada; c++) {
				if (sudoku.dame_celda(f, c).es_vacia()) {
					f_vacia = f;
					c_vacia = c;
					encontrada = true;
				}
			}
		}
		tReglasSudoku aux = sudoku;
		for (int v = 1; v <= sudoku.dame_dimension() && !ok; v++) {
			if (sudoku.es_valor_posible(f_vacia, c_vacia, v)) {
				sudoku.pon_valor(f_vacia, c_vacia, v);
				bool extra = resolver_sudoku(sudoku);
				if (extra == true) {
					ok = true;
				}
				else {
					sudoku = aux;
				}
			}
		}
	}
	return ok;
}
void ordenar(tListaSudokus& lista) {
	tReglasSudoku temp;
	bool cambio = true;
	while (cambio) {
		cambio = false;
		for (int i = 0; i < lista.dame_num_elems() - 1; i++) {
			if (lista[i + 1] < lista[i]) {
				temp = lista[i];
				lista[i] = lista[i + 1];
				lista[i + 1] = temp;
				cambio = true;
			}
		}
	}
}

ostream& operator<<(ostream& o, tListaSudokus& sudokus) {
	ordenar(sudokus);
	for (int i = 0; i < sudokus.dame_num_elems(); i++) {
		o << i + 1;
		o << endl << "Sudoku con "<< CYAN << sudokus[i].dame_num_celdas_vacias() << RESET << " casillas vacias" << endl << endl;
		int n = 0;
		for (int v = 1; v < 10; v++) {
			for (int f = 0; f < sudokus[i].dame_dimension(); f++) {
				for (int c = 0; c < sudokus[i].dame_dimension(); c++) {
					if (sudokus[i].posibles_valores(f, c) == v) n++;
				}
			}
			o << "	Celdas con " << YELLOW << v << RESET << " valores posibles:" << YELLOW << n << RESET << endl;
			n = 0;
		}
	}
	return o;
}
