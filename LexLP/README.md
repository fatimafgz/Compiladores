# Implementación del Analizador Léxico: LexLP

## 1. Descripción del proyecto

**LexLP** es un analizador léxico desarrollado en **C++** para reconocer los diferentes elementos que forman parte del lenguaje de programación **LP**.

El analizador recibe como entrada un archivo con extensión `.lp`, analiza su contenido carácter por carácter y reconoce los diferentes componentes léxicos del lenguaje.

El proyecto fue desarrollado progresivamente en **tres fases**, de acuerdo con las actividades indicadas para el curso de **Compiladores**:

* **Fase 1:** Lectura del archivo y reconocimiento de números enteros y decimales.
* **Fase 2:** Reconocimiento de identificadores, textos, palabras reservadas y tabla de símbolos.
* **Fase 3:** Implementación de operadores, símbolos, comentarios, integración completa, generación de la lista de tokens y manejo de errores léxicos.

Al finalizar, el analizador permite obtener:

* Una lista secuencial de tokens.
* El tipo de cada token.
* El lexema reconocido.
* La línea donde aparece.
* La columna donde aparece.
* Una tabla de símbolos para los identificadores.
* Un archivo con los errores léxicos encontrados.

---

# 2. Objetivo

## Objetivo general

Implementar un **analizador léxico en C++ para el lenguaje LP**, capaz de reconocer los tokens definidos mediante expresiones regulares, generar una lista secuencial de tokens, mantener una tabla de símbolos con identificadores únicos y reportar errores léxicos.

## Objetivos específicos

* Leer un archivo fuente `.lp`.
* Analizar el contenido carácter por carácter.
* Reconocer números enteros y decimales.
* Reconocer identificadores.
* Reconocer constantes de texto.
* Reconocer palabras reservadas.
* Construir una tabla de símbolos.
* Reconocer operadores aritméticos, lógicos, de asignación y comparación.
* Reconocer símbolos especiales del lenguaje.
* Reconocer comentarios.
* Registrar errores léxicos.
* Generar archivos de salida con los resultados del análisis.
* Integrar las tres fases en un solo analizador léxico.

---

# 3. Tecnologías utilizadas

| Tecnología         | Uso                                       |
| ------------------ | ----------------------------------------- |
| C++                | Lenguaje de programación utilizado        |
| Visual Studio Code | Entorno de desarrollo                     |
| MSYS2 / GCC        | Compilación del proyecto                  |
| Git                | Control de versiones                      |
| GitHub             | Almacenamiento y seguimiento del proyecto |
| Archivos `.lp`     | Archivos utilizados como entrada          |

---

# 4. Estructura del proyecto

El proyecto está organizado de la siguiente manera:

```text
LexLP/
│
├── LexLP.exe
├── README.md
│
├── output/
│   ├── tokens.txt
│   ├── tabla_simbolos.txt
│   └── errores.txt
│
├── src/
│   ├── Lexer.cpp
│   ├── Lexer.h
│   ├── main.cpp
│   ├── SymbolTable.cpp
│   ├── SymbolTable.h
│   ├── Token.cpp
│   └── Token.h
│
└── tests/
    ├── prueba_fase1.lp
    ├── prueba_fase2.lp
    ├── prueba_fase3.lp
    └── prueba_errores.lp
```

## Descripción de los archivos principales

### `main.cpp`

Es el punto de inicio del programa.

Se encarga de:

1. Iniciar el analizador.
2. Seleccionar el archivo `.lp` que será analizado.
3. Ejecutar el análisis léxico.
4. Mostrar los tokens obtenidos.
5. Mostrar la cantidad de errores encontrados.
6. Generar los archivos de salida.

### `Lexer.h`

Contiene la declaración de la clase `Lexer` y de las funciones utilizadas para realizar el análisis léxico.

### `Lexer.cpp`

Contiene la implementación principal del analizador.

Aquí se realiza el reconocimiento de:

* números;
* identificadores;
* palabras reservadas;
* textos;
* operadores;
* símbolos;
* comentarios;
* errores léxicos.

