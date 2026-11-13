#include "parser.h"
#include <sstream>

Parser::Parser(const std::vector<Token>& tokens)
    : tokens(tokens), posicion(0) {
}

const Token& Parser::actual() const {
    static const Token fin("FIN", "", 0, 0);

    if (posicion >= tokens.size()) {
        return fin;
    }

    return tokens[posicion];
}

bool Parser::esTipoBase(const Token& token) const {
    const std::string tipo = token.getTipo();
    return tipo == "INT" || tipo == "FLOAT" ||
           tipo == "CHAR" || tipo == "BOOLEAN";
}

bool Parser::aceptar(const std::string& tipo) {
    if (actual().getTipo() == tipo) {
        ++posicion;
        return true;
    }
    return false;
}

void Parser::registrarError(const std::string& mensaje) {
    std::ostringstream salida;
    salida << "Error sintactico en linea " << actual().getLinea()
           << ", columna " << actual().getColumna()
           << ": " << mensaje
           << ". Encontrado: '" << actual().getLexema() << "'";
    errores.push_back(salida.str());
}

bool Parser::esperar(
    const std::string& tipo,
    const std::string& esperado
) {
    if (aceptar(tipo)) {
        return true;
    }

    registrarError("se esperaba " + esperado);
    return false;
}

bool Parser::analizar() {
    errores.clear();
    posicion = 0;
    PROGRAMA();
    return errores.empty() && actual().getTipo() == "FIN";
}

const std::vector<std::string>& Parser::getErrores() const {
    return errores;
}

void Parser::PROGRAMA() {
    PRINCIPAL();

    if (actual().getTipo() != "FIN") {
        registrarError("no se esperaba contenido despues del bloque principal");
    }
}

void Parser::PRINCIPAL() {
    esperar("VOID", "'void'");
    esperar("MAIN", "'main'");
    esperar("(", "'('");
    esperar(")", "')'");
    BLOQUE();
}

void Parser::BLOQUE() {
    if (!esperar("{", "'{'")) {
        return;
    }

    LISTA_VARIABLES();
    COMANDOS();
    esperar("}", "'}'");
}

void Parser::LISTA_VARIABLES() {
    while (esTipoBase(actual())) {
        DECLARACION();
    }
}

void Parser::DECLARACION() {
    ++posicion; // Consume el tipo: int, float, char o boolean.
    esperar("ID", "un identificador");

    while (aceptar(",")) {
        esperar("ID", "un identificador despues de ','");
    }

    if (!esperar(";", "';' al final de la declaracion")) {
        while (actual().getTipo() != ";" &&
               actual().getTipo() != "}" &&
               actual().getTipo() != "FIN") {
            ++posicion;
        }
        aceptar(";");
    }
}

void Parser::COMANDOS() {
    while (actual().getTipo() != "}" &&
           actual().getTipo() != "FIN") {
        const size_t posicionAnterior = posicion;
        COMANDO();

        if (posicion == posicionAnterior) {
            ++posicion;
        }
    }
}

void Parser::COMANDO() {
    if (actual().getTipo() == "ID") {
        ASIGNACION();
        return;
    }

    registrarError("se esperaba una asignacion");
    ++posicion;
}

void Parser::ASIGNACION() {
    esperar("ID", "un identificador");
    esperar("=", "'='");
    COMPLEMENTO();
    esperar(";", "';' al final de la asignacion");
}

void Parser::COMPLEMENTO() {
    EXPRESION();
}

void Parser::EXPRESION() {
    EXPRESION_ADITIVA();
}

void Parser::EXPRESION_ADITIVA() {
    EXPRESION_MULTIPLICATIVA();

    while (actual().getTipo() == "+" || actual().getTipo() == "-") {
        ++posicion;
        EXPRESION_MULTIPLICATIVA();
    }
}

void Parser::EXPRESION_MULTIPLICATIVA() {
    FACTOR();

    while (actual().getTipo() == "*" ||
           actual().getTipo() == "/" ||
           actual().getTipo() == "%") {
        ++posicion;
        FACTOR();
    }
}

void Parser::FACTOR() {
    if (aceptar("+") || aceptar("-")) {
        FACTOR();
        return;
    }

    if (aceptar("ID") ||
        aceptar("NUM_INT") ||
        aceptar("NUM_DEC")) {
        return;
    }

    if (aceptar("(")) {
        EXPRESION();
        esperar(")", "')'");
        return;
    }

    registrarError(
        "se esperaba un identificador, numero o expresion entre parentesis"
    );

    if (actual().getTipo() != "FIN") {
        ++posicion;
    }
}