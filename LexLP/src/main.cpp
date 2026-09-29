#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Lexer.h"
#include "Token.h"

int main() {

    std::string nombreArchivo =
        "tests/prueba_errores.lp";

    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {

        std::cerr
            << "Error: no se pudo abrir el archivo "
            << nombreArchivo
            << std::endl;

        return 1;
    }


    // Leer todo el archivo
    std::stringstream buffer;

    buffer << archivo.rdbuf();

    std::string fuente = buffer.str();

    archivo.close();


    // Crear lexer
    Lexer lexer(fuente);

    // Analizar
    std::vector<Token> tokens =
        lexer.analizar();


    // =================================================
    // MOSTRAR TOKENS EN CONSOLA
    // =================================================

    std::cout
        << "===== ANALIZADOR LEXICO LexLP =====\n\n";

    std::cout
        << "LISTA DE TOKENS:\n\n";


    for (const Token& token : tokens) {

        std::cout
            << "<"
            << token.getTipo()
            << "> "
            << token.getLexema()
            << "  (Linea: "
            << token.getLinea()
            << ", Columna: "
            << token.getColumna()
            << ")\n";
    }


    // =================================================
    // CREAR OUTPUT/TOKENS.TXT
    // =================================================

    std::ofstream archivoTokens(
        "output/tokens.txt"
    );

    for (const Token& token : tokens) {

        archivoTokens
            << "<"
            << token.getTipo()
            << "> "
            << token.getLexema()
            << "  (Linea: "
            << token.getLinea()
            << ", Columna: "
            << token.getColumna()
            << ")\n";
    }

    archivoTokens.close();


    // =================================================
    // CREAR TABLA DE SÍMBOLOS
    // =================================================

    std::ofstream archivoSimbolos(
        "output/tabla_simbolos.txt"
    );

    archivoSimbolos
        << "===== TABLA DE SIMBOLOS =====\n";

    lexer.getTablaSimbolos()
         .mostrar(archivoSimbolos);

    archivoSimbolos.close();


    // =================================================
    // CREAR ARCHIVO DE ERRORES
    // =================================================

    std::ofstream archivoErrores(
        "output/errores.txt"
    );

    archivoErrores
        << "===== ERRORES LEXICOS =====\n";


    const std::vector<std::string>& errores =
        lexer.getErrores();


    if (errores.empty()) {

        archivoErrores
            << "No se encontraron errores lexicos.\n";

    } else {

        for (const std::string& error : errores) {

            archivoErrores
                << error
                << std::endl;
        }
    }

    archivoErrores.close();


    // =================================================
    // RESUMEN
    // =================================================

    std::cout
        << "\nTotal de tokens: "
        << tokens.size()
        << std::endl;

    std::cout
        << "Total de errores: "
        << errores.size()
        << std::endl;

    std::cout
        << "\nArchivos generados en output/:\n"
        << "- tokens.txt\n"
        << "- tabla_simbolos.txt\n"
        << "- errores.txt\n";


    return 0;
}