### `Token.h`

Define la estructura de un token.

Cada token contiene información como:

* tipo;
* lexema;
* línea;
* columna.

### `Token.cpp`

Contiene la implementación relacionada con los objetos `Token`.

### `SymbolTable.h`

Define la estructura utilizada para manejar la tabla de símbolos.

### `SymbolTable.cpp`

Implementa el almacenamiento y manejo de los identificadores encontrados durante el análisis.

### `tests/`

Contiene los archivos `.lp` utilizados para comprobar el funcionamiento del analizador durante las diferentes fases.

### `output/`

Contiene los resultados generados por el programa:

```text
tokens.txt
tabla_simbolos.txt
errores.txt
```

---

# 5. Funcionamiento general del analizador

El funcionamiento general de LexLP se puede representar de la siguiente manera:

```text
                 ARCHIVO .LP
                      │
                      ▼
                  main.cpp
                      │
                      ▼
                    LEXER
                      │
          ┌───────────┼───────────┐
          │           │           │
          ▼           ▼           ▼
       Números     IDs/TEXTO   Operadores
          │           │           │
          └───────────┼───────────┘
                      │
                      ▼
                Lista de Tokens
                      │
             ┌────────┴────────┐
             │                 │
             ▼                 ▼
      Tabla de símbolos     Errores
             │                 │
             └────────┬────────┘
                      ▼
                   OUTPUT
```

El analizador recorre el archivo desde el primer carácter hasta el último.

En cada posición determina qué tipo de elemento comienza allí y llama a la función correspondiente.

Por ejemplo:

```text
int edad = 20;
```

se analiza como:

```text
int       → INT
edad      → ID
=         → =
20        → NUM_INT
;         → ;
```

---

# 6. FASE 1 — Lectura, números enteros y números decimales

## 6.1 Objetivo de la Fase 1

La primera fase tuvo como objetivo implementar la lectura del archivo fuente y reconocer los primeros tokens definidos para el lenguaje LP.

Los primeros elementos implementados fueron:

* `NUM_INT`
* `NUM_DEC`

Además, se implementó el recorrido carácter por carácter del archivo.

---

## 6.2 Lectura del archivo

El analizador recibe un archivo con extensión `.lp`.

Por ejemplo:

```text
20
15.5
100
3.14
```

El `Lexer` recorre cada carácter y determina si pertenece a un número.

Para realizar el recorrido se mantiene una posición dentro de la cadena y también se controla:

* línea actual;
* columna actual.

Esto permite indicar posteriormente exactamente dónde fue encontrado cada token.

---

## 6.3 Reconocimiento de números enteros

La expresión regular indicada para los números enteros fue:

```text
D = [0-9]

NUM_INT = D+
```

Esto significa que un número entero está formado por uno o más dígitos.

Ejemplos válidos:

```text
0
7
20
100
999
```

Por ejemplo:

```text
20
```

se convierte en:

```text
<NUM_INT> 20
```

---

## 6.4 Reconocimiento de números decimales

La expresión regular indicada fue:

```text
NUM_DEC = D+\.D+
```

Esto representa un número formado por:

```text
uno o más dígitos
+
punto decimal
+
uno o más dígitos
```

Ejemplos válidos:

```text
15.5
3.14
1.0
25.75
999.99
```

Por ejemplo:

```text
15.5
```

produce:

```text
<NUM_DEC> 15.5
```

---

## 6.5 Control de posición

Desde esta fase se implementó el control de:

```text
Línea
Columna
```

Esto permite obtener resultados como:

```text
<NUM_INT> 20  (Linea: 5, Columna: 8)
```

De esta manera no solamente se conoce qué token fue encontrado, sino también su ubicación dentro del archivo fuente.

---

## 6.6 Prueba de la Fase 1

Se creó el archivo:

```text
tests/prueba_fase1.lp
```

Este archivo fue utilizado para comprobar el reconocimiento de números enteros y decimales.

La salida permitió comprobar que los números eran clasificados correctamente como:

