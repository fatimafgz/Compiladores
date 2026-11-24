#ifndef AST_H
#define AST_H

#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

struct NodoAST {
    std::string tipo;
    std::string valor;
    int linea;
    int columna;
    std::vector<std::unique_ptr<NodoAST>> hijos;

    NodoAST(
        std::string tipo,
        std::string valor = "",
        int linea = 0,
        int columna = 0
    )
        : tipo(std::move(tipo)),
          valor(std::move(valor)),
          linea(linea),
          columna(columna) {
    }

    void agregar(std::unique_ptr<NodoAST> hijo) {
        if (hijo) {
            hijos.push_back(std::move(hijo));
        }
    }
};

inline void imprimirAST(
    const NodoAST& nodo,
    std::ostream& salida,
    int nivel = 0
) {
    salida << std::string(nivel * 2, ' ') << nodo.tipo;

    if (!nodo.valor.empty()) {
        salida << ": " << nodo.valor;
    }

    salida << '\n';

    for (const auto& hijo : nodo.hijos) {
        imprimirAST(*hijo, salida, nivel + 1);
    }
}

#endif