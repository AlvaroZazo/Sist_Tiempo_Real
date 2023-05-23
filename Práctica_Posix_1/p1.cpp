#include "pila_acotada.h"
#include <stdio.h>

#define REPETICIONES 50
#define NUM_HILOS 5

void imprimir_lectura(int hilo, int elem){
	printf("El hilo %d ha leído el valor %d de la pila\n", hilo, elem);
}

void imprimir_escritura(int hilo, int elem){
	printf("El hilo %d ha añadido el valor %d a la pila\n", hilo, elem);
}

/*
Definir un tipo de datos DatosHilos_t que sea una estructura con dos campos:
- Identificador de hilo de puntero a hilo_t: este campo almacenará la dirección de memoria de la clase que almacena los datos del hilo para poder usarlos dentro de la función
- Pila acotada de tipo puntero a PilaAcotada: este campo servirá para que todos los hilos compartan la misma pila acotada (de ahí que sea un puntero)
Para definir un tipo se usa la palabra reservada typedef, mientras que para definir una estructura se usa la palabra reservada struct. Por tanto, para definir un
tipo estructura se deben incluir ambas palabras reservadas de la forma "typedef struct { campos de la estructura } NombreDelTipo;"
*/
typedef struct {
    hilo_t *h;
    PilaAcotada *Pila;

} DatosHilo_t;



//Definir la función hilo_productor para la creación de hilos productores siguiendo el formato específico de este tipo de funciones (ver apuntes del tema 2 o el anexo de la práctica 1)
void *hilo_productor(void *aux) {
    //Definir una variable de tipo DatosHilos_t
    DatosHilo_t datos;
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo de la práctica 1)
    datos = *((DatosHilo_t *) aux);
    /*
    Llamar REPETICIONES veces al método PilaAniadir del campo relativo a la pila acotada mostrando un mensaje con el número que se introducirá y el identificador del hilo
    Como en la estructura DatosHilos_t los campos son punteros, para usar sus métodos usaremos '->' en lugar de '.'
    */
    for(int i = 0;i<REPETICIONES;i++) {
        datos.Pila->PilaAniadir(datos.h->ObtenerIdentificador());
        imprimir_escritura(datos.h->ObtenerIdentificador(),datos.h->ObtenerIdentificador());
    }
//Fin hilo_productor
}

//Definir la función hilo_consumidor para la creación de hilos consumidores siguiendo el formato específico de este tipo de funciones (ver apuntes del tema 2 o el anexo de la práctica 1)
void *hilo_consumidor(void *aux) {
    //Definir una variable de tipo DatosHilos_t
    DatosHilo_t datos;
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo de la práctica 1)
    datos = *((DatosHilo_t *) aux);
    /*
    Llamar REPETICIONES veces al método PilaSacar del campo relativo a la pila acotada mostrando un mensaje con el número leído y el identificador del hilo
    Como en la estructura DatosHilos_t los campos son punteros, para usar sus métodos usaremos '->' en lugar de '.'
    */
    int datosacar;
    for (int i = 0; i < REPETICIONES; i++) {
        datosacar = datos.Pila->PilaSacar();
        imprimir_lectura(datos.h->ObtenerIdentificador(),datosacar);
    }
//Fin hilo_consumidor
}

int main (void) {
	//Definir una variable PilaAcotada inicializándola a 20
    PilaAcotada Pila(20);
	//Definir dos vectores de tipo DatosHilos_t y de tamaño NUM_HILOS para almacenar los datos de los productores y de los consumidores
    DatosHilo_t vect_datoProd [NUM_HILOS];
    DatosHilo_t vect_datoCons [NUM_HILOS];
	//Crear dos vectores de tipo hilo_t y de tamaño NUM_HILOS para almacenar los objetos para almacenar los manejadores de hilos de los productores y de los consumidores
    hilo_t vect_hiloManeProd [NUM_HILOS];
    hilo_t vect_hiloManeCons [NUM_HILOS];

    for(int i=0; i < NUM_HILOS;i++ ) {
        //Lanzar los NUM_HILOS productores y consumidores. Para lanzar cada hilo, hay que hacer los siguientes pasos:
        //	1. Asignar al campo Hilo del productor/consumidor i-ésimo la dirección de memoria del valor i-ésimo del vector de manejadores de productores/consumidores según corresponda
        vect_datoProd[i].h = &vect_hiloManeProd[i];
        vect_datoCons[i].h = &vect_hiloManeProd[i];
        //	2. Asignar al campo relativo a la pila acotada del productor/consumidor i-ésimo la dirección de memoria de la variable de tipo PilaAcotada definida al comienzo de la
        //	función (poner & delante de la variable para acceder a su dirección de memoria)
        vect_datoProd[i].Pila = &Pila;
        vect_datoCons[i].Pila = &Pila;
        //	3. Asignar al manejador de hilo del productor/consumidor i-ésimo la función hilo_productor/hilo_consumidor y la dirección de memoria del dato i-ésimo del
        //	productor/consumidor
        vect_hiloManeProd[i].AsignarFuncion(hilo_productor);
        vect_hiloManeProd[i].AsignarDato(&vect_datoProd[i]);
        vect_hiloManeCons[i].AsignarFuncion(hilo_consumidor);
        vect_hiloManeCons[i].AsignarDato(&vect_datoCons[i]);
        //	4. Lanzar el hilo productor/consumidor -iésimo
        vect_hiloManeProd[i].Lanzar();
        vect_hiloManeCons[i].Lanzar();
    }
	//Esperar a que terminen todos los productores y los consumidores
    //TODO BUCLE JOIN PARA PRODUCTOR Y CONSUMIDOR

    for(int i=0; i < NUM_HILOS;i++ ) {
        vect_hiloManeProd[i].Join();
        vect_hiloManeCons[i].Join();
    }
	return 0;
}
