#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "validaciones.h"

// Función auxiliar para reemplazar comas por puntos y verificar si la cadena es un número válido
static int procesar_y_validar_numero(char *buffer, float *out_valor) {
    // Reemplazamos la coma por punto si el usuario la utilizó
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == ',') {
            buffer[i] = '.';
        }
    }

    // Intentamos convertir a flotante con sscanf
    float val;
    if (sscanf(buffer, "%f", &val) == 1) {
        if (val > 0.0f) {
            *out_valor = val;
            return 1; // Válido y mayor a cero
        }
    }
    return 0; // Inválido o menor/igual a cero
}

float leer_aceleracion_con_limite(const char *mensaje, float valor_defecto, float limite_max) {
    char buffer[100];
    float valor;

    while (1) {
        printf("%s [Por defecto: %.2f, Max seguro: %.1f]: ", mensaje, valor_defecto, limite_max);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        // Si presiona Enter sin escribir nada, usa el valor por defecto
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            printf(" -> Se utilizara el valor por defecto: %.2f\n\n", valor_defecto);
            return valor_defecto;
        }

        // REEMPLAZO DE COMA POR PUNTO 
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (buffer[i] == ',') {
                buffer[i] = '.'; // Cambia la coma por un punto para que sscanf no falle
            }
        }

        // Intentamos convertir a flotante y validamos el rango
        if (sscanf(buffer, "%f", &valor) == 1) {
            if (valor > 0.0f && valor <= limite_max) {
                printf("  Valor aceptado: %.2f\n\n", valor);
                return valor;
            } else if (valor > limite_max) {
                printf("  >>Error fisico: La aceleracion (%.2f m/s^2) supera el limite maximo seguro para la tripulacion (%.1f m/s^2).\n", valor, limite_max);
            } else {
                printf("  >>Error: Ingrese un valor mayor a cero.\n");
            }
        } else {
            printf("  Error: Ingrese un numero valido (ej: 20.0 o 20,0).\n");
        }
    }
}
// Lee un número flotante con valor por defecto si el usuario presiona Enter
float leer_flotante_con_defecto(const char *mensaje, float valor_defecto) {
    char buffer[100];
    float valor;

    while (1) {
        printf("%s [Por defecto: %.2f]: ", mensaje, valor_defecto);
        
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        // Si presiona Enter sin escribir nada (solo carácter newline)
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            printf("  Se utilizara el valor por defecto: %.2f\n\n", valor_defecto);
            return valor_defecto;
        }

        if (procesar_y_validar_numero(buffer, &valor)) {
            printf(" Valor aceptado: %.2f\n\n", valor);
            return valor;
        } else {
            printf("   >>Error: Ingrese un numero valido mayor a cero (use punto para decimales, ej: 20.0).\n");
        }
    }
}

// Lee la altitud, pregunta la unidad y convierte a metros si es necesario
float leer_altitud(const char *mensaje, float valor_defecto_m) {
    char buffer[100];
    float valor;
    int unidad_opcion;

    while (1) {
        printf("%s [Por defecto: %.1f m / 160.0 km]: ", mensaje, valor_defecto_m);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            continue;
        }

        // Si presiona Enter directamente
        if (buffer[0] == '\n' || buffer[0] == '\r') {
            printf("  Se utilizara la altitud por defecto: %.1f m\n\n", valor_defecto_m);
            return valor_defecto_m;
        }

        if (procesar_y_validar_numero(buffer, &valor)) {
            // Preguntamos la unidad ingresada
            while (1) {
                printf("  ¿En que unidad ingreso la altitud? (1: Metros [m], 2: Kilometros [km]): ");
                if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                    if (sscanf(buffer, "%d", &unidad_opcion) == 1) {
                        if (unidad_opcion == 1) {
                            printf("  Altitud registrada: %.1f m\n\n", valor);
                            return valor;
                        } else if (unidad_opcion == 2) {
                            float valor_m = valor * 1000.0f; // Conversión km a m
                            printf(" Convertido a metros: %.1f m\n\n", valor_m);
                            return valor_m;
                        }
                    }
                }
                printf("  >>Opcion invalida. Ingrese 1 para metros o 2 para kilometros.\n");
            }
        } else {
            printf("   >>Error: Ingrese una altitud valida mayor a cero.\n");
        }
    }
}

