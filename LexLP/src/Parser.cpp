#include "Parser.h"

#include <sstream>
#include <utility>

Parser::Parser(const std::vector<Token>& entrada)
    : tokens(entrada), posicion(0) {
}

const Token& Parser::actual() const {
    static const Token fin("FIN", "", 0, 0);

    if (posicion >= tokens.size()) {
        return fin;
    }

    return tokens[posicion];
}

bool Parser::aceptar(const std::string& tipo) {
    if (actual().getTipo() == tipo) {
        ++posicion;
        return true;
    }
    return false;
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

bool Parser::esTipoBase(const Token& token) const {
    const std::string tipo = token.getTipo();
    return tipo == "INT" || tipo == "FLOAT" ||
           tipo == "CHAR" || tipo == "BOOLEAN";
}

bool Parser::esComparador(const Token& token) const {
    const std::string tipo = token.getTipo();
    const std::string lexema = token.getLexema();

    return tipo == "COMP" ||
           lexema == ">" || lexema == ">=" ||
           lexema == "<" || lexema == "<=" ||
           lexema == "!=" || lexema == "==";
}

bool Parser::esInicioComando(const Token& token) const {
    const std::string tipo = token.getTipo();

    return tipo == "ID" || tipo == "FUNC" ||
           tipo == "SCANF" || tipo == "PRINTLN" ||
           tipo == "IF" || tipo == "WHILE" ||
           tipo == "RETURN";
}

void Parser::registrarError(const std::string& mensaje) {
    std::ostringstream salida;

    salida << "Error sintactico en linea " << actual().getLinea()
           << ", columna " << actual().getColumna()
           << ": " << mensaje;

    if (actual().getTipo() == "FIN") {
        salida << ". Encontrado el fin del archivo";
    } else {
        salida << ". Encontrado: '" << actual().getLexema() << "'";
    }

    errores.push_back(salida.str());
}

void Parser::sincronizarComando() {
    while (actual().getTipo() != "FIN" &&
           actual().getTipo() != "}" &&
           actual().getTipo() != ";") {
        if (esInicioComando(actual())) {
            return;
        }
        ++posicion;
    }

    aceptar(";");
}

Parser::Nodo Parser::crearNodo(
    const std::string& tipo,
    const std::string& valor,
    const Token* token
) {
    if (token) {
        return std::make_unique<NodoAST>(
            tipo, valor, token->getLinea(), token->getColumna()
        );
    }

    return std::make_unique<NodoAST>(tipo, valor);
}

bool Parser::analizar() {
    errores.clear();
    posicion = 0;
    raiz.reset();

    raiz = PROGRAMA();

    return errores.empty() &&
           actual().getTipo() == "FIN" &&
           raiz != nullptr;
}

const std::vector<std::string>& Parser::getErrores() const {
    return errores;
}

const NodoAST* Parser::getAST() const {
    return raiz.get();
}

Parser::Nodo Parser::PROGRAMA() {
    Nodo nodo = crearNodo("Programa");

    nodo->agregar(LISTA_FUNCIONES());
    nodo->agregar(PRINCIPAL());

    if (actual().getTipo() != "FIN") {
        registrarError("no se esperaba contenido despues del programa");
    }

    return nodo;
}

Parser::Nodo Parser::LISTA_FUNCIONES() {
    Nodo lista = crearNodo("ListaFunciones");

    while (esTipoBase(actual()) || actual().getTipo() == "VOID") {
        // void main() es la función principal, no una función previa.
        if (actual().getTipo() == "VOID" &&
            posicion + 1 < tokens.size() &&
            tokens[posicion + 1].getTipo() == "MAIN") {
            break;
        }

        lista->agregar(DECLARACION_FUNCION());
    }

    return lista;
}

Parser::Nodo Parser::DECLARACION_FUNCION() {
    Nodo funcion = crearNodo("DeclaracionFuncion");

    if (actual().getTipo() == "VOID") {
        Token tipo = actual();
        ++posicion;
        funcion->agregar(crearNodo("TipoRetorno", "void", &tipo));
    } else {
        funcion->agregar(TIPO());
    }

    if (actual().getTipo() == "ID") {
        Token nombre = actual();
        ++posicion;
        funcion->agregar(crearNodo("Nombre", nombre.getLexema(), &nombre));
    } else {
        esperar("ID", "un identificador para la funcion");
    }

    esperar("(", "'('");
    funcion->agregar(PARAMETROS());
    esperar(")", "')'");
    funcion->agregar(BLOQUE());

    return funcion;
}

Parser::Nodo Parser::PRINCIPAL() {
    Nodo principal = crearNodo("Principal");

    esperar("VOID", "'void'");
    esperar("MAIN", "'main'");
    esperar("(", "'('");
    esperar(")", "')'");

    principal->agregar(BLOQUE());
    return principal;
}

Parser::Nodo Parser::BLOQUE() {
    Nodo bloque = crearNodo("Bloque");

    if (!esperar("{", "'{'")) {
        return bloque;
    }

    bloque->agregar(LISTA_VARIABLES());
    bloque->agregar(COMANDOS());
    esperar("}", "'}'");

    return bloque;
}

Parser::Nodo Parser::TIPO() {
    Nodo tipo = crearNodo("Tipo");

    if (esTipoBase(actual())) {
        Token base = actual();
        ++posicion;
        tipo->agregar(crearNodo("TipoBase", base.getLexema(), &base));
    } else {
        registrarError("se esperaba un tipo base");
        return tipo;
    }

    tipo->agregar(DIMENSION());
    return tipo;
}

Parser::Nodo Parser::DIMENSION() {
    Nodo dimensiones = crearNodo("Dimensiones");

    while (aceptar("[")) {
        Nodo dimension = crearNodo("Dimension");

        if (actual().getTipo() == "NUM_INT") {
            Token tamano = actual();
            ++posicion;
            dimension->agregar(
                crearNodo("Entero", tamano.getLexema(), &tamano)
            );
        } else {
            esperar("NUM_INT", "un tamaño entero para el arreglo");
        }

        esperar("]", "']'");
        dimensiones->agregar(std::move(dimension));
    }

    return dimensiones;
}

Parser::Nodo Parser::PARAMETROS() {
    Nodo parametros = crearNodo("Parametros");

    if (!esTipoBase(actual())) {
        return parametros;
    }

    do {
        Nodo parametro = crearNodo("Parametro");
        parametro->agregar(TIPO());

        if (actual().getTipo() == "ID") {
            Token nombre = actual();
            ++posicion;
            parametro->agregar(
                crearNodo("Identificador", nombre.getLexema(), &nombre)
            );
        } else {
            esperar("ID", "un identificador de parametro");
        }

        parametros->agregar(std::move(parametro));
    } while (aceptar(","));

    return parametros;
}

Parser::Nodo Parser::LISTA_VARIABLES() {
    Nodo variables = crearNodo("Declaraciones");

    while (esTipoBase(actual())) {
        variables->agregar(DECLARACION());
    }

    return variables;
}

Parser::Nodo Parser::DECLARACION() {
    Nodo declaracion = crearNodo("DeclaracionVariable");
    declaracion->agregar(TIPO());

    if (actual().getTipo() == "ID") {
        Token nombre = actual();
        ++posicion;

        Nodo identificadores = crearNodo("Identificadores");
        identificadores->agregar(
            crearNodo("Identificador", nombre.getLexema(), &nombre)
        );

        while (aceptar(",")) {
            if (actual().getTipo() == "ID") {
                Token otro = actual();
                ++posicion;
                identificadores->agregar(
                    crearNodo("Identificador", otro.getLexema(), &otro)
                );
            } else {
                esperar("ID", "un identificador despues de ','");
                break;
            }
        }

        declaracion->agregar(std::move(identificadores));
    } else {
        esperar("ID", "un identificador");
    }

    if (!esperar(";", "';' al final de la declaracion")) {
        sincronizarComando();
    }

    return declaracion;
}

Parser::Nodo Parser::COMANDOS() {
    Nodo comandos = crearNodo("Comandos");

    while (actual().getTipo() != "}" &&
           actual().getTipo() != "FIN") {
        const std::size_t antes = posicion;
        comandos->agregar(COMANDO());

        if (posicion == antes && actual().getTipo() != "FIN") {
            ++posicion;
        }
    }

    return comandos;
}

Parser::Nodo Parser::COMANDO() {
    const std::string tipo = actual().getTipo();

    if (tipo == "ID") return ASIGNACION();
    if (tipo == "FUNC") return FUNCION();
    if (tipo == "SCANF") return LECTURA();
    if (tipo == "PRINTLN") return ESCRITURA();
    if (tipo == "IF") return SELECCION();
    if (tipo == "WHILE") return MIENTRAS();
    if (tipo == "RETURN") return RETORNO();

    registrarError("se esperaba un comando");
    sincronizarComando();
    return crearNodo("ErrorComando");
}

Parser::Nodo Parser::LECTURA() {
    Token inicio = actual();
    aceptar("SCANF");

    Nodo lectura = crearNodo("Lectura", "", &inicio);
    esperar("(", "'('");
    lectura->agregar(TERMINO_LECTURA());

    while (aceptar(",")) {
        lectura->agregar(TERMINO_LECTURA());
    }

    esperar(")", "')'");
    esperar(";", "';' despues de scanf");
    return lectura;
}

Parser::Nodo Parser::TERMINO_LECTURA() {
    Nodo termino = crearNodo("TerminoLectura");

    if (actual().getTipo() == "ID") {
        Token id = actual();
        ++posicion;
        termino->agregar(crearNodo("Identificador", id.getLexema(), &id));
        termino->agregar(DIMENSION2());
    } else {
        esperar("ID", "un identificador en scanf");
    }

    return termino;
}

Parser::Nodo Parser::ESCRITURA() {
    Token inicio = actual();
    aceptar("PRINTLN");

    Nodo escritura = crearNodo("Escritura", "", &inicio);
    esperar("(", "'('");

    if (actual().getTipo() != ")") {
        escritura->agregar(TERMINO_ESCRITURA());

        while (aceptar(",")) {
            escritura->agregar(TERMINO_ESCRITURA());
        }
    }

    esperar(")", "')'");
    esperar(";", "';' despues de println");
    return escritura;
}

Parser::Nodo Parser::TERMINO_ESCRITURA() {
    if (actual().getTipo() == "ID") {
        Token id = actual();
        ++posicion;

        Nodo termino = crearNodo("Identificador", id.getLexema(), &id);
        termino->agregar(DIMENSION2());
        return termino;
    }

    if (actual().getTipo() == "TEXTO") {
        Token texto = actual();
        ++posicion;
        return crearNodo("Texto", texto.getLexema(), &texto);
    }

    if (actual().getTipo() == "NUM_INT" ||
        actual().getTipo() == "NUM_DEC") {
        return CONSTANTE();
    }

    registrarError("se esperaba identificador, constante o texto en println");

    if (actual().getTipo() != "FIN") {
        ++posicion;
    }

    return crearNodo("ErrorTermino");
}

Parser::Nodo Parser::ASIGNACION() {
    Token id = actual();
    esperar("ID", "un identificador");

    Nodo asignacion = crearNodo("Asignacion", id.getLexema(), &id);
    esperar("=", "'='");
    asignacion->agregar(COMPLEMENTO());
    esperar(";", "';' al final de la asignacion");

    return asignacion;
}

Parser::Nodo Parser::COMPLEMENTO() {
    if (actual().getTipo() == "FUNC") {
        return FUNCION();
    }

    return EXPRESION();
}

Parser::Nodo Parser::FUNCION() {
    Token inicio = actual();
    esperar("FUNC", "'func'");

    Nodo llamada = crearNodo("LlamadaFuncion", "", &inicio);

    if (actual().getTipo() == "ID") {
        Token nombre = actual();
        ++posicion;
        llamada->agregar(
            crearNodo("Nombre", nombre.getLexema(), &nombre)
        );
    } else {
        esperar("ID", "un nombre de funcion");
    }

    esperar("(", "'('");
    llamada->agregar(ARGUMENTOS());
    esperar(")", "')'");

    return llamada;
}

Parser::Nodo Parser::ARGUMENTOS() {
    Nodo argumentos = crearNodo("Argumentos");

    if (actual().getTipo() == ")") {
        return argumentos;
    }

    argumentos->agregar(EXPRESION());

    while (aceptar(",")) {
        argumentos->agregar(EXPRESION());
    }

    return argumentos;
}

Parser::Nodo Parser::SELECCION() {
    Token inicio = actual();
    aceptar("IF");

    Nodo seleccion = crearNodo("Seleccion", "if", &inicio);

    esperar("(", "'('");
    seleccion->agregar(EXPRESION());
    esperar(")", "')'");
    seleccion->agregar(BLOQUE());

    if (aceptar("ELSE")) {
        seleccion->agregar(BLOQUE());
    }

    return seleccion;
}

Parser::Nodo Parser::MIENTRAS() {
    Token inicio = actual();
    aceptar("WHILE");

    Nodo mientras = crearNodo("Mientras", "", &inicio);
    esperar("(", "'('");
    mientras->agregar(EXPRESION());
    esperar(")", "')'");
    mientras->agregar(BLOQUE());

    return mientras;
}

Parser::Nodo Parser::RETORNO() {
    Token inicio = actual();
    aceptar("RETURN");

    Nodo retorno = crearNodo("Retorno", "", &inicio);
    retorno->agregar(EXPRESION());
    esperar(";", "';' despues de return");

    return retorno;
}

Parser::Nodo Parser::DIMENSION2() {
    Nodo indices = crearNodo("Indices");

    while (aceptar("[")) {
        Nodo indice = crearNodo("Indice");
        indice->agregar(EXPRESION_ADITIVA());
        esperar("]", "']'");
        indices->agregar(std::move(indice));
    }

    return indices;
}

Parser::Nodo Parser::EXPRESION() {
    return EXPRESION_O();
}

Parser::Nodo Parser::EXPRESION_O() {
    Nodo izquierda = EXPRESION_Y();

    while (actual().getTipo() == "||") {
        Token op = actual();
        ++posicion;

        Nodo nuevo = crearNodo("Binario", op.getLexema(), &op);
        nuevo->agregar(std::move(izquierda));
        nuevo->agregar(EXPRESION_Y());
        izquierda = std::move(nuevo);
    }

    return izquierda;
}

Parser::Nodo Parser::EXPRESION_Y() {
    Nodo izquierda = EXPRESION_RELACIONAL();

    while (actual().getTipo() == "&&") {
        Token op = actual();
        ++posicion;

        Nodo nuevo = crearNodo("Binario", op.getLexema(), &op);
        nuevo->agregar(std::move(izquierda));
        nuevo->agregar(EXPRESION_RELACIONAL());
        izquierda = std::move(nuevo);
    }

    return izquierda;
}

Parser::Nodo Parser::EXPRESION_RELACIONAL() {
    Nodo izquierda = EXPRESION_ADITIVA();

    if (esComparador(actual())) {
        Token op = actual();
        ++posicion;

        Nodo nuevo = crearNodo("Binario", op.getLexema(), &op);
        nuevo->agregar(std::move(izquierda));
        nuevo->agregar(EXPRESION_ADITIVA());
        izquierda = std::move(nuevo);
    }

    return izquierda;
}

Parser::Nodo Parser::EXPRESION_ADITIVA() {
    Nodo izquierda = EXPRESION_MULTIPLICATIVA();

    while (actual().getTipo() == "+" || actual().getTipo() == "-") {
        Token op = actual();
        ++posicion;

        Nodo nuevo = crearNodo("Binario", op.getLexema(), &op);
        nuevo->agregar(std::move(izquierda));
        nuevo->agregar(EXPRESION_MULTIPLICATIVA());
        izquierda = std::move(nuevo);
    }

    return izquierda;
}

Parser::Nodo Parser::EXPRESION_MULTIPLICATIVA() {
    Nodo izquierda = FACTOR();

    while (actual().getTipo() == "*" ||
           actual().getTipo() == "/" ||
           actual().getTipo() == "%") {
        Token op = actual();
        ++posicion;

        Nodo nuevo = crearNodo("Binario", op.getLexema(), &op);
        nuevo->agregar(std::move(izquierda));
        nuevo->agregar(FACTOR());
        izquierda = std::move(nuevo);
    }

    return izquierda;
}

Parser::Nodo Parser::FACTOR() {
    if (actual().getTipo() == "+" ||
        actual().getTipo() == "-" ||
        actual().getTipo() == "!") {
        Token op = actual();
        ++posicion;

        Nodo unario = crearNodo("Unario", op.getLexema(), &op);
        unario->agregar(FACTOR());
        return unario;
    }

    if (aceptar("(")) {
        Nodo expresion = EXPRESION();
        esperar(")", "')'");
        return expresion;
    }

    if (actual().getTipo() == "TEXTO") {
        Token texto = actual();
        ++posicion;
        return crearNodo("Texto", texto.getLexema(), &texto);
    }

    return TERMINO();
}

Parser::Nodo Parser::TERMINO() {
    if (actual().getTipo() == "ID") {
        Token id = actual();
        ++posicion;

        Nodo nodo = crearNodo("Identificador", id.getLexema(), &id);
        nodo->agregar(DIMENSION2());
        return nodo;
    }

    return CONSTANTE();
}

Parser::Nodo Parser::CONSTANTE() {
    if (actual().getTipo() == "NUM_INT") {
        Token numero = actual();
        ++posicion;
        return crearNodo("Entero", numero.getLexema(), &numero);
    }

    if (actual().getTipo() == "NUM_DEC") {
        Token numero = actual();
        ++posicion;
        return crearNodo("Decimal", numero.getLexema(), &numero);
    }

    registrarError("se esperaba una constante numerica");

    if (actual().getTipo() != "FIN") {
        ++posicion;
    }

    return crearNodo("ErrorConstante");
}