
#include <stdio.h>

int main(void) {
    double aceleracion;
    double tiempo;
    double velocidad;

    printf("Ingrese la aceleracion (m/s2): ");
    scanf("%lf", &aceleracion);

    printf("Ingrese el tiempo (s): ");
    scanf("%lf", &tiempo);

    velocidad = aceleracion * tiempo;

    printf("Velocidad final: %.2f m/s\n", velocidad);

    return 0;
}
