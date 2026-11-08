/*
 * artemis.h
 * Resultados de la simulación y prototipos de los módulos 2 a 5.
 * Los datos de entrada vienen de DatosSimulacion (validaciones.h).
 */
#ifndef ARTEMIS_H
#define ARTEMIS_H
 
#include "validaciones.h"

/* Resultados que van calculando los módulos 2, 3 y 4 */
typedef struct {
    double v1, y1;        /* fase 1, al desacople (módulo 2) */
    double v2;            /* rapidez al apagar los motores (módulo 3) */
    double hmax;          /* altitud máxima en caída libre (módulo 4) */
    int    hay_fase2;     /* 1 si el desacople ocurre antes de llegar a h */
} ResultadosSimulacion;
 
/* MÓDULO 1: Entrada y validación de datos
   La función principal es solicitar_datos_entrada() (validaciones.h).
   Esta carga los valores del enunciado sin preguntar. */
DatosSimulacion cargar_datos_enunciado(void);
 
/* MÓDULO 2: Fase 1 y chequeo de coherencia */
void fase1_chequeo_coherencia(const DatosSimulacion *d, ResultadosSimulacion *r);
 
/* MÓDULO 3: Fase 2 (MRUV con velocidad inicial) */
void fase2_mruv_velocidad_inicial(const DatosSimulacion *d, ResultadosSimulacion *r);
 
/* MÓDULO 4: Caída libre tras el apagado */
void caida_libre_tras_apagado(const DatosSimulacion *d, ResultadosSimulacion *r);
 
/* MÓDULO 5: Aceleración en múltiplos de g y presentación */
double aceleracion_en_g(double a, double g);
void aceleracion_g_y_presentacion(const DatosSimulacion *d,
                                  const ResultadosSimulacion *r);
 
#endif
 