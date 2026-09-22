#include "Token.h"

Token::Token(std::string tipo, std::string lexema, int linea, int columna)
    : tipo(tipo), lexema(lexema), linea(linea), columna(columna) {
}

std::string Token::getTipo() const {
    return tipo;
}

std::string Token::getLexema() const {
    return lexema;
}

int Token::getLinea() const {
    return linea;
}

int Token::getColumna() const {
    return columna;
}