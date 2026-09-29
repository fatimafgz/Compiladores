#include "Lexer.h"

Lexer::Lexer(const std::string& fuente)
    : fuente(fuente), posicion(0), linea(1), columna(1) {
}

// Avanza un carácter y actualiza línea y columna
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

// Verifica si el carácter es un número
bool Lexer::esDigito(char c) const {
    return c >= '0' && c <= '9';
}

// Verifica si el carácter puede iniciar un identificador
bool Lexer::esLetra(char c) const {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

// Verifica espacios, tabulaciones y saltos de línea
bool Lexer::esEspacio(char c) const {
    return c == ' ' ||
           c == '\t' ||
           c == '\n' ||
           c == '\r';
}


// =====================================================
// NÚMEROS
// =====================================================

Token Lexer::reconocerNumero() {

    int lineaInicial = linea;
    int columnaInicial = columna;

    std::string lexema;

    // Lee la parte entera
    while (posicion < fuente.length() &&
           esDigito(fuente[posicion])) {

        lexema += fuente[posicion];
        avanzar();
    }

    // Verifica si es decimal
    if (posicion < fuente.length() &&
        fuente[posicion] == '.') {

        // Debe existir al menos un número después del punto
        if (posicion + 1 < fuente.length() &&
            esDigito(fuente[posicion + 1])) {

            lexema += fuente[posicion];
            avanzar();

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

        // Ejemplo inválido: 15.
        lexema += '.';
        avanzar();

        agregarError(
            "Número decimal inválido: " + lexema,
            lineaInicial,
            columnaInicial
        );

        return Token(
            "ERROR",
            lexema,
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


// =====================================================
// IDENTIFICADORES Y PALABRAS RESERVADAS
// =====================================================

Token Lexer::reconocerIdentificador() {

    int lineaInicial = linea;
    int columnaInicial = columna;

    std::string lexema;

    // El primer carácter debe ser letra o _
    while (posicion < fuente.length() &&
           (esLetra(fuente[posicion]) ||
            esDigito(fuente[posicion]))) {

        lexema += fuente[posicion];
        avanzar();
    }

    // Si es palabra reservada
    if (esPalabraReservada(lexema)) {

        return Token(
            tipoPalabraReservada(lexema),
            lexema,
            lineaInicial,
            columnaInicial
        );
    }

    // Si no es reservada, es identificador
    tablaSimbolos.agregar(lexema);

    return Token(
        "ID",
        lexema,
        lineaInicial,
        columnaInicial
    );
}


// =====================================================
// TEXTO
// =====================================================

Token Lexer::reconocerTexto() {

    int lineaInicial = linea;
    int columnaInicial = columna;

    std::string lexema;

    // Guarda la comilla inicial
    lexema += fuente[posicion];
    avanzar();

    while (posicion < fuente.length()) {

        // Si encuentra la comilla final
        if (fuente[posicion] == '"') {

            lexema += fuente[posicion];
            avanzar();

            return Token(
                "TEXTO",
                lexema,
                lineaInicial,
                columnaInicial
            );
        }

        // Un salto de línea significa que el texto
        // quedó sin cerrar
        if (fuente[posicion] == '\n') {

            agregarError(
                "Texto sin comilla de cierre",
                lineaInicial,
                columnaInicial
            );

            return Token(
                "ERROR",
                lexema,
                lineaInicial,
                columnaInicial
            );
        }

        lexema += fuente[posicion];
        avanzar();
    }

    // Llegó al final del archivo sin encontrar "
    agregarError(
        "Texto sin comilla de cierre",
        lineaInicial,
        columnaInicial
    );

    return Token(
        "ERROR",
        lexema,
        lineaInicial,
        columnaInicial
    );
}


// =====================================================
// PALABRAS RESERVADAS
// =====================================================

bool Lexer::esPalabraReservada(
    const std::string& lexema) const {

    return lexema == "int" ||
           lexema == "float" ||
           lexema == "char" ||
           lexema == "boolean" ||
           lexema == "void" ||
           lexema == "if" ||
           lexema == "else" ||
           lexema == "for" ||
           lexema == "while" ||
           lexema == "scanf" ||
           lexema == "println" ||
           lexema == "main" ||
           lexema == "return";
}


// Devuelve el token específico de cada palabra reservada
std::string Lexer::tipoPalabraReservada(
    const std::string& lexema) const {

    if (lexema == "int") return "INT";
    if (lexema == "float") return "FLOAT";
    if (lexema == "char") return "CHAR";
    if (lexema == "boolean") return "BOOLEAN";
    if (lexema == "void") return "VOID";
    if (lexema == "if") return "IF";
    if (lexema == "else") return "ELSE";
    if (lexema == "for") return "FOR";
    if (lexema == "while") return "WHILE";
    if (lexema == "scanf") return "SCANF";
    if (lexema == "println") return "PRINTLN";
    if (lexema == "main") return "MAIN";
    if (lexema == "return") return "RETURN";

    return "ID";
}


// =====================================================
// OPERADORES Y SÍMBOLOS
// =====================================================

Token Lexer::reconocerOperadorOsimbolo() {

    int lineaInicial = linea;
    int columnaInicial = columna;

    char actual = fuente[posicion];

    // -------------------------------------------------
    // COMENTARIOS //
    // -------------------------------------------------

    if (actual == '/' &&
        posicion + 1 < fuente.length() &&
        fuente[posicion + 1] == '/') {

        std::string lexema;

        while (posicion < fuente.length() &&
               fuente[posicion] != '\n') {

            lexema += fuente[posicion];
            avanzar();
        }

        return Token(
            "COMENT",
            lexema,
            lineaInicial,
            columnaInicial
        );
    }


    // -------------------------------------------------
    // OPERADORES DE DOS CARACTERES
    // -------------------------------------------------

    if (posicion + 1 < fuente.length()) {

        std::string dosCaracteres =
            fuente.substr(posicion, 2);

        if (dosCaracteres == "&&" ||
            dosCaracteres == "||" ||
            dosCaracteres == ">=" ||
            dosCaracteres == "<=" ||
            dosCaracteres == "!=" ||
            dosCaracteres == "==") {

            avanzar();
            avanzar();

            return Token(
                dosCaracteres,
                dosCaracteres,
                lineaInicial,
                columnaInicial
            );
        }
    }


    // -------------------------------------------------
    // OPERADORES Y SÍMBOLOS DE UN CARÁCTER
    // -------------------------------------------------

    std::string lexema(1, actual);

    avanzar();

    switch (actual) {

        // Asignación
        case '=':
            return Token(
                "=",
                lexema,
                lineaInicial,
                columnaInicial
            );

        // Operadores aritméticos
        case '+':
            return Token(
                "+",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '-':
            return Token(
                "-",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '*':
            return Token(
                "*",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '/':
            return Token(
                "/",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '%':
            return Token(
                "%",
                lexema,
                lineaInicial,
                columnaInicial
            );

        // Operador lógico
        case '!':
            return Token(
                "!",
                lexema,
                lineaInicial,
                columnaInicial
            );

        // Comparadores
        case '>':
            return Token(
                "COMP",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '<':
            return Token(
                "COMP",
                lexema,
                lineaInicial,
                columnaInicial
            );

        // Símbolos especiales
        case '(':
            return Token(
                "(",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case ')':
            return Token(
                ")",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '[':
            return Token(
                "[",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case ']':
            return Token(
                "]",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '{':
            return Token(
                "{",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case '}':
            return Token(
                "}",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case ',':
            return Token(
                ",",
                lexema,
                lineaInicial,
                columnaInicial
            );

        case ';':
            return Token(
                ";",
                lexema,
                lineaInicial,
                columnaInicial
            );

        default:

            agregarError(
                "Caracter no reconocido: " + lexema,
                lineaInicial,
                columnaInicial
            );

            return Token(
                "ERROR",
                lexema,
                lineaInicial,
                columnaInicial
            );
    }
}


// =====================================================
// REGISTRO DE ERRORES
// =====================================================

void Lexer::agregarError(
    const std::string& mensaje,
    int lineaError,
    int columnaError) {

    errores.push_back(
        mensaje +
        " (Linea: " +
        std::to_string(lineaError) +
        ", Columna: " +
        std::to_string(columnaError) +
        ")"
    );
}


// =====================================================
// ANALIZADOR PRINCIPAL
// =====================================================

std::vector<Token> Lexer::analizar() {

    std::vector<Token> tokens;

    while (posicion < fuente.length()) {

        char actual = fuente[posicion];

        // Ignorar espacios
        if (esEspacio(actual)) {
            avanzar();
            continue;
        }

        // Números
        if (esDigito(actual)) {
            tokens.push_back(
                reconocerNumero()
            );
            continue;
        }

        // Identificadores / palabras reservadas
        if (esLetra(actual)) {
            tokens.push_back(
                reconocerIdentificador()
            );
            continue;
        }

        // Textos
        if (actual == '"') {
            tokens.push_back(
                reconocerTexto()
            );
            continue;
        }

        // Operadores, símbolos y comentarios
        tokens.push_back(
            reconocerOperadorOsimbolo()
        );
    }

    return tokens;
}


// =====================================================
// GETTERS
// =====================================================

SymbolTable& Lexer::getTablaSimbolos() {
    return tablaSimbolos;
}

const std::vector<std::string>& Lexer::getErrores() const {
    return errores;
}
