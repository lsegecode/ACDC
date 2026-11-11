#include "artemis.h"

#include <math.h>
#include <stdio.h>

#include "validaciones.h"

/* ====================================================================
   MODULO 1: entrada y validacion de datos
   ==================================================================== */

Datos modulo1_entrada(void)
{
    Datos d;
    printf("\n[Módulo 1] Cargá los datos de la Artemis (Enter = valor por defecto).\n");
    d.a1 = leer_numero("a1 (aceleración fase 1)", "m/s^2", 20.0, false);
    d.t1 = leer_numero("t1 (tiempo hasta desacople SRB)", "s", 120.0, false);
    d.a2 = leer_numero("a2 (aceleración fase 2)", "m/s^2", 12.5, false);
    d.h  = leer_numero("h (altitud de apagado, podés escribir 160 km)", "m", 160000.0, true);
    d.g  = leer_numero("g (gravedad)", "m/s^2", 9.80, false);
    return d;
}

/* ====================================================================
   MODULO 2: fase 1 y chequeo de coherencia
   ==================================================================== */

ResultadoFase1 modulo2_fase1(double a1, double t1, double h)
{
    ResultadoFase1 r;
    r.v1 = a1 * t1;                 /* v1 = a1 * t1 */
    r.y1 = 0.5 * a1 * t1 * t1;      /* y1 = 1/2 * a1 * t1^2 (t al cuadrado) */
    r.hay_fase2 = (r.y1 < h);
    r.v_apagado = 0.0;

    if (!r.hay_fase2)               /* no hay fase 2: v^2 = 2*a1*h */
        r.v_apagado = sqrt(2.0 * a1 * h);

    return r;
}

void mostrar_fase1(const ResultadoFase1 *r)
{
    printf("\nResultados de la fase 1 de la Artemis:\n");
    printf("  v1 = %s m/s\n", fmt(r->v1, 1));
    printf("  y1 = %s m (%s km)\n", fmt(r->y1, 1), fmt(r->y1 / 1000.0, 2));

    if (r->hay_fase2) {
        printf("  Hay fase 2: y1 < h.\n");
    } else {
        printf("  ⚠ y1 >= h: la Artemis llega a h antes del desacople. No hay fase 2.\n");
        printf("  Rapidez al apagado (v^2 = 2*a1*h): %s m/s\n", fmt(r->v_apagado, 1));
    }
}

/* ====================================================================
   MODULO 3: fase 2 (MRUV con velocidad inicial)
   ==================================================================== */

double modulo3_fase2(double v1, double y1, double a2, double h)
{
    double delta_y = h - y1;                         /* lo que falta subir */
    double v2_cuad = v1 * v1 + 2.0 * a2 * delta_y;   /* v2^2 = v1^2 + 2*a2*dy */
    return sqrt(v2_cuad);
}

/* ====================================================================
   MODULO 4: caida libre tras el apagado
   ==================================================================== */

ResultadoCaida modulo4_caida_libre(double v2, double h, double g)
{
    ResultadoCaida c;
    c.delta_h = (v2 * v2) / (2.0 * g);   /* dh = v2^2 / (2g) */
    c.h_max   = h + c.delta_h;           /* no olvidar sumar h */
    return c;
}

/* ====================================================================
   MODULO 5: multiplos de g y presentacion
   ==================================================================== */

void mostrar_multiplos_de_g(double a1, double g)
{
    printf("\nAceleración máxima de la fase 1: %s g\n", fmt(a1 / g, 2));
    printf("(Opcional) Lo que \"siente\" la tripulación, (a1 + g)/g: %s g\n",
           fmt((a1 + g) / g, 2));
}

void modulo5_presentacion(const Datos *d, const ResultadoFase1 *f1,
                          double v2, const ResultadoCaida *c)
{
    printf("\n============ RESULTADOS DE LA ARTEMIS ============\n");

    printf("\nPregunta 1 — Fase 1 (hasta el desacople)\n");
    printf("  v1 = %s m/s\n", fmt(f1->v1, 1));
    printf("  y1 = %s m (%s km)\n", fmt(f1->y1, 1), fmt(f1->y1 / 1000.0, 2));

    printf("\nPregunta 2 — Rapidez al apagado de los motores\n");
    if (f1->hay_fase2) {
        printf("  v2 = %s m/s\n", fmt(v2, 1));
    } else {
        printf("  La fase 2 no aplica: la Artemis llegó a h antes del desacople.\n");
        printf("  Rapidez al apagado = %s m/s\n", fmt(v2, 1));
    }

    printf("\nPregunta 3 — Altitud máxima\n");
    printf("  dh (sube tras el apagado) = %s m (%s km)\n",
           fmt(c->delta_h, 1), fmt(c->delta_h / 1000.0, 2));
    printf("  h_max = %s m (%s km)\n", fmt(c->h_max, 1), fmt(c->h_max / 1000.0, 2));

    printf("\nPregunta 4 — Aceleración máxima de la fase 1\n");
    mostrar_multiplos_de_g(d->a1, d->g);

    printf("\n==================================================\n");
}

