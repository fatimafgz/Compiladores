#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Lexer.h"
#include "Token.h"

int main() {

    std::string nombreArchivo = "tests/prueba_fase2.lp";

    std::ifstream archivo(nombreArchivo);

    if (!archivo.is_open()) {

        std::cerr << "Error: no se pudo abrir el archivo "
                  << nombreArchivo << std::endl;

        return 1;
    }

    std::stringstream buffer;
    buffer << archivo.rdbuf();

    std::string fuente = buffer.str();

    archivo.close();

    Lexer lexer(fuente);

    std::vector<Token> tokens = lexer.analizar();

    std::cout << "===== ANALIZADOR LEXICO LexLP =====\n\n";

    std::cout << "Lista de tokens:\n\n";

    for (const Token& token : tokens) {

        std::cout << "<"
                  << token.getTipo()
                  << "> "
                  << token.getLexema()
                  << "  (Linea: "
                  << token.getLinea()
                  << ", Columna: "
                  << token.getColumna()
                  << ")\n";
    }

    lexer.getTablaSimbolos().mostrar();

    std::cout << "\nTotal de tokens: "
              << tokens.size()
              << std::endl;

    return 0;
}