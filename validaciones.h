#ifndef VALIDACIONES_H
#define VALIDACIONES_H

// Estructura que empaqueta los 5 datos de entrada para compartirlos con otros módulos
typedef struct {
    float a1; // Aceleración Fase 1 (m/s²)
    float t1; // Tiempo inicial hasta desacople SRB (s)
    float a2; // Aceleración Fase 2 (m/s²)
    float h;  // Altitud de apagado de motores (m)
    float g;  // Aceleración de la gravedad (m/s²)
} DatosSimulacion;

// Prototipos de funciones
float leer_flotante_con_defecto(const char *mensaje, float valor_defecto);
float leer_altitud(const char *mensaje, float valor_defecto_m);
DatosSimulacion solicitar_datos_entrada(void);

#endif