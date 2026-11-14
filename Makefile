# Makefile - Simulador de Misiones Artemis
# make        -> compila y genera el ejecutable "simulador"
# make run    -> compila y ejecuta
# make clean  -> borra los archivos generados
# (en Windows con MinGW el comando es mingw32-make)

simulador: main.o artemis.o validaciones.o
	gcc main.o artemis.o validaciones.o -lm -o simulador

main.o: main.c artemis.h validaciones.h
	gcc -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 -c main.c

artemis.o: artemis.c artemis.h validaciones.h
	gcc -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 -c artemis.c

validaciones.o: validaciones.c validaciones.h
	gcc -Wall -Wextra -finput-charset=UTF-8 -fexec-charset=UTF-8 -c validaciones.c

run: simulador
	./simulador

clean:
	rm -f main.o artemis.o validaciones.o simulador simulador.exe
