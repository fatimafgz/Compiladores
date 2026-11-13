# LexLP — Analizador sintáctico

Este proyecto integra el analizador léxico de LP con un analizador sintáctico por descenso recursivo. Corresponde al **entregable 1**: reconoce la estructura básica del programa, declaraciones de variables y asignaciones sencillas.

## Requisitos

- Compilador `g++` con soporte para C++17.
- Una terminal ubicada en la carpeta raíz del proyecto `LexLP`.

## Compilación

En una terminal UCRT64, ejecuta:

```bash
g++ -std=c++17 -Isrc src/main_sintactico.cpp src/Parser.cpp src/Lexer.cpp src/Token.cpp src/SymbolTable.cpp -o output/LexLP.exe
```

El ejecutable del analizador sintáctico se genera en `output/LexLP.exe`.

## Ejecución

Indica como argumento el archivo `.lp` que deseas analizar:

```bash
./output/LexLP.exe tests/prueba_sintactica.lp
```

Si no indicas un archivo, el programa utiliza `tests/prueba_sintactica.lp`:

```bash
./output/LexLP.exe
```

> Ejecuta `./output/LexLP.exe`, no `./LexLP.exe`. El segundo corresponde al ejecutable léxico anterior que se encuentra en la raíz del proyecto.

## Ejemplo de programa válido

```c
void main() {
    int edad;
    float promedio;

    edad = 20;
    promedio = 15.5;
    edad = edad + 2 * 3;
}
```

Resultado esperado:

```text
Programa sintacticamente valido.
```

## Ejemplo de programa inválido

```c
void main() {
    int edad
    edad = 20;
}
```

El analizador debe informar que falta el punto y coma al finalizar la declaración.

## Funcionalidades del entregable 1

- Lectura de un archivo fuente `.lp`.
- Obtención de tokens mediante el analizador léxico existente.
- Reconocimiento de `void main()` y bloques delimitados por llaves.
- Reconocimiento de declaraciones de variables simples.
- Reconocimiento de asignaciones con números, identificadores y operaciones aritméticas sencillas.
- Reporte de errores léxicos y sintácticos básicos.

Las funciones, estructuras de control, entrada y salida, AST y expresiones completas corresponden al entregable 2.

---

# 33. Integrantes

**Curso:** Compiladores

**Proyecto:** Implementación del Analizador Léxico: LexLP

**Lenguaje:** C++

**Repositorio:** GitHub

**Integrantes:**

* Fatima Florez Gonzalez - fflorezg@ulasalle.edu.pe
* Allison Mayra Usedo Quispe - ausedoq@ulasalle.edu.pe
   
---

