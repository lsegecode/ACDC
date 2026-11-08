# Funciones y punteros en C

Guía de referencia rápida — Laboratorio de C / Simulador Artemis.

## 1. Funciones

Una función agrupa instrucciones reutilizables. En C se declara el tipo de dato que devuelve y el tipo de cada parámetro.

```c
double calcular_velocidad(double aceleracion, double tiempo) {
    return aceleracion * tiempo;
}
```

Uso:

```c
double velocidad = calcular_velocidad(20, 120); // 2400
```

`return` devuelve el resultado y termina la ejecución de esa función.

## 2. Procedimientos (`void`)

En C no existe una palabra reservada `procedure`. Se suele llamar *procedimiento* a una función `void`, que no devuelve un valor.

```c
void mostrar_velocidad(double velocidad) {
    printf("Velocidad: %.2f m/s\n", velocidad);
}
```

Uso: `mostrar_velocidad(2400);`.

**Nota:** una función `void` puede modificar variables externas mediante punteros, aunque no devuelva un valor con `return`.

## 3. Parámetros por valor

En C, los parámetros se pasan por valor: la función recibe una copia del argumento.

```c
void duplicar(double numero) {
    numero = numero * 2;
}

int main(void) {
    double velocidad = 100;
    duplicar(velocidad);
    printf("%.0f\n", velocidad); // 100
    return 0;
}
```

La variable original sigue valiendo `100`.

## 4. ¿Qué es un puntero?

Un puntero es una variable que almacena una **dirección de memoria**.

```c
double velocidad = 100;
double *puntero = &velocidad;
```

- `velocidad`: el valor (`100`).
- `&velocidad`: la dirección de memoria de `velocidad`.
- `puntero`: guarda esa dirección.
- `*puntero`: accede al valor que hay en esa dirección (`100`).

```c
*puntero = 200;
printf("%.0f\n", velocidad); // 200
```

`puntero` **no contiene la variable**: contiene su dirección. `*puntero` permite acceder a la variable apuntada.

## 5. Los dos usos de `*`

```c
double *p;       // DECLARACIÓN: p es un puntero a double
p = &velocidad;  // p apunta a velocidad
*p = 300;        // DESREFERENCIA: cambia el valor apuntado
```

El significado depende del contexto: en una declaración indica que la variable es puntero; en una expresión accede al valor apuntado. `*` también es el operador de multiplicación (`a * b`).

## 6. Pasar una dirección a una función

```c
#include <stdio.h>

void duplicar(double *numero) {
    *numero = *numero * 2;
}

int main(void) {
    double velocidad = 100;
    duplicar(&velocidad);
    printf("%.0f\n", velocidad); // 200
    return 0;
}
```

**Importante:** C sigue pasando una copia, pero esta vez copia la **dirección**, no el valor de `velocidad`. Por eso la función puede modificar la variable original.

| Concepto | Sin puntero | Con puntero |
|---|---|---|
| Parámetro | `double numero` | `double *numero` |
| Llamada | `duplicar(velocidad)` | `duplicar(&velocidad)` |
| Modificación | `numero = 200;` | `*numero = 200;` |
| Cambia la original | No | Sí |

## 7. Devolver dos resultados usando punteros

Una función en C devuelve como máximo un valor directamente con `return`. Para producir varios resultados se pueden usar parámetros puntero.

```c
#include <stdio.h>

void calcular_fase1(double aceleracion, double tiempo,
                    double *velocidad, double *altura) {
    *velocidad = aceleracion * tiempo;
    *altura = 0.5 * aceleracion * tiempo * tiempo;
}

int main(void) {
    double velocidad = 0;
    double altura = 0;

    calcular_fase1(20, 120, &velocidad, &altura);

    printf("Velocidad: %.2f m/s\n", velocidad); // 2400.00
    printf("Altura: %.2f m\n", altura);          // 144000.00
    return 0;
}
```

Este patrón te puede servir para el simulador Artemis.

## 8. Precauciones

- **No desreferencies un puntero sin inicializar:** `double *p; *p = 10;` es incorrecto.
- Usá `&variable` para obtener una dirección válida de una variable existente.
- Un puntero puede cambiar para apuntar a otra variable compatible.
- Un puntero puede ser `NULL`: no uses `*p` si `p == NULL`.
- No devuelvas la dirección de una variable local que deja de existir al terminar la función.

## 9. Ejercicios

**Ejercicio 1 — Por valor.** Predecí la salida:

```c
void cambiar(int x) { x = 50; }
int main(void) {
    int numero = 10;
    cambiar(numero);
    printf("%d\n", numero);
    return 0;
}
```

**Ejercicio 2 — Con puntero.** Predecí la salida:

```c
void cambiar(int *x) { *x = 50; }
int main(void) {
    int numero = 10;
    cambiar(&numero);
    printf("%d\n", numero);
    return 0;
}
```

**Ejercicio 3 — Artemis.** Escribí `void calcular_fase1(double a, double t, double *v, double *h)` para calcular velocidad (`a*t`) y altura (`0.5*a*t*t`). Probá con `a=20` y `t=120`.

## 10. Compilar y ejecutar (Windows)

```powershell
gcc -Wall -Wextra main.c -o programa.exe
.\programa.exe
```

**Resumen:** `&` obtiene una dirección; `*` en una declaración crea un puntero; `*` en una expresión accede al valor apuntado. Los punteros no se usan solamente en funciones.
