
/*
 * main.c
 * MÓDULO 6: main.c (menú y flujo general)
 * Usa solicitar_datos_entrada() de validaciones.c y los módulos 2 a 5.
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "artemis.h"
#include "validaciones.h"

/* Lee la opción del menú de forma segura (termina si se cierra la entrada) */
static int leer_opcion(int minimo, int maximo)
{
    char linea[64];
    char *fin;
    long valor;

    for (;;) {
        printf("Opción: ");
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

        printf("  [!] Ingrese un número entre %d y %d.\n", minimo, maximo);
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
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    DatosSimulacion datos;
    ResultadosSimulacion res = {0};
    int opcion;
    int simulaciones = 0;

    do {
        printf("\n==================================================\n");
        printf("       SIMULADOR ARTEMIS - ASCENSO DEL SLS      \n");
        printf("==================================================\n");
        printf(" 1. Ingresar datos de simulación personalizados\n");
        printf(" 2. Ejecutar con datos oficiales del enunciado\n");
        printf(" 3. Ver historial de simulaciones\n");
        printf(" 4. Ver resultados detallados de la misión\n");
        printf(" 5. Salir del sistema\n");
        printf("--------------------------------------------------\n");

        opcion = leer_opcion(1, 5);

        switch (opcion) {
            case 1:
                datos = solicitar_datos_entrada();  /* MÓDULO 1 */
                if (validaciones_hubo_fin_entrada()) return EXIT_FAILURE;

                simular(&datos, &res);
                simulaciones++;
                registrar_simulacion(1, (float)(res.hmax / 1000.0));

                printf("\nSimulación completada.\n");
                break;

            case 2:
                datos = cargar_datos_enunciado();   /* MÓDULO 1 */

                simular(&datos, &res);
                simulaciones++;
                registrar_simulacion(2, (float)(res.hmax / 1000.0));

                printf("\nSimulación completada con los datos del enunciado.\n");
                break;

            case 3:
                mostrar_historial();
                pausar_pantalla();
                break;

            case 4:
                if (simulaciones > 0) {
                    aceleracion_g_y_presentacion(&datos, &res); /* MÓDULO 5 */
                    pausar_pantalla();
                } else {
                    printf("\n[!] Primero debe simular (opción 1 o 2).\n");
                }
                break;

            case 5:
                reporte_final(simulaciones, &res);  /* MÓDULO 5 */

                printf("\n==================================================\n");
                printf("       CIERRE DE TELEMETRÍA - MISIÓN ARTEMIS      \n");
                printf("==================================================\n");
                printf("  Desconectando sistemas de abordo...\n");
                printf("  Sesión finalizada con éxito. Hasta pronto!\n");
                printf("--------------------------------------------------\n");
                break;

            default:
                break;
        }

    } while (opcion != 5);

    return 0;
}
