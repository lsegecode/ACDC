#include <stdio.h>

#include "artemis.h"
#include "validaciones.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main(void)
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);   /* para ver bien tildes y símbolos */
#endif

    bool salir = false;

    while (!salir) {
        printf("\n=== MISIÓN ARTEMIS ===\n");
        printf("1) Programa completo (módulos 1 a 5)\n");
        printf("2) Usar un módulo por separado (cargando los datos a mano)\n");
        printf("0) Salir\n");

        switch (leer_opcion("Elegí una opción", 0, 2)) {
        case 1:  programa_completo(); break;
        case 2:  menu_modulos();      break;
        default: salir = true;        break;
        }
    }

    printf("¡Hasta luego!\n");
    return 0;
}
