#include "Lexer.h"

Lexer::Lexer(const std::string& fuente)
    : fuente(fuente), posicion(0), linea(1), columna(1) {
}

// Avanza al siguiente carácter.
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

// Comprueba si el carácter es un dígito.
bool Lexer::esDigito(char c) const {
    return c >= '0' && c <= '9';
}

// Comprueba si el carácter es una letra o "_".
bool Lexer::esLetra(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// Reconoce números enteros y decimales.
Token Lexer::reconocerNumero() {

    int lineaInicial = linea;
    int columnaInicial = columna;
    std::string lexema;

    // Lee los dígitos antes del punto.
    while (posicion < fuente.length() &&
           esDigito(fuente[posicion])) {

        lexema += fuente[posicion];
        avanzar();
    }

    // Comprueba si existe un punto decimal.
    if (posicion < fuente.length() &&
        fuente[posicion] == '.') {

        // El punto debe estar seguido de un dígito.
        if (posicion + 1 < fuente.length() &&
            esDigito(fuente[posicion + 1])) {

            lexema += fuente[posicion];
            avanzar();

            // Lee los dígitos después del punto.
            while (posicion < fuente.length() &&
                   esDigito(fuente[posicion])) {

                lexema += fuente[posicion];
                avanzar();
            }

            return Token(
                "NUM_DEC",
                lexema,
                lineaInicial,
                columnaInicial
            );
        }

        return Token(
            "ERROR",
            lexema + ".",
            lineaInicial,
            columnaInicial
        );
    }

    return Token(
        "NUM_INT",
        lexema,
        lineaInicial,
        columnaInicial
    );
}

// Reconoce identificadores y palabras reservadas.
Token Lexer::reconocerIdentificador() {

    int lineaInicial = linea;
    int columnaInicial = columna;
    std::string lexema;

    // El primer carácter debe ser letra o "_".
    while (posicion < fuente.length() &&
           (esLetra(fuente[posicion]) ||
            esDigito(fuente[posicion]))) {

        lexema += fuente[posicion];
        avanzar();
    }

    // Comprueba si es una palabra reservada.
    if (esPalabraReservada(lexema)) {

        return Token(
            "PALABRA_RESERVADA",
            lexema,
            lineaInicial,
            columnaInicial
        );
    }

    // Si no es reservada, es un identificador.
    tablaSimbolos.agregar(lexema);

    return Token(
        "ID",
        lexema,
        lineaInicial,
        columnaInicial
    );
}

// Reconoce textos entre comillas.
Token Lexer::reconocerTexto() {

    int lineaInicial = linea;
    int columnaInicial = columna;
    std::string lexema;

    // Guarda la comilla inicial.
    lexema += fuente[posicion];
    avanzar();

    // Lee hasta encontrar la comilla final.
    while (posicion < fuente.length() &&
           fuente[posicion] != '"') {

        lexema += fuente[posicion];
        avanzar();
    }

    // Si encontró la comilla final.
    if (posicion < fuente.length() &&
        fuente[posicion] == '"') {

        lexema += fuente[posicion];
        avanzar();

        return Token(
            "TEXTO",
            lexema,
            lineaInicial,
            columnaInicial
        );
    }

    // Si no encontró la comilla final.
    return Token(
        "ERROR",
        lexema,
        lineaInicial,
        columnaInicial
    );
}

// Comprueba si el lexema es una palabra reservada.
bool Lexer::esPalabraReservada(
    const std::string& lexema) const {

    return lexema == "if" ||
           lexema == "else" ||
           lexema == "while" ||
           lexema == "for" ||
           lexema == "return" ||
           lexema == "int" ||
           lexema == "float" ||
           lexema == "string" ||
           lexema == "void" ||
           lexema == "main";
}

// Analiza todo el código fuente.
std::vector<Token> Lexer::analizar() {

    std::vector<Token> tokens;

    while (posicion < fuente.length()) {

        char actual = fuente[posicion];

        // Ignora espacios y saltos de línea.
        if (actual == ' ' ||
            actual == '\t' ||
            actual == '\n' ||
            actual == '\r') {

            avanzar();
            continue;
        }

        // Reconoce números.
        if (esDigito(actual)) {

            tokens.push_back(reconocerNumero());
            continue;
        }

        // Reconoce identificadores y palabras reservadas.
        if (esLetra(actual)) {

            tokens.push_back(reconocerIdentificador());
            continue;
        }

        // Reconoce textos.
        if (actual == '"') {

            tokens.push_back(reconocerTexto());
            continue;
        }

        // Por ahora, otros caracteres no se procesan.
        avanzar();
    }

    return tokens;
}

// Permite acceder a la tabla de símbolos.
SymbolTable& Lexer::getTablaSimbolos() {
    return tablaSimbolos;
}