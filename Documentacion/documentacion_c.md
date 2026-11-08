# Documentación de C

Material de referencia sobre fundamentos, estructuras de control, funciones y punteros en C.

---

## Fundamentos de C

# Fundamentos de C — Guía rápida

> Para quienes ya saben programar y necesitan familiarizarse con la sintaxis de C. Ejemplos compatibles con GCC.

## 1. Declarar variables

En C, **cada variable tiene un tipo** y debe declararse antes de usarla.

```c
int cantidad = 5;           // Entero
double aceleracion = 20.0;  // Decimal de mayor precisión
float temperatura = 25.5f;  // Decimal de menor precisión
char inicial = 'A';         // Un carácter
char nombre[30] = "Orion";  // Texto: arreglo de caracteres
```

`char` usa comillas simples para un carácter; los textos usan comillas dobles.

### Declarar primero, asignar después

```c
double tiempo;       // Declaración: todavía no tiene valor definido
tiempo = 120.0;      // Primera asignación
tiempo = 130.0;      // Reasignación
```

También se puede hacer todo junto:

```c
double tiempo = 120.0;
```

**Importante:** una variable local sin inicializar tiene un valor indeterminado. No la leas antes de asignarle un valor.

No podés hacer `tiempo = 120.0;` si `tiempo` no fue declarada previamente.

## 2. Declarar constantes

Con `const` definís un valor que no puede modificarse después de inicializarlo:

```c
const double GRAVEDAD = 9.8;
// GRAVEDAD = 10.0; // Error: no se puede reasignar
```

Otra opción es `#define`, que hace una sustitución durante el preprocesamiento:

```c
#define GRAVEDAD 9.8
```

Para empezar, preferí `const` cuando quieras una constante con tipo. En el simulador, **la gravedad podría ser una variable** si el usuario debe poder modificarla.

## 3. Mostrar datos con `printf`

`printf` imprime texto y reemplaza los **especificadores de formato** (`%...`) por los argumentos que siguen a la cadena.

```c
int fase = 2;
float temperatura = 25.5f;
double velocidad = 2481.934;
char letra = 'A';
char nave[] = "Orion";

printf("Fase: %d\n", fase);
printf("Temperatura: %.1f\n", temperatura);
printf("Velocidad: %.2f m/s\n", velocidad);
printf("Letra: %c\n", letra);
printf("Nave: %s\n", nave);
```

Salida:

```text
Fase: 2
Temperatura: 25.5
Velocidad: 2481.93 m/s
Letra: A
Nave: Orion
```

En `%.2f`, `%` inicia el formato, `.2` pide **dos decimales** y `f` indica representación decimal de punto flotante. `\n` agrega un salto de línea.

## 4. Leer datos con `scanf`

`scanf` lee la entrada según el formato indicado. Para variables numéricas simples se utiliza `&` porque la función necesita **la dirección de memoria donde escribir**.

```c
int fase;
float temperatura;
double aceleracion;
char letra;
char nombre[30];

scanf("%d", &fase);
scanf("%f", &temperatura);
scanf("%lf", &aceleracion);
scanf(" %c", &letra);  // Espacio: omite saltos de línea pendientes
scanf("%29s", nombre); // Sin &: el arreglo ya se usa como dirección
```

**Atención:** `%29s` lee una sola palabra (hasta el primer espacio) y evita exceder el arreglo de 30 caracteres. Para nombres con espacios y entradas robustas conviene `fgets`.

### Tabla rápida de formatos

| Tipo | Ejemplo de declaración | `printf` | `scanf` |
|---|---|---|---|
| Entero `int` | `int n;` | `%d` | `%d` |
| Decimal `float` | `float x;` | `%f` o `%.2f` | `%f` |
| Decimal `double` | `double x;` | `%f` o `%.2f` | `%lf` |
| Carácter `char` | `char c;` | `%c` | ` %c` |
| Texto `char[]` | `char s[30];` | `%s` | `%29s` |

**Detalle importante:** `printf` usa `%f` tanto para `float` como para `double` (el `float` se promociona a `double`). En cambio, `scanf` usa `%f` para `float` y `%lf` para `double`.

## 5. Ejemplo completo: primera fase del cohete

Guardá esto como `main.c`:

```c
#include <stdio.h>

int main(void) {
    double aceleracion;
    double tiempo;
    double velocidad;

    printf("Aceleracion (m/s2): ");
    if (scanf("%lf", &aceleracion) != 1) {
        printf("Error: ingresa un numero valido.\n");
        return 1;
    }

    printf("Tiempo (s): ");
    if (scanf("%lf", &tiempo) != 1) {
        printf("Error: ingresa un numero valido.\n");
        return 1;
    }

    velocidad = aceleracion * tiempo;
    printf("Velocidad final: %.2f m/s\n", velocidad);

    return 0;
}
```

Compilar y ejecutar desde PowerShell:

```powershell
gcc -Wall -Wextra main.c -o programa.exe
.\programa.exe
```

Ejemplo de entrada: `20` y `120`. Salida: `Velocidad final: 2400.00 m/s`.

