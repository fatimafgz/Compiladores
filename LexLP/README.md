# LexLP — Analizador léxico y sintáctico

LexLP analiza programas escritos en LP. El analizador léxico convierte el código fuente en tokens y el analizador sintáctico comprueba si esos tokens cumplen la gramática del lenguaje. Cuando el programa es válido, se muestra su AST.

## Requisitos

- `g++` con soporte para C++17.
- Terminal UCRT64 de MSYS2 o una terminal compatible con `g++`.
- Ejecutar los comandos desde la carpeta raíz del proyecto.

## Organización de módulos

| Archivo | Responsabilidad |
|---|---|
| `src/main_sintactico.cpp` | Lee el archivo `.lp`, integra el lexer con el parser y muestra el resultado y el AST. |
| `src/Lexer.h` y `src/Lexer.cpp` | Analizan el texto fuente y generan tokens. También reportan errores léxicos. |
| `src/Token.h` y `src/Token.cpp` | Definen los tokens y sus datos: tipo, lexema, línea y columna. |
| `src/Parser.h` y `src/Parser.cpp` | Analizan los tokens mediante descenso recursivo, reportan errores sintácticos y construyen el AST. |
| `src/AST.h` | Define los nodos del AST y una función para imprimirlo. |
| `src/SymbolTable.h` y `src/SymbolTable.cpp` | Mantienen la tabla de identificadores producidos por el lexer. |
| `tests/` | Contiene programas `.lp` para probar el analizador. |
| `output/` | Carpeta destinada al ejecutable y a las salidas guardadas. |

## Compilación

Desde la raíz del proyecto, compila el analizador sintáctico con:

```bash
g++ -std=c++17 -Wall -Wextra -Isrc \
  src/main_sintactico.cpp src/Parser.cpp src/Lexer.cpp \
  src/Token.cpp src/SymbolTable.cpp -o output/LexLP.exe
```

## Ejecución

Indica el archivo `.lp` como argumento:

```bash
./output/LexLP.exe tests/prueba_sintactica.lp
```

Si no se proporciona una ruta, el programa intenta analizar `tests/prueba_sintactica.lp`.

## Ejemplo válido

```c
void main() {
    int edad;
    float promedio;
    edad = 20;
    promedio = 15.5;
    if (edad >= 18 && edad <= 60) {
        println("Edad valida");
    } else {
        println("Edad no valida");
    }
    while (edad > 0) {
        edad = edad - 1;
    }
    return edad;
}
```

Al aceptar un programa, el sistema muestra:

```text
Programa sintacticamente valido.
```

y luego imprime el AST.

# Integrantes

**Curso:** Compiladores

**Proyecto:** Implementación del Analizador Léxico: LexLP

**Lenguaje:** C++

**Repositorio:** GitHub

**Integrantes:**

* Fatima Florez Gonzalez - fflorezg@ulasalle.edu.pe
* Allison Mayra Usedo Quispe - ausedoq@ulasalle.edu.pe
   
---