/* ====================================================================
   Flujos
   ==================================================================== */

void programa_completo(void)
{
    printf("\n--- PROGRAMA COMPLETO: ARTEMIS ---\n");

    Datos d = modulo1_entrada();
    ResultadoFase1 f1 = modulo2_fase1(d.a1, d.t1, d.h);

    double v2 = f1.hay_fase2
                    ? modulo3_fase2(f1.v1, f1.y1, d.a2, d.h)
                    : f1.v_apagado;

    ResultadoCaida c = modulo4_caida_libre(v2, d.h, d.g);
    modulo5_presentacion(&d, &f1, v2, &c);
}

void menu_modulos(void)
{
    bool volver = false;

    while (!volver) {
        printf("\n--- MÓDULOS INDIVIDUALES (ARTEMIS) ---\n");
        printf("1) Módulo 1: Entrada y validación de datos\n");
        printf("2) Módulo 2: Fase 1 y chequeo de coherencia\n");
        printf("3) Módulo 3: Fase 2 (MRUV con velocidad inicial)\n");
        printf("4) Módulo 4: Caída libre tras el apagado\n");
        printf("5) Módulo 5: Aceleración en múltiplos de g\n");
        printf("0) Volver al menú principal\n");

        switch (leer_opcion("Elegí un módulo", 0, 5)) {

        case 1: {
            Datos d = modulo1_entrada();
            printf("\nDatos validados:\n");
            printf("  a1 = %s m/s^2\n", fmt(d.a1, 2));
            printf("  t1 = %s s\n",     fmt(d.t1, 2));
            printf("  a2 = %s m/s^2\n", fmt(d.a2, 2));
            printf("  h  = %s m\n",     fmt(d.h, 2));
            printf("  g  = %s m/s^2\n", fmt(d.g, 2));
            break;
        }

        case 2: {
            printf("\nDatos que necesita el módulo 2:\n");
            double a1 = leer_numero("a1 (aceleración fase 1)", "m/s^2", 20.0, false);
            double t1 = leer_numero("t1 (tiempo hasta desacople)", "s", 120.0, false);
            double h  = leer_numero("h (altitud de apagado, podés escribir 160 km)",
                                    "m", 160000.0, true);
            ResultadoFase1 r = modulo2_fase1(a1, t1, h);
            mostrar_fase1(&r);
            break;
        }

        case 3: {
            printf("\nDatos que necesita el módulo 3:\n");
            double v1 = leer_numero("v1 (rapidez al desacople)", "m/s", 2400.0, false);
            double y1 = leer_numero("y1 (altura al desacople)", "m", 144000.0, false);
            double a2 = leer_numero("a2 (aceleración fase 2)", "m/s^2", 12.5, false);
            double h  = leer_numero("h (altitud de apagado, podés escribir 160 km)",
                                    "m", 160000.0, true);
            if (y1 >= h) {
                printf("\n⚠ y1 >= h: no existe fase 2 (la Artemis llega a h antes del desacople).\n");
                printf("  Usá el módulo 2 para calcular la rapidez en ese caso.\n");
            } else {
                double v2 = modulo3_fase2(v1, y1, a2, h);
                printf("\nRapidez al apagado: v2 = %s m/s\n", fmt(v2, 1));
            }
            break;
        }

        case 4: {
            printf("\nDatos que necesita el módulo 4:\n");
            double v2 = leer_numero("v2 (rapidez al apagado)", "m/s", 2481.94, false);
            double h  = leer_numero("h (altitud de apagado, podés escribir 160 km)",
                                    "m", 160000.0, true);
            double g  = leer_numero("g (gravedad)", "m/s^2", 9.80, false);
            ResultadoCaida c = modulo4_caida_libre(v2, h, g);
            printf("\nLa Artemis sube %s m más después del apagado.\n", fmt(c.delta_h, 1));
            printf("Altitud máxima: %s m (%s km)\n",
                   fmt(c.h_max, 1), fmt(c.h_max / 1000.0, 2));
            break;
        }

        case 5: {
            printf("\nDatos que necesita el módulo 5 (parte A):\n");
            double a1 = leer_numero("a1 (aceleración fase 1)", "m/s^2", 20.0, false);
            double g  = leer_numero("g (gravedad)", "m/s^2", 9.80, false);
            mostrar_multiplos_de_g(a1, g);
            break;
        }

        default:
            volver = true;
            break;
        }
    }
}