> **Limitación intencional:** este ejemplo solo comprueba si `scanf` pudo leer un número. Todavía no valida rangos, entradas completas ni comas decimales (`9,8`). Para el trabajo final usaremos `fgets` y conversión validada, de modo que el programa no falle con entradas incorrectas.

## 6. Recordatorio rápido

- **Declarar:** `double altura;`
- **Asignar:** `altura = 160000.0;`
- **Declarar y asignar:** `double altura = 160000.0;`
- **Constante:** `const double PI = 3.14159;`
- **Imprimir un `double`:** `printf("%.2f", altura);`
- **Leer un `double`:** `scanf("%lf", &altura);`
- **`&altura`:** dirección de memoria de la variable, no su valor.

---

## Condicionales y bucles en C

# Condicionales y estructuras repetitivas en C

Guía rápida de referencia para el laboratorio **Artemis SLS**.

## 1. Condicionales: `if`, `else if`, `else`

```c
#include <stdio.h>

int main(void) {
    double altura = 160000.0;

    if (altura > 160000.0) {
        printf("Superamos la altura objetivo\n");
    } else if (altura == 160000.0) {
        printf("Estamos en la altura objetivo\n");
    } else {
        printf("Seguimos ascendiendo\n");
    }

    return 0;
}
```

> `=` asigna; `==` compara. Evitá comparar resultados calculados con decimales usando `==` cuando pueda haber errores de redondeo.

### Operadores

| Operador | Significado |
|---|---|
| `==` | Igual a |
| `!=` | Distinto de |
| `>` / `<` | Mayor / menor |
| `>=` / `<=` | Mayor o igual / menor o igual |
| `&&` | Y lógico (AND) |
| `\|\|` | O lógico (OR) |
| `!` | Negación (NOT) |

```c
if (tiempo >= 0 && tiempo <= 120) {
    printf("Primera fase\n");
}
```

## 2. Selección múltiple: `switch`

Útil para un menú. Los `case` usan valores enteros (o caracteres), no `double` ni cadenas.

```c
int opcion = 2;

switch (opcion) {
    case 1:
        printf("Iniciar simulacion\n");
        break;
    case 2:
        printf("Ver resultados\n");
        break;
    case 3:
        printf("Salir\n");
        break;
    default:
        printf("Opcion invalida\n");
}
```

`break` evita continuar al siguiente `case`.

## 3. Bucle `for`

Útil cuando conocés el recorrido o la cantidad de iteraciones.

```c
for (int segundo = 0; segundo <= 120; segundo += 10) {
    double velocidad = 20.0 * segundo;
    printf("Tiempo: %d s | Velocidad: %.2f m/s\n", segundo, velocidad);
}
```

Estructura: `for (inicialización; condición; actualización)`. Primero inicializa, luego comprueba la condición, ejecuta el bloque y actualiza.

## 4. Bucle `while`

Repite **mientras** la condición sea verdadera; puede no ejecutarse nunca.

```c
int segundo = 0;

while (segundo <= 120) {
    printf("Tiempo: %d s\n", segundo);
    segundo += 10; // Sin actualizarlo, podríamos crear un bucle infinito.
}
```

## 5. Bucle `do while`

Ejecuta el bloque **al menos una vez** y después comprueba la condición.

```c
int opcion = 0;

do {
    printf("1. Simular | 2. Salir\n");
    printf("Opcion: ");

    if (scanf("%d", &opcion) != 1) {
        printf("Entrada no numerica\n");
        return 1;
    }
} while (opcion != 2);
```

> Este ejemplo solo ilustra el bucle. Para el trabajo final habrá que validar y recuperar entradas incorrectas sin cerrar el programa; más adelante usaremos `fgets` y conversión controlada.

## 6. `break` y `continue`

- `break`: sale del bucle actual.
- `continue`: salta al siguiente ciclo (en `for`, pasa por la actualización antes de comprobar de nuevo la condición).

```c
for (int i = 0; i < 5; i++) {
    if (i == 2) continue; // Omite el 2.
    if (i == 4) break;    // Termina el bucle.
    printf("%d\n", i);  // Imprime 0, 1 y 3.
}
```

## 7. Diferencias rápidas

| Estructura | Cuándo usarla |
|---|---|
| `if / else` | Elegir entre condiciones |
| `switch` | Seleccionar una opción entre valores concretos |
| `for` | Recorrer una secuencia o contar iteraciones |
| `while` | Repetir mientras se cumpla una condición |
| `do while` | Ejecutar al menos una vez (por ejemplo, mostrar un menú) |

## 8. Compilar y ejecutar (PowerShell)

```powershell
gcc -Wall -Wextra main.c -o programa.exe
.\programa.exe
```

**Recordatorios:** terminá las instrucciones con `;`; encerrá los bloques con `{}`; usá `%d` para `int` y `%f` (por ejemplo `%.2f`) para mostrar `double` en `printf`; no uses `&` al imprimir valores normales.

---

## Funciones y punteros en C

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