```text
NUM_INT
NUM_DEC
```

---

# 7. FASE 2 — Identificadores, textos, palabras reservadas y tabla de símbolos

## 7.1 Objetivo de la Fase 2

En la segunda fase se amplió el analizador para reconocer los elementos relacionados con variables y estructuras básicas del lenguaje.

Se implementaron:

* identificadores;
* constantes de texto;
* palabras reservadas;
* tabla de símbolos.

---

# 7.2 Reconocimiento de identificadores

La expresión regular definida para los identificadores fue:

```text
L = [a-zA-Z_]

ID = L(L|D)*
```

Esto significa que un identificador:

1. Debe comenzar con una letra o `_`.
2. Después puede contener letras.
3. También puede contener números.
4. Puede contener `_`.

Ejemplos:

```text
edad
promedio
nombre1
_variable
alumno123
```

Por ejemplo:

```text
edad
```

produce:

```text
<ID> edad
```

---

# 7.3 Reconocimiento de textos

Para las constantes de texto se utilizó la expresión:

```text
TEXTO = ".*"
```

Esto permite reconocer texto encerrado entre comillas dobles.

Ejemplo:

```text
"Hola mundo"
```

se reconoce como:

```text
<TEXTO> "Hola mundo"
```

También se implementó el control de errores para detectar textos que no tengan correctamente la comilla de cierre.

Por ejemplo:

```text
"Texto sin cerrar
```

debe ser reportado como un error léxico.

---

# 7.4 Palabras reservadas

Se implementó el reconocimiento de las palabras reservadas del lenguaje LP.

Las palabras reservadas utilizadas son:

| Palabra   | Token   |
| --------- | ------- |
| `int`     | INT     |
| `float`   | FLOAT   |
| `char`    | CHAR    |
| `boolean` | BOOLEAN |
| `void`    | VOID    |
| `if`      | IF      |
| `else`    | ELSE    |
| `for`     | FOR     |
| `while`   | WHILE   |
| `scanf`   | SCANF   |
| `println` | PRINTLN |
| `main`    | MAIN    |
| `return`  | RETURN  |

Por ejemplo:

```text
int
```

produce:

```text
<INT> int
```

Mientras que:

```text
edad
```

produce:

```text
<ID> edad
```

La diferencia se realiza cuando el analizador reconoce una palabra completa y verifica si pertenece al conjunto de palabras reservadas.

---

# 7.5 Tabla de símbolos

En la Fase 2 también se implementó la **tabla de símbolos**.

La tabla permite registrar los identificadores encontrados durante el análisis.

Por ejemplo, si el programa contiene:

```text
int edad;
float promedio;
boolean activo;
```

la tabla puede registrar:

```text
edad
promedio
activo
```

Los identificadores reconocidos como palabras reservadas no se agregan como identificadores normales.

Por ejemplo:

```text
int edad;
```

produce:

```text
INT  → int
ID   → edad
```

Solamente `edad` corresponde a un identificador.

La tabla de símbolos evita registrar repetidamente el mismo identificador.

Por ejemplo, si aparece:

```text
edad = 20;
edad = edad + 1;
```

`edad` continúa siendo el mismo identificador.

---

# 7.6 Prueba de la Fase 2

Para esta fase se utilizó:

```text
tests/prueba_fase2.lp
```

La prueba permitió comprobar:

* palabras reservadas;
* identificadores;
* textos;
* tabla de símbolos.

De esta forma se verificó que el analizador ya podía reconocer elementos básicos de un programa LP.

---

# 8. FASE 3 — Integración completa del analizador

## 8.1 Objetivo de la Fase 3

La tercera fase corresponde a la integración de las funcionalidades desarrolladas anteriormente.

En esta etapa se implementaron:

* operadores;
* símbolos;
* operadores de comparación;
* operadores lógicos;
* comentarios;
* lista completa de tokens;
* manejo de errores;
* integración de las fases anteriores;
* pruebas del analizador.

El objetivo fue obtener un analizador léxico funcional para una entrada completa del lenguaje LP.

---

