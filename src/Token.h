#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {
private:
    std::string tipo;
    std::string lexema;
    int linea;
    int columna;

public:
    Token(std::string tipo, std::string lexema, int linea, int columna);

    std::string getTipo() const;
    std::string getLexema() const;
    int getLinea() const;
    int getColumna() const;
};

#endif