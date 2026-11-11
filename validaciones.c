#include "validaciones.h"

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Auxiliares internas ---------- */

static void reemplazar_punto_por_coma(char *s)
{
    for (; *s; s++)
        if (*s == '.') *s = ',';
}

/* Saca espacios al inicio y al final, y pasa a minusculas. */
static void limpiar(char *s)
{
    size_t n = strlen(s);
    size_t ini = 0;

    while (n > 0 && isspace((unsigned char)s[n - 1])) s[--n] = '\0';
    while (s[ini] && isspace((unsigned char)s[ini])) ini++;
    if (ini > 0) memmove(s, s + ini, strlen(s + ini) + 1);

    for (char *p = s; *p; p++) *p = (char)tolower((unsigned char)*p);
}

/* Lee una linea de stdin. Si se cierra la entrada (EOF), termina el programa. */
static void leer_linea(char *buf, size_t tam)
{
    if (!fgets(buf, (int)tam, stdin)) {
        printf("\n");
        exit(0);
    }
    /* si la linea era mas larga que el buffer, descartar el resto */
    if (!strchr(buf, '\n')) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}

/* Numero para mostrar el valor por defecto: sin ceros de mas. */
static const char *fmt_corto(double valor)
{
    static char bufs[4][64];
    static int i = 0;
    char *b = bufs[i];
    i = (i + 1) % 4;
    snprintf(b, 64, "%.10g", valor);
    reemplazar_punto_por_coma(b);
    return b;
}

/* ---------- Funciones publicas ---------- */

const char *fmt(double valor, int decimales)
{
    static char bufs[8][64];
    static int i = 0;
    char *b = bufs[i];
    i = (i + 1) % 8;
    snprintf(b, 64, "%.*f", decimales, valor);
    reemplazar_punto_por_coma(b);
    return b;
}

int leer_opcion(const char *mensaje, int min, int max)
{
    char linea[64];

    for (;;) {
        printf("%s (%d-%d): ", mensaje, min, max);
        leer_linea(linea, sizeof linea);

        char *fin;
        long op = strtol(linea, &fin, 10);
        while (isspace((unsigned char)*fin)) fin++;

        if (fin != linea && *fin == '\0' && op >= min && op <= max)
            return (int)op;

        printf("  Opción inválida, probá de nuevo.\n");
    }
}

double leer_numero(const char *nombre, const char *unidad,
                   double defecto, bool acepta_km)
{
    char linea[128];

    for (;;) {
        printf("%s [%s] (por defecto %s): ", nombre, unidad, fmt_corto(defecto));
        leer_linea(linea, sizeof linea);
        limpiar(linea);

        if (linea[0] == '\0')
            return defecto;               /* Enter -> valor por defecto */

        double factor = 1.0;
        size_t n = strlen(linea);
        if (acepta_km && n >= 2 && linea[n - 2] == 'k' && linea[n - 1] == 'm') {
            factor = 1000.0;              /* km -> m */
            linea[n - 2] = '\0';
            limpiar(linea);
        }

        /* coma -> punto, y solo se aceptan caracteres numericos */
        bool caracteres_ok = (linea[0] != '\0');
        for (char *p = linea; *p; p++) {
            if (*p == ',') *p = '.';
            if (!strchr("0123456789.+-e", *p)) caracteres_ok = false;
        }

        if (caracteres_ok) {
            char *fin;
            double v = strtod(linea, &fin);
            if (*fin == '\0' && isfinite(v) && v > 0.0)
                return v * factor;
        }

        printf("  Valor inválido: ingresá un número mayor que cero.\n");
    }
}
