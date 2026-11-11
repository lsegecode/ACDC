#ifndef ARTEMIS_H
#define ARTEMIS_H

#include <stdbool.h>

/* ---------- Tipos ---------- */

/* Datos de entrada de la Artemis (modulo 1) */
typedef struct {
    double a1;  /* aceleracion fase 1 (m/s^2) */
    double t1;  /* tiempo hasta el desacople de los SRB (s) */
    double a2;  /* aceleracion fase 2 (m/s^2) */
    double h;   /* altitud de apagado de motores (m) */
    double g;   /* gravedad (m/s^2) */
} Datos;

/* Resultado del modulo 2 */
typedef struct {
    double v1;         /* rapidez al desacople (m/s) */
    double y1;         /* altura al desacople (m) */
    bool   hay_fase2;  /* true si y1 < h */
    double v_apagado;  /* solo vale si NO hay fase 2: v = sqrt(2*a1*h) */
} ResultadoFase1;

/* Resultado del modulo 4 */
typedef struct {
    double delta_h;  /* cuanto sube despues del apagado (m) */
    double h_max;    /* altitud maxima desde la plataforma (m) */
} ResultadoCaida;

/* ---------- Modulos ---------- */

Datos          modulo1_entrada(void);
ResultadoFase1 modulo2_fase1(double a1, double t1, double h);
void           mostrar_fase1(const ResultadoFase1 *r);
double         modulo3_fase2(double v1, double y1, double a2, double h);
ResultadoCaida modulo4_caida_libre(double v2, double h, double g);
void           mostrar_multiplos_de_g(double a1, double g);
void           modulo5_presentacion(const Datos *d, const ResultadoFase1 *f1,
                                    double v2, const ResultadoCaida *c);

/* ---------- Flujos ---------- */

void programa_completo(void);  /* modulos 1 -> 5 encadenados */
void menu_modulos(void);       /* uso de cada modulo por separado */

#endif /* ARTEMIS_H */
