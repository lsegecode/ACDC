
#include <stdio.h>


double calcular_altura(double velocidad_inicial, double altura_inicial){
    double desplazamiento;

    printf("Se considera a la gravedad como: 9.8m/s^2\n");

    desplazamiento = (velocidad_inicial * velocidad_inicial) / 2 * 9.8;

    printf("El desplazamiento del cohete con los motores apagados es de: %.2f metros.\n", desplazamiento);

    return altura_inicial + desplazamiento;    
    
}

int main(void) {

    // Supongamos que acá los demás programadores establecieron las condiciones iniciales

    double velocidad_inicial;    
    double altura_inicial;
    double altura_final;


    printf("Ingrese la velocidad inicial:");
    scanf("%lf", &velocidad_inicial);

    printf("Ingrese la altura inicial:");
    scanf("%lf", &altura_inicial);
    
    altura_final = calcular_altura(velocidad_inicial, altura_inicial);

    printf("La altura final es: %.2f", altura_final);

    return 0;

}
