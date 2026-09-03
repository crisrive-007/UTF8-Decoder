# Lab 1: Decodificador de UTF-8

Programa en C++ que lee un archivo de texto en modo binario y decodifica manualmente su contenido byte por byte utilizando el estándar UTF-8, detectando errores de codificación y reportando las estadísticas del archivo procesado.

## Requisitos de Compilación

* **Compilador:** `g++` (GCC 13 o superior recomendado para soporte completo de `<format>`)
* **Estándar C++:** C++20 (`-std=c++20`)

## Compilación

Para compilar el proyecto utilizando `g++`, ejecuta el siguiente comando en la terminal:

```bash
g++ -std=c++20 main.cpp decoder.cpp -o utf8_decoder