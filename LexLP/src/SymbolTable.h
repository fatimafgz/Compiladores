#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <vector>
#include <ostream>

class SymbolTable {
private:
    std::vector<std::string> simbolos;

public:
    int agregar(const std::string& lexema);
    int buscar(const std::string& lexema) const;

    void mostrar(std::ostream& salida) const;
};

#endif
