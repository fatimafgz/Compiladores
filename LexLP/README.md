# LexLP – Analizador Léxico para un Lenguaje de Programación de Alcance Reducido

**Proyecto:** LexLP – Analizador Léxico para un Lenguaje de Programación de Alcance Reducido

**Fase actual:** Fase 1 – Lectura + NUM_INT + NUM_DEC

**Curso:** Compiladores

**Fecha de entrega:** 15/09/2026

## Descripción

**LexLP** es un proyecto desarrollado para el curso de **Compiladores**. Su objetivo es implementar un analizador léxico capaz de leer un archivo fuente y reconocer los diferentes elementos que forman parte de un lenguaje de programación de alcance reducido.

El proyecto se desarrollará progresivamente en diferentes fases.

En esta primera fase se implementa la lectura del archivo fuente y el reconocimiento de **números enteros (`NUM_INT`)** y **números decimales (`NUM_DEC`)**.

---

# 1. Objetivo de la Fase 1

El objetivo de esta primera fase es desarrollar la base del analizador léxico, permitiendo:

* Leer un archivo fuente con extensión `.lp`.
* Recorrer el contenido del archivo carácter por carácter.
* Identificar números enteros.
* Identificar números decimales.
* Generar tokens indicando:

  * Tipo de token.
  * Lexema.
  * Línea.
  * Columna.
* Mostrar los tokens reconocidos en la consola.

---

# 2. Especificaciones implementadas

Para esta fase se consideran las siguientes reglas:

### Número entero

Un número entero está formado por uno o más dígitos.

```text
D = [0-9]

D+ → NUM_INT
```

Ejemplos:

```text
20
100
0
7
50
999
```

### Número decimal

Un número decimal está formado por uno o más dígitos, seguido de un punto y uno o más dígitos.

```text
D+\.D+ → NUM_DEC
```

Ejemplos:

```text
15.5
3.14
25.75
999.99
1.0
```

---

# 3. Estructura del proyecto

```text
LexLP/
│
├── src/
│   ├── main.cpp
│   ├── Lexer.cpp
│   ├── Lexer.h
│   ├── Token.cpp
│   └── Token.h
│
└── tests/
    └── prueba_numeros.lp
```

---

# 4. Descripción de los archivos

## `main.cpp`

Es el archivo principal del programa.

Sus funciones principales son:

1. Abrir el archivo `prueba_numeros.lp`.
2. Leer todo su contenido.
3. Crear un objeto de la clase `Lexer`.
4. Ejecutar el análisis léxico.
5. Mostrar los tokens reconocidos en la consola.

---

## `Lexer.h`

Contiene la declaración de la clase `Lexer`.

En esta clase se definen elementos como:

* La fuente de código.
* La posición actual.
* La línea.
* La columna.
* Las funciones necesarias para analizar los números.

---

## `Lexer.cpp`

Contiene la implementación del analizador léxico.

En esta fase se encarga principalmente de:

* Avanzar por los caracteres del archivo.
* Identificar dígitos.
* Reconocer números enteros.
* Reconocer números decimales.
* Registrar la posición donde aparece cada token.

---

## `Token.h`

Define la estructura que representa un token.

Cada token contiene:

```text
tipo
lexema
linea
columna
```

Por ejemplo:

```text
<NUM_DEC> Lexema: 15.5 | Linea: 2 | Columna: 1
```

---

## `Token.cpp`

Contiene la implementación asociada a la estructura `Token`.

---

## `prueba_numeros.lp`

Es el archivo utilizado para realizar las pruebas de la primera fase.

Contenido utilizado:

```text
20
15.5
100
3.14
0
25.75
7
999.99
1.0
50
```

---
# 5. Funcionamiento del analizador

El funcionamiento de **LexLP** durante esta primera fase puede representarse de la siguiente manera:

```text
                         ┌─────────────────────┐
                         │     ARCHIVO .LP     │
                         │  prueba_numeros.lp  │
                         └──────────┬──────────┘
                                    │
                                    ▼
                         ┌─────────────────────┐
                         │    LECTURA DEL      │
                         │      ARCHIVO        │
                         └──────────┬──────────┘
                                    │
                                    ▼
                    ┌──────────────────────────────┐
                    │           LEXER              │
                    │                              │
                    │  Recorre el archivo carácter │
                    │       por carácter            │
                    └──────────────┬───────────────┘
                                   │
                                   ▼
                         ┌─────────────────────┐
                         │   ¿COMIENZA CON    │
                         │     UN DÍGITO?     │
                         └──────────┬──────────┘
                                    │
                              ┌─────┴─────┐
                             SÍ           NO
                              │             │
                              ▼             ▼
                    ┌───────────────┐   Continúa
                    │ RECONOCER     │   recorriendo
                    │    NÚMERO     │
                    └───────┬───────┘
                            │
                            ▼
                  ┌──────────────────────┐
                  │ ¿TIENE PUNTO Y       │
                  │ DÍGITOS DESPUÉS?    │
                  └──────────┬───────────┘
                             │
                       ┌─────┴─────┐
                      SÍ           NO
                       │             │
                       ▼             ▼
                ┌────────────┐  ┌────────────┐
                │  NUM_DEC   │  │  NUM_INT   │
                │   15.5     │  │    20      │
                └─────┬──────┘  └─────┬──────┘
                      │                │
                      └───────┬────────┘
                              ▼
                    ┌─────────────────────┐
                    │   GENERAR TOKEN     │
                    │                     │
                    │ tipo + lexema +    │
                    │ línea + columna    │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │  MOSTRAR RESULTADO  │
                    │      EN CONSOLA     │
                    └─────────────────────┘
```