# 9. Operadores implementados

## 9.1 Operador de asignación

Se reconoce:

```text
=
```

Ejemplo:

```text
edad = 20;
```

produce:

```text
<ID> edad
<=> =
<NUM_INT> 20
<;> ;
```

---

## 9.2 Operadores aritméticos

Se implementaron:

```text
+
-
*
/
%
```

Ejemplo:

```text
edad = edad + 1;
```

produce:

```text
<ID> edad
<=> =
<ID> edad
<+> +
<NUM_INT> 1
<;> ;
```

---

# 10. Operadores lógicos

Se implementaron:

```text
&&
||
!
```

Estos operadores permiten representar operaciones lógicas dentro del lenguaje.

Por ejemplo:

```text
activo && edad
```

se reconoce mediante:

```text
<ID> activo
<&&> &&
<ID> edad
```

---

# 11. Operadores de comparación

Los operadores relacionales definidos para el lenguaje son:

```text
>
>=
<
<=
!=
==
```

Estos operadores se utilizan para comparar valores.

Ejemplo:

```text
edad >= 18
```

produce:

```text
<ID> edad
<COMP> >=
<NUM_INT> 18
```

La categoría utilizada para estos operadores es:

```text
COMP
```

---

# 12. Símbolos especiales

También se implementó el reconocimiento de los símbolos utilizados para estructurar las instrucciones.

| Símbolo | Token |
| ------- | ----- |
| `(`     | `(`   |
| `)`     | `)`   |
| `[`     | `[`   |
| `]`     | `]`   |
| `{`     | `{`   |
| `}`     | `}`   |
| `,`     | `,`   |
| `;`     | `;`   |

Por ejemplo:

```text
if (edad > 18) {
```

contiene:

```text
IF
(
ID
COMP
NUM_INT
)
{
```

---

# 13. Reconocimiento de comentarios

También se implementó el reconocimiento de comentarios de una sola línea.

La expresión utilizada es:

```text
//.*
```

Ejemplo:

```text
// Este es un comentario
```

se reconoce como un comentario:

```text
<COMENT> // Este es un comentario
```

Esto permite que el analizador identifique el comentario y no lo confunda con operadores separados.

---

# 14. Lista de tokens

Una de las funciones principales solicitadas para el proyecto fue generar una **lista secuencial de tokens**.

Cada token contiene:

* tipo;
* lexema;
* línea;
* columna.

Ejemplo:

```text
<INT> int  (Linea: 1, Columna: 1)
<ID> edad  (Linea: 1, Columna: 5)
<;> ;  (Linea: 1, Columna: 9)
```

Otro ejemplo:

```text
<IF> if  (Linea: 8, Columna: 1)
<(> (  (Linea: 8, Columna: 4)
<ID> edad  (Linea: 8, Columna: 5)
<COMP> >=  (Linea: 8, Columna: 10)
<NUM_INT> 18  (Linea: 8, Columna: 13)
<)> )  (Linea: 8, Columna: 15)
<{> {  (Linea: 8, Columna: 17)
```

De esta manera, la lista mantiene el mismo orden en que aparecen los elementos dentro del código fuente.

---

# 15. Manejo de errores léxicos

El analizador también fue preparado para detectar caracteres o estructuras que no corresponden a los tokens definidos.

Cuando se encuentra un elemento no válido, se registra un error indicando su ubicación.

Algunos ejemplos de casos que pueden producir errores son:

### Número decimal incorrecto

```text
20.
```

### Número mal formado

```text
15.5.7
```

### Carácter desconocido

```text
@20
```

### Texto sin cerrar

```text
"Texto sin cerrar
```

### Símbolo no reconocido

```text
$valor
```

El objetivo es que el programa no solamente reconozca entradas correctas, sino que también pueda informar cuando encuentra una entrada que no pertenece al lenguaje definido.

---

# 16. Pruebas de errores

Para probar el manejo de errores se creó:

```text
tests/prueba_errores.lp
```

Este archivo contiene diferentes situaciones incorrectas para comprobar que el analizador pueda detectarlas.

