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
    int simulaciones = 0;

    do {
        printf("\n==================================================\n");
        printf("       SIMULADOR ARTEMIS - ASCENSO DEL SLS      \n");
        printf("==================================================\n");
        printf(" 1. Ingresar datos de simulacion personalizados\n");
        printf(" 2. Ejecutar con datos oficiales del enunciado\n");
        printf(" 3. Ver resultados detallados de la mision\n");
        printf(" 4. Salir del sistema\n");
        printf("--------------------------------------------------\n");
        opcion = leer_opcion(1, 4);

        switch (opcion) {
            case 1:
                datos = solicitar_datos_entrada();     /* MÓDULO 1 */
                simular(&datos, &res);
                simulaciones++;
                printf("\nSimulacion completada.\n");
                break;
            case 2:
                datos = cargar_datos_enunciado();      /* MÓDULO 1 */
                simular(&datos, &res);
                simulaciones++;
                printf("\nSimulacion completada con los datos del enunciado.\n");
                break;
            case 3:
                if (simulaciones > 0) {
                    aceleracion_g_y_presentacion(&datos, &res);   /* MÓDULO 5 */
                    pausar_pantalla();
                } else {
                    printf("\n[!] Primero debe simular (opcion 1 o 2).\n");
                }
                break;
            case 4:
                reporte_final(simulaciones, &res);         /* MÓDULO 5 */
                printf("\n==================================================\n");
                printf("       CIERRE DE TELEMETRIA - MISION ARTEMIS      \n");
                printf("==================================================\n");
                printf("  Desconectando sistemas de abordo...\n");
                printf("  Sesion finalizada con exito. Hasta pronto!\n");
                printf("--------------------------------------------------\n");
                break;
            default:
                break;
        }
    } while (opcion != 4);

    return 0;
}
