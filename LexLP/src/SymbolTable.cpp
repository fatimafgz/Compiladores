#include "SymbolTable.h"
#include <iostream>

// Busca un identificador en la tabla.
// Devuelve su posición si existe.
// Devuelve -1 si no existe.
int SymbolTable::buscar(const std::string& lexema) const {

    for (int i = 0; i < simbolos.size(); i++) {

        if (simbolos[i] == lexema) {
            return i;
        }
    }

    return -1;
}

// Agrega un identificador si todavía no existe.
// Devuelve su posición en la tabla.
int SymbolTable::agregar(const std::string& lexema) {

    int posicion = buscar(lexema);

    // Si ya existe, no lo vuelve a agregar.
    if (posicion != -1) {
        return posicion;
    }

    simbolos.push_back(lexema);

    return simbolos.size() - 1;
}

// Muestra todos los identificadores de la tabla.
void SymbolTable::mostrar() const {

    std::cout << "\n===== TABLA DE SIMBOLOS =====\n";

    for (int i = 0; i < simbolos.size(); i++) {
        std::cout << i << " -> " << simbolos[i] << std::endl;
    }
}