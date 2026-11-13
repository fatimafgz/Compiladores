#include "Lexer.h"
#include "parser.h"

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

    Parser parser(tokens);

    if (parser.analizar()) {
        std::cout << "Programa sintacticamente valido.\n";
        return 0;
    }

    std::cout << "Programa sintacticamente incorrecto:\n";
    for (const std::string& error : parser.getErrores()) {
        std::cout << "- " << error << '\n';
    }

    return 1;
}