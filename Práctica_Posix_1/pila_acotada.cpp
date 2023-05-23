#include "pila_acotada.h"

//Implementación del constructor sin parámetros
PilaAcotada::PilaAcotada() {
    //Llamar al constructor con un parámetro usando 10 como parámetro
    PilaAniadir(10);
//Fin del constructor sin parámetros
}

 PilaAcotada::PilaAcotada(int tam_max){
//Implementación del constructor con un parámetro para indicar el tamaño máximo de la pila
     this->m = new mutex_t;
    //Inicializar el tamaño máximo de la pila con el parámetro de entrada del constructor
    this->tam_max = tam_max;
    //Inicializar el mutex de la clase
    this->m->Inicializar();
    //Inicializar las dos variables de condición de la clase

    this->llena = new variable_condicion_t;
    this->vacia = new variable_condicion_t;

    this->llena->Inicializar();
    this->vacia->Inicializar();
    //Asociar el mutex de la clase a las dos variables de condición de la clase. Para ello, hay que pasar el manejador del mutex al método AsociarManejadorMutex de las variables de condición
    this->llena->AsociarManejadorMutex(this->m->ObtenerManejador());
    this->vacia->AsociarManejadorMutex(this->m->ObtenerManejador());
//fin del constructor con un parámetro
}

void PilaAcotada::PilaAniadir(int dato) {
//Implementación del método PilaAniadir con un parámetro de entrada de tipo entero

    //Acceder a la región crítica a través del mutex
    this->m->Lock();
    //Mientras la pila esté llena, bloquearse en la variable de condición de la pila llena
    while(this->pila.size()==this->tam_max){
        this->llena->Wait();
    }
    //Añadir elem en la cima de la pila
    this->pila.push_back(dato);
    //Liberar el mutex
    this->m->Unlock();
    //Liberar, al menos, un hilo de la variable de condición de la pila vacia
    this->vacia->Signal();
//Fin del método PilaAniadir
}


int PilaAcotada::PilaSacar() {
//Implementación del método PilaSacar sin parámetros y que retorna un entero

    //Acceder a la región crítica a través del mutex
    this->m->Lock();
    //Mientras la pila esté vacía, bloquearse en la variable de condición de la pila vacia
    while(this->pila.size() == 0){
          this->vacia->Wait();
    }
    //Definir una variable entera y guardar ahí el valor contenido en la cima de la pila
    int dato_aux = this->pila[this->pila.size()];
    //Eliminar el elemento contenido en la cima de la pila
    this->pila.pop_back();
    //Liberar el mutex
    this->m->Unlock();
    //Hacer un signal en la variable de condición de la pila llena
    this->llena->Signal();
    //Devolver el elemento leído
    return dato_aux;
//Fin del método PilaSacar
}
