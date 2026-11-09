#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include "validaciones.h"

#define TAM_BUFFER 128

static int fin_entrada = 0;

int validaciones_hubo_fin_entrada(void)
{
    return fin_entrada;
}

/* Lee una línea completa y descarta el resto si excede el buffer. */
static int leer_linea(char *buffer, size_t capacidad)
{
    size_t longitud;
    int c;

    if (fgets(buffer, (int)capacidad, stdin) == NULL) {
        fin_entrada = 1;
        return 0;
    }

    longitud = strlen(buffer);
    if (longitud > 0 && buffer[longitud - 1] == '\n') {
        buffer[--longitud] = '\0';
        if (longitud > 0 && buffer[longitud - 1] == '\r') {
            buffer[--longitud] = '\0';
        }
        return 1;
    }

    c = getchar();
    if (c == '\n' || c == EOF) {
        return 1;
    }

    do {
        c = getchar();
    } while (c != '\n' && c != EOF);

    if (c == EOF) {
        fin_entrada = 1;
        return 0;
    }
    return -1;
}

/* Convierte una línea completa a un float positivo; admite coma o punto. */
static int parsear_positivo(const char *linea, float *salida)
{
    char normalizada[TAM_BUFFER];
    const char *inicio = linea;
    char *fin;
    size_t indice = 0;
    float valor;

    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }
    while (*inicio != '\0') {
        if (indice + 1 >= sizeof(normalizada)) {
            return 0;
        }
        normalizada[indice++] = (*inicio == ',') ? '.' : *inicio;
        inicio++;
    }
    normalizada[indice] = '\0';

    errno = 0;
    valor = strtof(normalizada, &fin);
    if (fin == normalizada || errno == ERANGE || !isfinite(valor) || valor <= 0.0f) {
        return 0;
    }
    while (isspace((unsigned char)*fin)) {
        fin++;
    }
    if (*fin != '\0') {
        return 0;
    }

    *salida = valor;
    return 1;
}

static int leer_numero(const char *mensaje, float defecto, float limite,
                       int con_limite, float *salida)
{
    char buffer[TAM_BUFFER];
    int estado;
    float valor;

    for (;;) {
        if (con_limite) {
            printf("%s [Por defecto: %.2f, Máx. seguro: %.1f]: ",
                   mensaje, defecto, limite);
        } else {
            printf("%s [Por defecto: %.2f]: ", mensaje, defecto);
        }

        estado = leer_linea(buffer, sizeof(buffer));
        if (estado == 0) {
            return 0;
        }
        if (estado < 0) {
            printf("  Error: la entrada es demasiado larga.\n");
            continue;
        }
        if (buffer[0] == '\0') {
            *salida = defecto;
            printf("  Se utilizará el valor por defecto: %.2f\n\n", defecto);
            return 1;
        }
        if (!parsear_positivo(buffer, &valor)) {
            printf("  Error: ingrese un número válido mayor a cero (ej: 20.0 o 20,0).\n");
            continue;
        }
        if (con_limite && valor > limite) {
            printf("  Error físico: la aceleración supera el límite máximo seguro (%.1f).\n", limite);
            continue;
        }

        *salida = valor;
        printf("  Valor aceptado: %.2f\n\n", valor);
        return 1;
    }
}

float leer_aceleracion_con_limite(const char *mensaje, float valor_defecto,
                                  float limite_max)
{
    float valor = valor_defecto;
    if (!leer_numero(mensaje, valor_defecto, limite_max, 1, &valor)) {
        return valor_defecto;
    }
    return valor;
}

float leer_flotante_con_defecto(const char *mensaje, float valor_defecto)
{
    float valor = valor_defecto;
    if (!leer_numero(mensaje, valor_defecto, 0.0f, 0, &valor)) {
        return valor_defecto;
    }
    return valor;
}

