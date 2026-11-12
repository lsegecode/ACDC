
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

/* ---------------------------------------------------------------------
 * Opción 6: ejecutar un módulo por separado.
 * Reutiliza las funciones de validaciones.c (lectura con Enter = valor del
 * enunciado) y de artemis.c (módulos 2 a 5). Cada módulo pide solo los
 * datos que necesita; los demás quedan con los valores oficiales.
 * ------------------------------------------------------------------- */
static void ejecutar_modulo_individual(void)
{
    DatosSimulacion d = cargar_datos_enunciado();   /* valores oficiales */
    ResultadosSimulacion r = {0};
    int modulo;

    printf("\n--- MÓDULOS INDIVIDUALES ---\n");
    printf(" 1. Módulo 1: Entrada y validación de datos\n");
    printf(" 2. Módulo 2: Fase 1 y chequeo de coherencia\n");
    printf(" 3. Módulo 3: Fase 2 (MRUV con velocidad inicial)\n");
    printf(" 4. Módulo 4: Caída libre tras el apagado\n");
    printf(" 5. Módulo 5: Aceleración en múltiplos de g\n");
    printf(" 6. Volver al menú principal\n");
    modulo = leer_opcion(1, 6);

    switch (modulo) {
        case 1:
            d = solicitar_datos_entrada();
            if (validaciones_hubo_fin_entrada()) exit(EXIT_FAILURE);
            printf("\nDatos validados correctamente.\n");
            break;

        case 2:
            printf("\nDatos que necesita el módulo 2 (Enter = valor oficial):\n");
            d.a1 = leer_aceleracion_con_limite("Aceleración de la Fase 1 (m/s^2)", d.a1, 50.0f);
            d.t1 = leer_flotante_con_defecto("Tiempo hasta desacople SRB (s)", d.t1);
            d.h  = leer_altitud("Altitud de apagado de motores", d.h);
            if (validaciones_hubo_fin_entrada()) exit(EXIT_FAILURE);
            fase1_chequeo_coherencia(&d, &r);
            printf("\n  v1 = %.1f m/s\n  y1 = %.1f m (%.2f km)\n",
                   r.v1, r.y1, r.y1 / 1000.0);
            if (!r.hay_fase2) {
                printf("  [!] y1 >= h: no hay fase 2 (la nave llega a h antes del desacople).\n");
            }
            break;

        case 3:
            printf("\nDatos que necesita el módulo 3 (Enter = valor oficial):\n");
            d.a1 = leer_aceleracion_con_limite("Aceleración de la Fase 1 (m/s^2)", d.a1, 50.0f);
            d.t1 = leer_flotante_con_defecto("Tiempo hasta desacople SRB (s)", d.t1);
            d.a2 = leer_aceleracion_con_limite("Aceleración de la Fase 2 (m/s^2)", d.a2, 50.0f);
            d.h  = leer_altitud("Altitud de apagado de motores", d.h);
            if (validaciones_hubo_fin_entrada()) exit(EXIT_FAILURE);
            fase1_chequeo_coherencia(&d, &r);       /* da v1, y1 */
            fase2_mruv_velocidad_inicial(&d, &r);
            printf("\n  Rapidez al apagado v2 = %.1f m/s\n", r.v2);
            if (!r.hay_fase2) {
                printf("  (no hay fase 2: calculada solo con la fase 1)\n");
            }
            break;

        case 4:
            printf("\nDatos que necesita el módulo 4 (Enter = valor oficial):\n");
            r.v2 = leer_flotante_con_defecto("Rapidez al apagado v2 (m/s)", 2481.94f);
            d.h  = leer_altitud("Altitud de apagado de motores", d.h);
            d.g  = leer_flotante_con_defecto("Aceleración de la gravedad (m/s^2)", d.g);
            if (validaciones_hubo_fin_entrada()) exit(EXIT_FAILURE);
            caida_libre_tras_apagado(&d, &r);
            printf("\n  Ascenso extra = %.1f m\n  Altitud máxima = %.1f m (%.2f km)\n",
                   r.hmax - d.h, r.hmax, r.hmax / 1000.0);
            break;

        case 5:
            printf("\nDatos que necesita el módulo 5 (Enter = valor oficial):\n");
            d.a1 = leer_aceleracion_con_limite("Aceleración de la Fase 1 (m/s^2)", d.a1, 50.0f);
            d.g  = leer_flotante_con_defecto("Aceleración de la gravedad (m/s^2)", d.g);
            if (validaciones_hubo_fin_entrada()) exit(EXIT_FAILURE);
            printf("\n  Aceleración máxima de la fase 1 = %.2f g\n",
                   aceleracion_en_g(d.a1, d.g));
            break;

        default:
            return;
    }
    pausar_pantalla();
}

