Revisá el proyecto **Simulador de Misiones Artemis**, desarrollado en lenguaje C.

Quiero mejorar la legibilidad del código reemplazando los nombres de variables abreviados o poco descriptivos por nombres claros en español, utilizando la convención `snake_case`.

Por ejemplo:

- `a1` → `aceleracion_fase1`
- `t1` → `tiempo_fase1`
- `a2` → `aceleracion_fase2`
- `h` → `altura_apagado`
- `g` → `gravedad`
- `v1` → `velocidad_desacople`
- `y1` → `altura_desacople`
- `v2` → `velocidad_apagado`
- `hmax` → `altura_maxima`
- `d` → `datos`
- `r` → `resultados`

**Restricciones:**

1. No modificar la lógica de programación, las fórmulas físicas ni los resultados de la simulación.
2. No agregar ni eliminar funcionalidades.
3. No modificar los nombres de las funciones ni de las estructuras.
4. Actualizar todas las referencias a las variables renombradas para mantener la compatibilidad entre archivos `.c` y `.h`.
5. Mantener la organización y modularización existente.
6. Verificar que el proyecto compile correctamente después de los cambios.

## Al finalizar, presentar un resumen de los nombres modificados y confirmar si se conservaron el funcionamiento y los resultados originales.


---

## Reordenar el menú a 7 opciones con la integración del módulo por separado

Prompt utilizado (asistente: Claude):

> Reordenar el menú a 7 opciones con la integración del módulo por separado.
>
> 1. Ingresar los datos
> 2. Ejecutar con los datos oficiales
> 3. Ver resultados detallados de la última misión (solo este punto cambia, el resto queda igual)
> 4. Ver historial de simulaciones
> 5. Ver gráfico de trayectoria
> 6. Ejecutar un módulo por separado (con la posibilidad de que ejecute con los datos oficiales si ingresa Enter)
> 7. Salir del sistema
>

Resultado: se modificó `main.c` (menú de 7 opciones y función `ejecutar_modulo_individual`, que reutiliza las funciones de `validaciones.c` y `artemis.c`) y se movió `Flujo_de_trabajo.png` a `Documentacion/`.
