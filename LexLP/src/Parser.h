#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Token.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>


class Parser {
private:
    using Nodo = std::unique_ptr<NodoAST>;
    std::vector<Token> tokens;
    std::size_t posicion;
    std::vector<std::string> errores;
    Nodo raiz;

    const Token& actual() const;
    bool aceptar(const std::string& tipo);
    bool esperar(const std::string& tipo, const std::string& esperado);

    bool esTipoBase(const Token& token) const;
    bool esComparador(const Token& token) const;
    bool esInicioComando(const Token& token) const;

    void registrarError(const std::string& mensaje);
    void sincronizarComando();

    Nodo PROGRAMA();
    Nodo LISTA_FUNCIONES();
    Nodo DECLARACION_FUNCION();
    Nodo PRINCIPAL();
    Nodo BLOQUE();

    Nodo TIPO();
    Nodo DIMENSION();
    Nodo PARAMETROS();
    Nodo LISTA_VARIABLES();
    Nodo DECLARACION();

    Nodo COMANDOS();
    Nodo COMANDO();
    Nodo LECTURA();
    Nodo ESCRITURA();
    Nodo ASIGNACION();
    Nodo COMPLEMENTO();
    Nodo FUNCION();
    Nodo ARGUMENTOS();
    Nodo SELECCION();
    Nodo MIENTRAS();
    Nodo RETORNO();

    Nodo TERMINO_LECTURA();
    Nodo TERMINO_ESCRITURA();
    Nodo DIMENSION2();
    Nodo CONSTANTE();

    Nodo EXPRESION();
    Nodo EXPRESION_O();
    Nodo EXPRESION_Y();
    Nodo EXPRESION_RELACIONAL();
    Nodo EXPRESION_ADITIVA();
    Nodo EXPRESION_MULTIPLICATIVA();
    Nodo FACTOR();
    Nodo TERMINO();

    Nodo crearNodo(
        const std::string& tipo,
        const std::string& valor = "",
        const Token* token = nullptr
    );

public:
    explicit Parser(const std::vector<Token>& tokens);

    bool analizar();
    const std::vector<std::string>& getErrores() const;
    const NodoAST* getAST() const;
};

#endif