/*
 * artemis.c
 * Lógica de la simulación: módulos 2, 3, 4 y 5 (y carga del enunciado).
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"

/* =====================================================================
 * MÓDULO 1: Entrada y validación de datos (carga del enunciado)
 * La lectura con validaciones está en validaciones.c.
 * ===================================================================== */

DatosSimulacion cargar_datos_enunciado(void)
{
    DatosSimulacion d;
    d.a1 = 20.0f;
    d.t1 = 120.0f;
    d.a2 = 12.5f;
    d.h  = 160000.0f;
    d.g  = 9.80f;
    return d;
}

/* =====================================================================
 * MÓDULO 2: Fase 1 y chequeo de coherencia
 * Entrada: a1, t1, h. Salida: v1, y1 y si existe la fase 2.
 * Responde la pregunta 1.
 * ===================================================================== */

void fase1_chequeo_coherencia(const DatosSimulacion *d, ResultadosSimulacion *r)
{
    r->v1 = (double)d->a1 * d->t1;                         /* v1 = a1 * t1         */
    r->y1 = 0.5 * (double)d->a1 * d->t1 * d->t1;           /* y1 = 1/2 * a1 * t1^2 */

    /* Si y1 >= h, la nave llega a h antes del desacople: no hay fase 2 */
    r->hay_fase2 = (r->y1 < d->h) ? 1 : 0;
}

/* =====================================================================
 * MÓDULO 3: Fase 2 (MRUV con velocidad inicial)
 * Entrada: v1, y1, a2, h. Salida: v2.
 * Responde la pregunta 2.
 * ===================================================================== */

void fase2_mruv_velocidad_inicial(const DatosSimulacion *d, ResultadosSimulacion *r)
{
    double distancia;

    if (r->hay_fase2) {
        distancia = d->h - r->y1;                          /* lo que falta subir   */
        r->v2 = sqrt(r->v1 * r->v1 + 2.0 * d->a2 * distancia);
    } else {
        /* Solo hubo fase 1: v^2 = 2 * a1 * h */
        r->v2 = sqrt(2.0 * d->a1 * d->h);
    }
}

/* =====================================================================
 * MÓDULO 4: Caída libre tras el apagado
 * Entrada: v2, h, g. Salida: hmax = h + v2^2 / (2g).
 * Responde la pregunta 3.
 * ===================================================================== */

void caida_libre_tras_apagado(const DatosSimulacion *d, ResultadosSimulacion *r)
{
    double ascenso_extra = (r->v2 * r->v2) / (2.0 * d->g);
    r->hmax = d->h + ascenso_extra;
}

/* =====================================================================
 * MÓDULO 5: Aceleración en múltiplos de g y presentación
 * Parte A: aceleracion_en_g (pregunta 4).
 * Parte B: aceleracion_g_y_presentacion (un bloque por pregunta).
 * ===================================================================== */

double aceleracion_en_g(double a, double g)
{
    if (g <= 0.0) {
        return 0.0;  //* protección: las validaciones ya impiden g <= 0 */
    }
    return a / g;
}

void aceleracion_g_y_presentacion(const DatosSimulacion *d,
                                  const ResultadosSimulacion *r)
{
    printf("\n==============================================\n");
    printf("   RESULTADOS - ASCENSO DEL SLS / ORION\n");
    printf("==============================================\n");

    printf("\n[1] Al desacople de los SRB (t = %.1f s)\n", d->t1);
    if (r->hay_fase2) {
        printf("    Rapidez v1    = %.1f m/s\n", r->v1);
        printf("    Altitud y1    = %.1f m (%.2f km)\n", r->y1, r->y1 / 1000.0);
    } else {
        printf("    No se alcanza el desacople: la nave llega a h = %.1f m\n", d->h);
        printf("    antes de t1 (y1 teorica = %.1f m >= h).\n", r->y1);
    }

    printf("\n[2] Rapidez al apagar los motores (h = %.1f m)\n", d->h);
    printf("    Rapidez v2    = %.1f m/s\n", r->v2);
    if (!r->hay_fase2) {
        printf("    (calculada solo con la fase 1)\n");
    }

    printf("\n[3] Altitud maxima si los motores se apagan en h\n");
    printf("    Ascenso extra = %.1f m\n", r->hmax - d->h);
    printf("    h_max         = %.1f m (%.2f km)\n", r->hmax, r->hmax / 1000.0);

    printf("\n[4] Aceleracion maxima de la primera fase\n");
    printf("    a1 / g                 = %.2f g\n", aceleracion_en_g(d->a1, d->g));
    printf("    Percibida (a1 + g) / g = %.2f g\n",
           aceleracion_en_g((double)d->a1 + d->g, d->g));
    printf("==============================================\n");
}

/* =====================================================================
 * Pausa de pantalla para mejorar la experiencia de usuario
 * ===================================================================== */
void pausar_pantalla(void) {
    printf("\n----------------------------------------------\n");
    printf(" Presione [Enter] para volver al menu principal...");
    while (getchar() != '\n'); // Limpia el buffer residual
    getchar();                 // Espera a que el usuario presione Enter para que continue 
}
