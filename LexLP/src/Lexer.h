#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <string>
#include <vector>

class Lexer {
private:
    std::string fuente;
    size_t posicion;
    int linea;
    int columna;

public:
    Lexer(const std::string& fuente);

    std::vector<Token> analizar();

private:
    void avanzar();

    bool esDigito(char c) const;

    Token reconocerNumero();
};

#endif