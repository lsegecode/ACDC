/*
 * main.c
 * MÓDULO 6: main.c (menú y flujo general)
 * Usa solicitar_datos_entrada() de validaciones.c y los módulos 2 a 5.
 */
#include <stdio.h>
#include <stdlib.h>
#include "artemis.h"
#include "validaciones.h"

/* Lee la opción del menú de forma segura (termina si se cierra la entrada) */
static int leer_opcion(int minimo, int maximo)
{
    char linea[64];
    char *fin;
    long valor;

    for (;;) {
        printf("Opcion: ");
        if (fgets(linea, sizeof(linea), stdin) == NULL) {
            printf("\nEntrada finalizada. Cerrando el programa.\n");
            exit(EXIT_FAILURE);
        }
        valor = strtol(linea, &fin, 10);
        while (*fin == ' ' || *fin == '\t' || *fin == '\r' || *fin == '\n') {
            fin++;
        }
        if (fin != linea && *fin == '\0' && valor >= minimo && valor <= maximo) {
            return (int)valor;
        }
        printf("  [!] Ingrese un numero entre %d y %d.\n", minimo, maximo);
    }
}

/* Ejecuta los módulos 2, 3 y 4 en orden */
static void simular(const DatosSimulacion *d, ResultadosSimulacion *r)
{
    fase1_chequeo_coherencia(d, r);       /* MÓDULO 2 */
    fase2_mruv_velocidad_inicial(d, r);   /* MÓDULO 3 */
    caida_libre_tras_apagado(d, r);       /* MÓDULO 4 */
}

int main(void)
{
    DatosSimulacion datos;
    ResultadosSimulacion res = {0};
    int opcion;
    int simulado = 0;

    do {
        printf("\n=== SIMULADOR ARTEMIS: ASCENSO DEL SLS ===\n");
        printf("1. Ingresar datos y simular\n");
        printf("2. Usar datos del enunciado y simular\n");
        printf("3. Ver resultados\n");
        printf("4. Salir\n");
        opcion = leer_opcion(1, 4);

        switch (opcion) {
            case 1:
                datos = solicitar_datos_entrada();     /* MÓDULO 1 */
                simular(&datos, &res);
                simulado = 1;
                printf("\nSimulacion completada.\n");
                break;
            case 2:
                datos = cargar_datos_enunciado();      /* MÓDULO 1 */
                simular(&datos, &res);
                simulado = 1;
                printf("\nSimulacion completada con los datos del enunciado.\n");
                break;
            case 3:
                if (simulado) {
                    aceleracion_g_y_presentacion(&datos, &res);   /* MÓDULO 5 */
                } else {
                    printf("\n[!] Primero debe simular (opcion 1 o 2).\n");
                }
                break;
            case 4:
                printf("\nFin de la mision. Hasta pronto.\n");
                break;
            default:
                break;
        }
    } while (opcion != 4);

    return 0;
}
