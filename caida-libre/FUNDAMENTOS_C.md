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
