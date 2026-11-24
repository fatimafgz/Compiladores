#include "Lexer.h"
#include "Parser.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

int main(int argc, char* argv[]) {
    const std::string ruta =
        argc > 1 ? argv[1] : "tests/prueba_sintactica.lp";

    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir: " << ruta << '\n';
        return 1;
    }

    const std::string fuente(
        (std::istreambuf_iterator<char>(archivo)),
        std::istreambuf_iterator<char>()
    );

    Lexer lexer(fuente);
    const std::vector<Token> tokens = lexer.analizar();

    if (!lexer.getErrores().empty()) {
        std::cerr << "Se encontraron errores lexicos:\n";
        for (const std::string& error : lexer.getErrores()) {
            std::cerr << "- " << error << '\n';
        }
        return 1;
    }

    // Los comentarios son tokens del lexer, pero no forman parte de la gramática.
    std::vector<Token> tokensParser;
    for (const Token& token : tokens) {
        if (token.getTipo() != "COMENT") {
            tokensParser.push_back(token);
        }
    }

    Parser parser(tokensParser);

    if (!parser.analizar()) {
        std::cout << "Programa sintacticamente incorrecto:\n";

        for (const std::string& error : parser.getErrores()) {
            std::cout << "- " << error << '\n';
        }

        return 1;
    }

    std::cout << "Programa sintacticamente valido.\n";
    std::cout << "\nAST:\n";

    if (parser.getAST() != nullptr) {
        imprimirAST(*parser.getAST(), std::cout);
    }

    return 0;
}