float leer_altitud(const char *mensaje, float valor_defecto_m)
{
    char buffer[TAM_BUFFER];
    float valor;
    int estado;

    for (;;) {
        printf("%s [Por defecto: %.1f m / 160.0 km]: ", mensaje, valor_defecto_m);
        estado = leer_linea(buffer, sizeof(buffer));
        if (estado == 0) {
            return valor_defecto_m;
        }
        if (estado < 0) {
            printf("  Error: la entrada es demasiado larga.\n");
            continue;
        }
        if (buffer[0] == '\0') {
            printf("  Se utilizará la altitud por defecto: %.1f m\n\n", valor_defecto_m);
            return valor_defecto_m;
        }
        if (!parsear_positivo(buffer, &valor)) {
            printf("  Error: ingrese una altitud válida mayor a cero.\n");
            continue;
        }

        for (;;) {
            printf("  ¿En qué unidad ingresó la altitud? (1: Metros [m], 2: Kilómetros [km]): ");
            estado = leer_linea(buffer, sizeof(buffer));
            if (estado == 0) {
                return valor_defecto_m;
            }
            if (estado > 0 && (strcmp(buffer, "1") == 0 || strcmp(buffer, "2") == 0)) {
                break;
            }
            printf("  Opción inválida. Ingrese 1 para metros o 2 para kilómetros.\n");
        }

        if (buffer[0] == '2') {
            if (valor > FLT_MAX / 1000.0f) {
                printf("  Error: altitud demasiado grande para convertir a metros.\n");
                continue;
            }
            valor *= 1000.0f;
            if (!isfinite(valor)) {
                printf("  Error: desbordamiento al convertir a metros.\n");
                continue;
            }
            printf("  Convertido a metros: %.1f m\n\n", valor);
        } else {
            printf("  Altitud registrada: %.1f m\n\n", valor);
        }
        return valor;
    }
}

DatosSimulacion solicitar_datos_entrada(void)
{
    DatosSimulacion datos = {0};
    char buffer[TAM_BUFFER];
    int opcion;
    int estado;

    fin_entrada = 0;
    printf("=====================================================\n");
    printf("   INGRESO DE DATOS DE ENTRADA DE LA SIMULACIÓN SLS\n");
    printf("  (Presione Enter en cualquier campo para usar valor por defecto)\n");
    printf("=====================================================\n\n");

    datos.a1 = leer_aceleracion_con_limite("1. Aceleración de la Fase 1 (m/s^2)", 20.0f, 50.0f);
    if (fin_entrada) return datos;
    datos.t1 = leer_flotante_con_defecto("2. Tiempo de vuelo hasta desacople SRB (en segundos)", 120.0f);
    if (fin_entrada) return datos;
    datos.a2 = leer_aceleracion_con_limite("3. Aceleración de la Fase 2 (m/s^2)", 12.5f, 50.0f);
    if (fin_entrada) return datos;
    datos.h = leer_altitud("4. Altitud de apagado de motores", 160000.0f);
    if (fin_entrada) return datos;
    datos.g = leer_flotante_con_defecto("5. Aceleración de la gravedad (m/s^2)", 9.80f);
    if (fin_entrada) return datos;

    for (;;) {
        printf("\n-----------------------------------------------------\n");
        printf("           RESUMEN DE DATOS INGRESADOS\n");
        printf("-----------------------------------------------------\n");
        printf(" 1. Aceleración Fase 1 (a1) : %.2f m/s^2\n", datos.a1);
        printf(" 2. Tiempo inicial (t1)      : %.2f s\n", datos.t1);
        printf(" 3. Aceleración Fase 2 (a2) : %.2f m/s^2\n", datos.a2);
        printf(" 4. Altitud meta (h)        : %.2f m\n", datos.h);
        printf(" 5. Gravedad (g)            : %.2f m/s^2\n", datos.g);
        printf("-----------------------------------------------------\n");
        printf("¿Los datos son correctos?\n");
        printf("  [0] Sí, confirmar y continuar\n");
        printf("  [1-5] Modificar un parámetro específico\n");
        printf("  Opción elegida: ");

        estado = leer_linea(buffer, sizeof(buffer));
        if (estado == 0) {
            fin_entrada = 1;
            return datos;
        }
        if (estado < 0 || buffer[1] != '\0' || buffer[0] < '0' || buffer[0] > '5') {
            printf("  Opción inválida. Ingrese un número entre 0 y 5.\n");
            continue;
        }

        opcion = buffer[0] - '0';
        if (opcion == 0) {
            break;
        }
        printf("\n--- Modificar parámetro %d ---\n", opcion);
        switch (opcion) {
            case 1:
                datos.a1 = leer_aceleracion_con_limite("1. Aceleración de la Fase 1 (m/s^2)", 20.0f, 50.0f);
                break;
            case 2:
                datos.t1 = leer_flotante_con_defecto("Nuevo tiempo de vuelo SRB (s)", datos.t1);
                break;
            case 3:
                datos.a2 = leer_aceleracion_con_limite("3. Aceleración de la Fase 2 (m/s^2)", 12.5f, 50.0f);
                break;
            case 4:
                datos.h = leer_altitud("Nueva altitud de apagado", datos.h);
                break;
            case 5:
                datos.g = leer_flotante_con_defecto("Nueva aceleración de la gravedad (m/s^2)", datos.g);
                break;
            default:
                break;
        }
        if (fin_entrada) return datos;
    }

    printf("\nDatos confirmados con éxito.\n\n");
    return datos;
}
