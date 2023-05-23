#include "barrera.h"
#include <stdio.h>
#include <stdlib.h>

void imprimir_bloqueo(int hilo){
	printf("El hilo %d se ha bloqueado en la barrera\n", hilo);
}

void imprimir_desbloqueo(int hilo){
	printf("El hilo %d ha abandonado la barrera\n", hilo);
}

/*
Definir un tipo de datos DatosHilos_t que sea una estructura con dos campos (para ello, usar el formato "typedef struct { campos de la estructura } NombreDelTipo;":
- Objeto de tipo puntero a hilo_t: este campo almacenará el manejador del hilo
- Objeto de tipo puntero a Barrera: este campo servirá para que todos los hilos compartan la misma barrera (de ahí que sea un puntero)
*/
typedef struct {
    hilo_t *h;
    Barrera *barrier;
} DatosHilo_t;

//Definir la función HiloEnBarrera para la creación de hilos con un parámetro de entrada siguiendo el formato específico de este tipo de funciones (ver apuntes del tema 2 o el anexo de la práctica)
void *HiloEnBarrera(void *aux) {


	//Definir una variable de tipo DatosHilos_t
    DatosHilo_t datos;
	//Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo de la práctica)
    datos = *((DatosHilo_t *) aux);
	//Mostrar un mensaje indicando que el hilo se bloquea en la barrera
    imprimir_bloqueo(datos.h->ObtenerIdentificador());
	//Bloquearse en la barrera
	datos.barrier->Sincronizar();
	//Mostrar un mensaje indicando que el hilo sale de la barrera
    imprimir_desbloqueo(datos.h->ObtenerIdentificador());
//Fin de la función HiloEnBarrera

}
int main (int argc, char *argv[]) {
	if (argc!=2) {
		printf("Error en la ejecución. El formato correcto es %s num_hilos\n", argv[0]);
		return 0;
	}
	int num_hilos=atoi(argv[1]);
	if (num_hilos<1) {
		printf("Error en la ejecución. El número mínimo de hilos debe ser 1\n");
		return 0;
	}
	//Crear un array de manejadores de hilos de tipo hilo_t de tamaño num_hilos (no usar la clase vector de C++)
    hilo_t vect_hiloMane [num_hilos];
    //Crear una barrera inicializándola con num_hilos
	Barrera barrera(num_hilos);
	//Crear un array de tipo DatosHilos_t de tamaño num_hilos para almacenar los datos de los hilos (no usar la clase vector de C++)
    DatosHilo_t datosHilo [num_hilos];

	//Para cada hilo
	for(int i = 0;i<num_hilos;i++){

        /*Asignar la dirección de memoria del manejador i-ésimo de los manejadores de hilo (usar & delante del manejador para acceder a su dirección de memoria)
        en el campo puntero a hilo_t de la posición i-ésima del array de datos*/
        datosHilo[i].h = &vect_hiloMane[i];
        /*Asignar la dirección de memoria de la barrera (mismo valor para todos los hilos; usar & delante de la barrera para acceder a su dirección de memoria)
        en el campo puntero a barrera de la posición i-ésima del array de datos */
        datosHilo[i].barrier = &barrera;
        //Asignar la función HiloEnBarrera y el dato i-ésimo del array de datos al manejador de hilo i-ésimo
        datosHilo[i].h->AsignarFuncion(HiloEnBarrera);
        datosHilo[i].h->AsignarDato(&datosHilo[i]);
        //Lanzar el hilo i-ésimo
        datosHilo[i].h->Lanzar();

    }

    for(int i = 0;i<num_hilos;i++) {
        //Esperar por todos los hilos
        datosHilo[i].h->Join();
    }
}