// Función mejorada con opción de revisión y edición de datos
DatosSimulacion solicitar_datos_entrada(void) {
    DatosSimulacion datos;
    int confirmar = 0;
    char buffer[100];

    // Bucle para ingresar los datos por primera vez o re-ingresarlos si es necesario
    do {
        printf("=====================================================\n");
        printf("   INGRESO DE DATOS DE ENTRADA DE LA SIMULACION SLS  \n");
        printf("  (Presione Enter en cualquier campo para usar valor por defecto)\n");
        printf("=====================================================\n\n");

        // 1. Aceleración Fase 1
        datos.a1 = leer_aceleracion_con_limite("1. Aceleracion de la Fase 1 (m/s^2)", 20.0f, 50.0f);

        // 2. Tiempo inicial SRB
        datos.t1 = leer_flotante_con_defecto("2. Tiempo de vuelo hasta desacople SRB (en segundos)", 120.0f);

        // 3. Aceleración Fase 2
        datos.a2 = leer_aceleracion_con_limite("3. Aceleracion de la Fase 2 (m/s^2)", 12.5f, 50.0f);

        // 4. Altitud de apagado de motores
        datos.h = leer_altitud("4. Altitud de apagado de motores", 160000.0f);

        // 5. Aceleración de la gravedad
        datos.g = leer_flotante_con_defecto("5. Aceleracion de la gravedad (m/s^2)", 9.80f);

        // Bucle de revisión y edición individual
        int opcion_edicion = 0;
        while (1) {
            printf("\n-----------------------------------------------------\n");
            printf("           RESUMEN DE DATOS INGRESADOS               \n");
            printf("-----------------------------------------------------\n");
            printf(" 1. Aceleración Fase 1 (a1) : %.2f m/s^2\n", datos.a1);
            printf(" 2. Tiempo inicial (t1)      : %.2f s\n", datos.t1);
            printf(" 3. Aceleración Fase 2 (a2) : %.2f m/s^2\n", datos.a2);
            printf(" 4. Altitud meta (h)        : %.2f m\n", datos.h);
            printf(" 5. Gravedad (g)            : %.2f m/s^2\n", datos.g);
            printf("-----------------------------------------------------\n");
            printf(" Los datos son correctos?\n");
            printf("  [0] Si, confirmar y continuar\n");
            printf("  [1-5] Modificar un parametro especifico\n");
            printf("  Opcion elegida: ");

            if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
                if (sscanf(buffer, "%d", &opcion_edicion) == 1) {
                    if (opcion_edicion == 0) {
                        confirmar = 1; // Salimos del bucle principal
                        break;
                    } else if (opcion_edicion >= 1 && opcion_edicion <= 5) {
                        printf("\n--- Modificar parametro %d ---\n", opcion_edicion);
                        switch (opcion_edicion) {
                            case 1:
                               datos.a1 = leer_aceleracion_con_limite("1. Aceleracion de la Fase 1 (m/s^2)", 20.0f, 50.0f);
                                break;
                            case 2:
                                datos.t1 = leer_flotante_con_defecto("Nuevo Tiempo de vuelo SRB (s)", datos.t1);
                                break;
                            case 3:
                              datos.a2 = leer_aceleracion_con_limite("3. Aceleracion de la Fase 2 (m/s^2)", 12.5f, 50.0f);
                                break;
                            case 4:
                                datos.h = leer_altitud("Nueva Altitud de apagado", datos.h);
                                break;
                            case 5:
                                datos.g = leer_flotante_con_defecto("Nueva Aceleracion de la gravedad (m/s^2)", datos.g);
                                break;
                        }
                    } else {
                        printf("  >> Opcion inválida. Ingrese un numero entre 0 y 5.\n");
                    }
                }
            }
        }

    } while (!confirmar);

    printf("\n  Datos confirmados con exito! :)\n\n");
    return datos;
}