Ejemplo:

```text
int edad;

edad = 20.;

edad = 15.5.7;

edad = @20;

println("Texto sin cerrar);

123abc;

$valor;
```

Estas pruebas permiten comprobar diferentes tipos de errores:

* números mal formados;
* caracteres no reconocidos;
* textos sin cerrar;
* estructuras que no corresponden a los patrones definidos.

---

# 17. Prueba de integración

Para comprobar la integración de todas las fases se utilizó:

```text
tests/prueba_fase3.lp
```

Esta prueba contiene diferentes elementos del lenguaje:

```text
int edad;
float promedio;
char letra;
boolean activo;
void main;

edad = 20;
promedio = 15.5;
letra = "A";

if (edad > 18) {
    println("Mayor de edad");
} else {
    println("Menor de edad");
}

for (edad = 0; edad <= 10; edad = edad + 1) {
    scanf(edad);
}

while (activo == true) {
    edad = edad - 1;
}

return;
```

Esta entrada permite comprobar en conjunto:

* palabras reservadas;
* identificadores;
* números enteros;
* números decimales;
* textos;
* operadores;
* comparadores;
* operadores de asignación;
* símbolos;
* estructuras de control;
* comentarios;
* tabla de símbolos.

---

# 18. Expresiones regulares implementadas

Las principales expresiones regulares utilizadas en el analizador son:

| Expresión                | Token     | Categoría          |
| ------------------------ | --------- | ------------------ |
| `[0-9]+`                 | `NUM_INT` | Número entero      |
| `[0-9]+\.[0-9]+`         | `NUM_DEC` | Número decimal     |
| `[a-zA-Z_][a-zA-Z_0-9]*` | `ID`      | Identificador      |
| `".*"`                   | `TEXTO`   | Constante de texto |
| `//.*`                   | `COMENT`  | Comentario         |

Las palabras reservadas son reconocidas después de identificar una palabra completa y verificar si pertenece al conjunto de palabras reservadas.

---

# 19. Tabla general de tokens

La siguiente tabla resume los tokens implementados:

| Lexema / expresión               | Token     | Categoría          |
| -------------------------------- | --------- | ------------------ |
| `[0-9]+`                         | `NUM_INT` | Número entero      |
| `[0-9]+\.[0-9]+`                 | `NUM_DEC` | Número decimal     |
| `[a-zA-Z_][a-zA-Z_0-9]*`         | `ID`      | Identificador      |
| `"..."`                          | `TEXTO`   | Constante de texto |
| `int`                            | `INT`     | Palabra reservada  |
| `float`                          | `FLOAT`   | Palabra reservada  |
| `char`                           | `CHAR`    | Palabra reservada  |
| `boolean`                        | `BOOLEAN` | Palabra reservada  |
| `void`                           | `VOID`    | Palabra reservada  |
| `if`                             | `IF`      | Palabra reservada  |
| `else`                           | `ELSE`    | Palabra reservada  |
| `for`                            | `FOR`     | Palabra reservada  |
| `while`                          | `WHILE`   | Palabra reservada  |
| `scanf`                          | `SCANF`   | Palabra reservada  |
| `println`                        | `PRINTLN` | Palabra reservada  |
| `main`                           | `MAIN`    | Palabra reservada  |
| `return`                         | `RETURN`  | Palabra reservada  |
| `//...`                          | `COMENT`  | Comentario         |
| `=`                              | `=`       | Asignación         |
| `+`                              | `+`       | Aritmético         |
| `-`                              | `-`       | Aritmético         |
| `*`                              | `*`       | Aritmético         |
| `/`                              | `/`       | Aritmético         |
| `%`                              | `%`       | Aritmético         |
| `&&`                             | `&&`      | Lógico             |
| `\|\|`                           | `\|\|`    | Lógico             |
| `!`                              | `!`       | Lógico             |
| `>`, `>=`, `<`, `<=`, `!=`, `==` | `COMP`    | Comparación        |
| `(`                              | `(`       | Símbolo            |
| `)`                              | `)`       | Símbolo            |
| `[`                              | `[`       | Símbolo            |
| `]`                              | `]`       | Símbolo            |
| `{`                              | `{`       | Símbolo            |
| `}`                              | `}`       | Símbolo            |
| `,`                              | `,`       | Símbolo            |
| `;`                              | `;`       | Símbolo            |

