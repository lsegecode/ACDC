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
