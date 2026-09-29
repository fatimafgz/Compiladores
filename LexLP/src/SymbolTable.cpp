#include "SymbolTable.h"

int SymbolTable::buscar(
    const std::string& lexema) const {

    for (int i = 0; i < simbolos.size(); i++) {

        if (simbolos[i] == lexema) {
            return i;
        }
    }

    return -1;
}


int SymbolTable::agregar(
    const std::string& lexema) {

    int posicion = buscar(lexema);

    // Si ya existe, devuelve su posición
    if (posicion != -1) {
        return posicion;
    }

    // Si no existe, lo agrega
    simbolos.push_back(lexema);

    return simbolos.size() - 1;
}


void SymbolTable::mostrar(
    std::ostream& salida) const {

    for (int i = 0; i < simbolos.size(); i++) {

        salida << i
               << " -> "
               << simbolos[i]
               << std::endl;
    }
}