---

# 20. Tabla de símbolos

La tabla de símbolos almacena los identificadores reconocidos durante el análisis.

Por ejemplo, para:

```text
int edad;
float promedio;
boolean activo;
```

se identifican:

```text
edad
promedio
activo
```

La tabla permite mantener información de los identificadores encontrados y evitar que un mismo identificador sea registrado nuevamente.

Los identificadores pueden aparecer varias veces en el código fuente, pero conceptualmente representan el mismo elemento dentro de la tabla.

---

# 21. Archivos de salida

Después de ejecutar el analizador se generan archivos dentro de la carpeta:

```text
output/
```

## `tokens.txt`

Contiene la lista de tokens generados durante el análisis.

## `tabla_simbolos.txt`

Contiene los identificadores registrados en la tabla de símbolos.

## `errores.txt`

Contiene los errores léxicos detectados durante el análisis.

De esta manera, los resultados no solamente se muestran por consola, sino que también quedan almacenados en archivos.

---

# 22. Compilación del proyecto

Para compilar el proyecto utilizando `g++`, se ejecuta desde la carpeta `LexLP`:

```powershell
g++ src/main.cpp src/Lexer.cpp src/Token.cpp src/SymbolTable.cpp -o LexLP.exe
```

Si la compilación es correcta, se genera:

```text
LexLP.exe
```

---

# 23. Ejecución del programa

Para ejecutar el analizador:

```powershell
.\LexLP.exe
```

El programa muestra en la consola:

```text
===== ANALIZADOR LEXICO LexLP =====

LISTA DE TOKENS:
```

y posteriormente muestra los tokens encontrados.

Al finalizar también se muestra información como:

```text
Total de tokens: ...
Total de errores: ...
```

Además, se generan los archivos correspondientes dentro de:

```text
output/
```

---

# 24. Ejemplo de resultado

Para una entrada como:

```text
int edad;

edad = 20;

if (edad >= 18) {
    println("Mayor de edad");
}
```

el analizador reconoce elementos como:

```text
<INT> int
<ID> edad
<;> ;

<ID> edad
<=> =
<NUM_INT> 20
<;> ;

<IF> if
<(> (
<ID> edad
<COMP> >=
<NUM_INT> 18
<)> )
<{> {

<PRINTLN> println
<(> (
<TEXTO> "Mayor de edad"
<)> )
<;> ;

<}> }
```

Cada token incluye también su línea y columna correspondiente.

---

# 25. Integración de las tres fases

El proyecto fue desarrollado de manera progresiva.

### Fase 1

Se implementó:

```text
Lectura del archivo
       ↓
Números enteros
       ↓
Números decimales
```

### Fase 2

Se agregó:

```text
Fase 1
  +
Identificadores
  +
Textos
  +
Palabras reservadas
  +
Tabla de símbolos
```

### Fase 3

Finalmente se incorporó:

```text
Fase 2
  +
Operadores
  +
Símbolos
  +
Comparadores
  +
Operadores lógicos
  +
Comentarios
  +
Errores
  +
Lista completa de tokens
```

El resultado final es un analizador léxico integrado.

---

# 26. Flujo completo del análisis

El proceso realizado por LexLP puede resumirse en los siguientes pasos:

```text
1. Leer archivo .lp
        ↓
2. Inicializar Lexer
        ↓
3. Recorrer los caracteres
        ↓
4. Identificar el tipo de elemento
        ↓
5. Reconocer el token
        ↓
6. Registrar línea y columna
        ↓
7. Agregar identificadores a la tabla de símbolos
        ↓
8. Registrar errores cuando corresponda
        ↓
9. Generar lista de tokens
        ↓
10. Generar archivos de salida
```

