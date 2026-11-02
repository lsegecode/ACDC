//Esto es solo un main generado para probar que funcionan los archivos cabecera y los C.
#include <stdio.h>
#include "validaciones.h"

int main() {
    // Llamamos a tu función modularizada
    DatosSimulacion entrada = solicitar_datos_entrada();

    // Verificamos que los datos quedaron disponibles en la estructura
    printf("=== DATOS GUARDADOS CON EXITO ===\n");
    printf("Aceleracion Fase 1 (a1) : %.2f m/s^2\n", entrada.a1);
    printf("Tiempo inicial (t1)      : %.2f s\n", entrada.t1);
    printf("Aceleracion Fase 2 (a2) : %.2f m/s^2\n", entrada.a2);
    printf("Altitud meta (h)        : %.2f m\n", entrada.h);
    printf("Gravedad (g)            : %.2f m/s^2\n", entrada.g);
    printf("===================================================\n");

    return 0;
}

//Para ejecutar: voy a nueva terminal, coloco gcc main.c validaciones.c -o main y luego .\main