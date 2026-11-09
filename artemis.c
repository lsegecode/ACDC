/*
 * artemis.c
 * Lógica de la simulación: módulos 2, 3, 4 y 5 (y carga del enunciado).
 */
#include <stdio.h>
#include <math.h>
#include "artemis.h"

#define SEPARADOR "=============================================="
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
    r->tmax = r->v2 / d->g;
}

double altura_ascenso_libre(double h, double v2, double g, double t)
{
    return h + v2 * t - 0.5 * g * t * t;
}

double velocidad_ascenso_libre(double v2, double g, double t)
{
    return v2 - g * t;
}

/* =====================================================================
 * MÓDULO 5: Aceleración en múltiplos de g y presentación
 * Parte A: aceleracion_en_g (pregunta 4).
 * Parte B: aceleracion_g_y_presentacion (un bloque por pregunta).
 * ===================================================================== */

double aceleracion_en_g(double a, double g)
{
    if (g <= 0.0) {
        return 0.0;  /* protección: las validaciones ya impiden g <= 0 */
    }
    return a / g;
}
static void mostrar_datos_mision(const DatosSimulacion *d)
{
    printf("\nDATOS DE LA MISIÓN\n");
    printf("    %-28s %12.1f m/s^2\n", "Aceleración fase 1 (a1):", d->a1);
    printf("    %-28s %12.1f s\n",     "Tiempo hasta desacople (t1):", d->t1);
    printf("    %-28s %12.1f m/s^2\n", "Aceleración fase 2 (a2):", d->a2);
    printf("    %-28s %12.1f m\n",     "Altitud de apagado (h):", d->h);
    printf("    %-28s %12.2f m/s^2\n", "Gravedad (g):", d->g);
}
static void mostrar_pregunta1(const DatosSimulacion *d, const ResultadosSimulacion *r)
{
    printf("\n[1] Al desacople de los SRB (t = %.1f s)\n", d->t1);
    if (r->hay_fase2) {
        printf("    %-28s %12.1f m/s\n", "Rapidez v1:", r->v1);
        printf("    %-28s %12.1f m\n",   "Altitud y1:", r->y1);
        printf("    %-28s %12.2f km\n",  "Altitud y1 (km):", r->y1 / 1000.0);
    } else {
        printf("    No se alcanza el desacople: la nave llega a h = %.1f m\n", d->h);
        printf("    antes de t1 (y1 teórica = %.1f m >= h).\n", r->y1);
    }
}
static void mostrar_pregunta2(const DatosSimulacion *d, const ResultadosSimulacion *r)
{
    printf("\n[2] Rapidez al apagar los motores (h = %.1f m)\n", d->h);
    printf("    %-28s %12.1f m/s\n", "Rapidez v2:", r->v2);
    if (!r->hay_fase2) {
        printf("    (calculada solo con la fase 1)\n");
    }
}

static void mostrar_pregunta3(const DatosSimulacion *d, const ResultadosSimulacion *r)
{
    printf("\n[3] Altitud máxima si los motores se apagan en h\n");
    printf("    %-28s %12.1f m\n",  "Ascenso extra:", r->hmax - d->h);
    printf("    %-28s %12.1f m\n",  "Altitud máxima h_max:", r->hmax);
    printf("    %-28s %12.2f km\n", "Altitud máxima h_max (km):", r->hmax / 1000.0);
    printf("    %-28s %12.2f s\n", "Tiempo hasta altura máxima:", r->tmax);
    printf("\n    Ascenso sin motores\n");
    printf("    %-12s %-18s %-16s\n", "Tiempo (s)", "Altura (m)", "Velocidad (m/s)");
    for (int i = 0; i <= 4; i++) {
        double tiempo = (i == 4) ? r->tmax : r->tmax * i / 4.0;
        double altura = altura_ascenso_libre(d->h, r->v2, d->g, tiempo);
        double velocidad = velocidad_ascenso_libre(r->v2, d->g, tiempo);
        printf("    %-12.2f %-18.1f %-16.2f\n", tiempo, altura, velocidad);
    }
}

static void mostrar_pregunta4(const DatosSimulacion *d)
{
    printf("\n[4] Aceleración máxima de la primera fase\n");
    printf("    %-28s %12.2f g\n", "a1 / g:", aceleracion_en_g(d->a1, d->g));
    printf("    %-28s %12.2f g\n", "Percibida (a1 + g) / g:",
           aceleracion_en_g((double)d->a1 + d->g, d->g));
    printf("    (a1/g es la aceleracion de la nave; (a1+g)/g es lo que siente\n");
    printf("     la tripulación, porque además sostiene su propio peso)\n");
}

void aceleracion_g_y_presentacion(const DatosSimulacion *d,
                                  const ResultadosSimulacion *r)
{
    printf("\n%s\n", SEPARADOR);
    printf("   RESULTADOS - ASCENSO DEL SLS / ORION\n");
    printf("%s\n", SEPARADOR);

    mostrar_datos_mision(d);
    mostrar_pregunta1(d, r);
    mostrar_pregunta2(d, r);
    mostrar_pregunta3(d, r);
    mostrar_pregunta4(d);

    printf("%s\n", SEPARADOR);
}

/* =====================================================================
 * Pausa de pantalla para mejorar la experiencia de usuario
 * ===================================================================== */
void pausar_pantalla(void) {
    printf("\n----------------------------------------------\n");
    printf(" Presione [Enter] para volver al menu principal...");
    while (getchar() != '\n' && !feof(stdin)) {
        /* Espera un Enter; leer_opcion ya consumió el salto de línea anterior. */
    }
}

void reporte_final(int simulaciones, const ResultadosSimulacion *ultimo)
{
    printf("\n==============================================\n");
    printf("   REPORTE FINAL DE LA SESIÓN\n");
    printf("==============================================\n");
    printf("Simulaciones realizadas: %d\n", simulaciones);

    if (simulaciones > 0) {
        printf("Última simulación:\n");
        printf("  Rapidez al apagado v2 = %.1f m/s\n", ultimo->v2);
        printf("  Altitud máxima h_max  = %.2f km\n", ultimo->hmax / 1000.0);
    } else {
        printf("No se realizó ninguna simulación.\n");
    }
    printf("==============================================\n");
}