---

# 27. Manejo de espacios y saltos de línea

Los espacios, tabulaciones y saltos de línea se utilizan para separar los diferentes elementos del código.

El analizador los procesa para continuar con el siguiente token.

Los saltos de línea también son importantes porque permiten actualizar el número de línea y mantener correctamente la posición del token.

Por ejemplo:

```text
int edad;
float promedio;
```

permite identificar que:

```text
int       → Línea 1
edad      → Línea 1
float     → Línea 2
promedio  → Línea 2
```

---

# 28. Control de posición

Cada token generado almacena:

```text
Línea
Columna
```

Esto facilita la identificación de errores.

Por ejemplo:

```text
<NUM_INT> 20  (Linea: 5, Columna: 8)
```

significa que el número `20` fue encontrado en:

```text
Línea 5
Columna 8
```

Este mecanismo también permite que los errores puedan ser ubicados con mayor facilidad.

---

# 29. Control de versiones

El proyecto fue desarrollado utilizando **Git y GitHub**, permitiendo registrar progresivamente los cambios realizados durante las diferentes etapas.

La evolución del repositorio incluye cambios relacionados con:

* implementación inicial;
* Fase 1;
* Fase 2;
* tabla de símbolos;
* reconocimiento de operadores;
* manejo de errores;
* integración del analizador;
* archivos de prueba;
* README.

Esto permite mantener un historial del desarrollo del proyecto y observar la evolución desde las primeras funcionalidades hasta la versión integrada.

---

# 30. Pruebas realizadas

Para verificar el funcionamiento del analizador se utilizaron diferentes archivos de prueba.

### Prueba de Fase 1

```text
tests/prueba_fase1.lp
```

Permite comprobar:

* números enteros;
* números decimales.

### Prueba de Fase 2

```text
tests/prueba_fase2.lp
```

Permite comprobar:

* identificadores;
* palabras reservadas;
* textos;
* tabla de símbolos.

### Prueba de Fase 3

```text
tests/prueba_fase3.lp
```

Permite comprobar la integración de los diferentes tokens.

### Prueba de errores

```text
tests/prueba_errores.lp
```

Permite comprobar el manejo de entradas inválidas.

---

# 31. Resultados obtenidos

Al finalizar las tres fases, LexLP permite:

* Leer archivos fuente `.lp`.
* Recorrer el código carácter por carácter.
* Reconocer números enteros.
* Reconocer números decimales.
* Reconocer identificadores.
* Reconocer constantes de texto.
* Reconocer palabras reservadas.
* Reconocer operadores.
* Reconocer símbolos.
* Reconocer operadores de comparación.
* Reconocer operadores lógicos.
* Reconocer comentarios.
* Generar una lista ordenada de tokens.
* Registrar línea y columna.
* Mantener una tabla de símbolos.
* Detectar errores léxicos.
* Generar archivos de salida.

---

# 32. Conclusiones

El desarrollo de **LexLP** permitió implementar progresivamente las principales funciones de un analizador léxico.

Durante la **Fase 1** se construyó la base del analizador mediante la lectura del archivo y el reconocimiento de números enteros y decimales.

En la **Fase 2** se amplió el proyecto para reconocer identificadores, constantes de texto y palabras reservadas. También se implementó la tabla de símbolos para registrar los identificadores encontrados.

Finalmente, en la **Fase 3** se integraron las funcionalidades anteriores y se agregaron operadores, símbolos, comentarios y manejo de errores. Además, se implementó la generación de la lista de tokens y de los archivos de salida.

El resultado es un analizador léxico capaz de recibir un programa escrito en el lenguaje LP, identificar sus componentes léxicos y proporcionar información útil sobre cada elemento encontrado.

El proyecto también permitió aplicar conceptos fundamentales de la asignatura de **Compiladores**, como:

* expresiones regulares;
* análisis léxico;
* tokens;
* lexemas;
* tabla de símbolos;
* reconocimiento de palabras reservadas;
* manejo de errores;
* posición de tokens;
* pruebas de programas fuente.

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