### Ejemplo del resultado

Si el archivo contiene:

```text
20
15.5
```

El analizador genera:

```text
<NUM_INT> Lexema: 20 | Linea: 1 | Columna: 1
<NUM_DEC> Lexema: 15.5 | Linea: 2 | Columna: 1
```

De esta manera, el **Lexer** recibe el archivo, analiza su contenido y transforma los valores reconocidos en tokens que posteriormente podrán ser utilizados por las siguientes fases del compilador.


---

# 6. Compilación del proyecto

Para compilar el proyecto se utiliza **g++** desde la terminal de VS Code.

Desde la carpeta principal `LexLP`, ejecutar:

```bash
g++ src/main.cpp src/Lexer.cpp src/Token.cpp -o LexLP.exe
```

Si la compilación es correcta, se genera el archivo:

```text
LexLP.exe
```

---

# 7. Ejecución

Para ejecutar el programa:

```bash
.\LexLP.exe
```

El programa leerá automáticamente el archivo:

```text
tests/prueba_numeros.lp
```

---

# 8. Resultado de las pruebas

Al ejecutar el programa se obtiene una lista de tokens similar a:

```text
===== LISTA DE TOKENS =====
<NUM_INT> Lexema: 20 | Linea: 1 | Columna: 1
<NUM_DEC> Lexema: 15.5 | Linea: 2 | Columna: 1
<NUM_INT> Lexema: 100 | Linea: 3 | Columna: 1
<NUM_DEC> Lexema: 3.14 | Linea: 4 | Columna: 1
<NUM_INT> Lexema: 0 | Linea: 5 | Columna: 1
<NUM_DEC> Lexema: 25.75 | Linea: 6 | Columna: 1
<NUM_INT> Lexema: 7 | Linea: 7 | Columna: 1
<NUM_DEC> Lexema: 999.99 | Linea: 8 | Columna: 1
<NUM_DEC> Lexema: 1.0 | Linea: 9 | Columna: 1
<NUM_INT> Lexema: 50 | Linea: 10 | Columna: 1
```

Los resultados muestran que el analizador puede diferenciar correctamente entre números enteros y números decimales.

---

# 9. Pruebas realizadas

Se realizaron pruebas utilizando diferentes valores numéricos.

| Entrada  | Token esperado | Resultado |
| -------- | -------------- | --------- |
| `20`     | NUM_INT        | Correcto  |
| `100`    | NUM_INT        | Correcto  |
| `0`      | NUM_INT        | Correcto  |
| `7`      | NUM_INT        | Correcto  |
| `50`     | NUM_INT        | Correcto  |
| `15.5`   | NUM_DEC        | Correcto  |
| `3.14`   | NUM_DEC        | Correcto  |
| `25.75`  | NUM_DEC        | Correcto  |
| `999.99` | NUM_DEC        | Correcto  |
| `1.0`    | NUM_DEC        | Correcto  |

---

# 10. Ejemplos relacionados con el lenguaje

Dentro del lenguaje que se está desarrollando, los números podrán aparecer posteriormente en instrucciones como:

```text
int edad = 20;
```

En este caso, el valor:

```text
20
```

será reconocido como:

```text
NUM_INT
```

Otro ejemplo:

```text
float promedio = 15.5;
```

El valor:

```text
15.5
```

será reconocido como:

```text
NUM_DEC
```

> Nota: en esta primera fase solamente se implementa el reconocimiento de los números. Los identificadores, palabras reservadas, operadores y demás elementos serán incorporados progresivamente en las siguientes fases del proyecto.

---

# 11. Tecnologías utilizadas

* **Lenguaje:** C++
* **Compilador:** G++
* **Editor:** Visual Studio Code
* **Sistema operativo:** Windows
* **Control de versiones:** Git / GitHub
* **Extensión del archivo fuente:** `.lp`

---

# 12. Conclusión

En esta primera fase se implementó la base del analizador léxico **LexLP**.

Se logró realizar la lectura de un archivo fuente y reconocer correctamente números enteros y números decimales, generando tokens con su tipo, lexema, línea y columna.

Esta implementación servirá como base para agregar posteriormente los identificadores, palabras reservadas, texto, operadores, símbolos especiales y manejo de errores léxicos.

---

# 13. Próximas fases

El proyecto continuará desarrollándose progresivamente:

### Fase 2

Se implementará:

* Identificadores (`ID`).
* Texto (`TEXTO`).
* Palabras reservadas.
* Tabla de símbolos.

### Fase 3

Se implementará:

* Operadores.
* Símbolos especiales.
* Lista completa de tokens.
* Comentarios.
* Errores léxicos.
* Integración final.
* Pruebas completas.
