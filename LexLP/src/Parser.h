#ifndef PARSER_H
#define PARSER_H

#include "Token.h"
#include <string>
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    size_t posicion;
    std::vector<std::string> errores;

    const Token& actual() const;
    bool esTipoBase(const Token& token) const;
    bool aceptar(const std::string& tipo);
    bool esperar(const std::string& tipo, const std::string& esperado);
    void registrarError(const std::string& mensaje);

    void PROGRAMA();
    void PRINCIPAL();
    void BLOQUE();
    void LISTA_VARIABLES();
    void DECLARACION();
    void COMANDOS();
    void COMANDO();
    void ASIGNACION();
    void COMPLEMENTO();
    void EXPRESION();
    void EXPRESION_ADITIVA();
    void EXPRESION_MULTIPLICATIVA();
    void FACTOR();

public:
    explicit Parser(const std::vector<Token>& tokens);
    bool analizar();
    const std::vector<std::string>& getErrores() const;
};

#endif