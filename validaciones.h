#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#include <stdbool.h>

/* Pide un entero entre min y max (inclusive). Repite hasta que sea valido. */
int leer_opcion(const char *mensaje, int min, int max);

/* Pide un numero > 0. Enter = valor por defecto. Acepta coma o punto.
   Si acepta_km es true, permite escribir "160 km" y lo pasa a metros.
   Rechaza letras, vacios, NaN, infinitos, ceros y negativos. */
double leer_numero(const char *nombre, const char *unidad,
                   double defecto, bool acepta_km);

/* Devuelve el numero como texto con coma decimal (buffer rotativo). */
const char *fmt(double valor, int decimales);

#endif /* VALIDACIONES_H */