/* Portada ASCII que se muestra una sola vez al iniciar el programa */
static void mostrar_portada(void)
{
    static const char *const arte[] = {
        "          :::     :::::::::: :::::::::",
        "       :+: :+:   :+:        :+:    :+:",
        "     +:+   +:+  +:+        +:+    +:+",
        "   +#++:++#++: +#++:++#   +#+    +:+",
        "  +#+     +#+ +#+        +#+    +#+",
        " #+#     #+# #+#        #+#    #+#",
        "###     ### ########## #########",
        "",
        "",
        "                    /\\",
        "                   //\\\\",
        "                  ||##||",
        "                 //##mm\\\\",
        "                //##*mmm\\\\",
        "               //###**mmm\\\\",
        "              //###***nmmm\\\\",
        "             //####***@nmmm\\\\",
        "             ||####***@nnmm||",
        "             ||####**@@@nnm||",
        "             |______________|",
        "             |     ACDC     |",
        "              \\____________/",
        "               |          |",
        "              /|    /\\    |\\",
        "             /_|    || /\\ |_\\",
        "               |      ACDC|",
        "               |       \\/ |",
        "               |          |",
        "              /|    /\\    |\\",
        "             / |    ||    | \\",
        "            /  |    ||    |  \\",
        "           /  /\\    ||    /\\  \\",
        "          |__/  \\   ||   /  \\__|",
        "            /____\\      /____\\",
        "            |    |      |    |",
        "            |    |______|    |",
        "            |    | /--\\ |    |",
        "            |____|/----\\|____|",
        "             \\||/ //##\\\\ \\||/",
        "             /##\\//####\\\\/##\\",
        "            //##\\\\/####\\//##\\\\",
        "           ||/::\\||/##\\||/::\\||",
        "           \\\\\\''///:**:\\\\\\''///",
        "            \\\\\\///\\::::/\\\\\\///",
        "             \\\\//\\\\\\::///\\\\//",
        "              \\/\\\\\\\\..////\\/",
        "                 \\\\\\\\////",
        "                  \\\\\\///",
        "                   \\\\//",
        "                    \\/"
    };
    size_t i;

    printf("\n");
    for (i = 0; i < sizeof(arte) / sizeof(arte[0]); i++) {
        puts(arte[i]);
    }
    printf("\n");
}

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    mostrar_portada();

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
        printf(" 3. Ver resultados detallados de la última misión\n");
        printf(" 4. Ver historial de simulaciones\n");
        printf(" 5. Ver gráfico de trayectoria\n");
        printf(" 6. Ejecutar un módulo por separado\n");
        printf(" 7. Salir del sistema\n");
        printf("--------------------------------------------------\n");

        opcion = leer_opcion(1, 7);

        switch (opcion) {
            case 1:
                datos = solicitar_datos_entrada();  /* MÓDULO 1 */
                if (validaciones_hubo_fin_entrada()) return EXIT_FAILURE;

                simular(&datos, &res);
                simulaciones++;
                registrar_simulacion(TIPO_SIMULACION_PERSONALIZADA,
                                     res.hmax / 1000.0);

                printf("\nSimulación completada.\n");
                break;

            case 2:
                datos = cargar_datos_enunciado();   /* MÓDULO 1 */

                simular(&datos, &res);
                simulaciones++;
                registrar_simulacion(TIPO_SIMULACION_OFICIAL,
                                     res.hmax / 1000.0);

                printf("\nSimulación completada con los datos del enunciado.\n");
                break;

            case 4:
                mostrar_historial();
                pausar_pantalla();
                break;

            case 3:
                if (simulaciones > 0) {
                    aceleracion_g_y_presentacion(&datos, &res); /* MÓDULO 5 */
                    pausar_pantalla();
                } else {
                    printf("\n[!] Primero debe simular (opción 1 o 2).\n");
                }
                break;

            case 5:
                if (simulaciones > 0) {
                    mostrar_grafico_trayectoria_ascii(&datos, &res);
                    pausar_pantalla();
                } else {
                    printf("\n[!] Primero debe simular (opción 1 o 2).\n");
                }
                break;

            case 6:
                ejecutar_modulo_individual();
                break;

            case 7:
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

    } while (opcion != 7);

    return 0;
}
