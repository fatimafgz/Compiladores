#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include "SymbolTable.h"

#include <string>
#include <vector>

class Lexer {
private:
    std::string fuente;
    size_t posicion;
    int linea;
    int columna;

    SymbolTable tablaSimbolos;

public:
    Lexer(const std::string& fuente);

    std::vector<Token> analizar();

    SymbolTable& getTablaSimbolos();

private:
    void avanzar();

    bool esDigito(char c) const;
    bool esLetra(char c) const;

    Token reconocerNumero();
    Token reconocerIdentificador();
    Token reconocerTexto();

    bool esPalabraReservada(const std::string& lexema) const;
};

#endif