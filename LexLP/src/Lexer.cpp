#include "Lexer.h"

Lexer::Lexer(const std::string& fuente)
    : fuente(fuente), posicion(0), linea(1), columna(1) {
}

// Avanza un carácter dentro del código fuente.
void Lexer::avanzar() {
    if (posicion < fuente.length()) {
        if (fuente[posicion] == '\n') {
            linea++;
            columna = 1;
        } else {
            columna++;
        }

        posicion++;
    }
}

// Verifica si el carácter pertenece al rango 0-9.
bool Lexer::esDigito(char c) const {
    return c >= '0' && c <= '9';
}

// Reconoce números enteros y decimales.
Token Lexer::reconocerNumero() {

    int lineaInicial = linea;
    int columnaInicial = columna;

    std::string lexema;

    // Primero leemos todos los dígitos.
    while (posicion < fuente.length() && esDigito(fuente[posicion])) {
        lexema += fuente[posicion];
        avanzar();
    }

    // Si encontramos un punto, comprobamos si continúa con dígitos.
    if (posicion < fuente.length() &&
        fuente[posicion] == '.' &&
        posicion + 1 < fuente.length() &&
        esDigito(fuente[posicion + 1])) {

        lexema += fuente[posicion];
        avanzar();

        while (posicion < fuente.length() && esDigito(fuente[posicion])) {
            lexema += fuente[posicion];
            avanzar();
        }

        return Token("NUM_DEC", lexema, lineaInicial, columnaInicial);
    }

    return Token("NUM_INT", lexema, lineaInicial, columnaInicial);
}

// Analiza todo el código fuente.
std::vector<Token> Lexer::analizar() {

    std::vector<Token> tokens;

    while (posicion < fuente.length()) {

        char actual = fuente[posicion];

        // Ignoramos espacios, tabulaciones y saltos de línea.
        if (actual == ' ' || actual == '\t' || actual == '\n' || actual == '\r') {
            avanzar();
            continue;
        }

        if (esDigito(actual)) {
            tokens.push_back(reconocerNumero());
            continue;
        }
        avanzar();
    }

    return tokens;
}