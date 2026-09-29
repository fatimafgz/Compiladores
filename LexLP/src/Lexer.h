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
    std::vector<std::string> errores;

public:
    Lexer(const std::string& fuente);

    std::vector<Token> analizar();

    SymbolTable& getTablaSimbolos();
    const std::vector<std::string>& getErrores() const;

private:
    void avanzar();

    bool esDigito(char c) const;
    bool esLetra(char c) const;
    bool esEspacio(char c) const;

    Token reconocerNumero();
    Token reconocerIdentificador();
    Token reconocerTexto();
    Token reconocerOperadorOsimbolo();

    bool esPalabraReservada(const std::string& lexema) const;
    std::string tipoPalabraReservada(const std::string& lexema) const;

    void agregarError(const std::string& mensaje,
                      int lineaError,
                      int columnaError);
};

#endif
