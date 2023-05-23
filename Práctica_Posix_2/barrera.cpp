#include "barrera.h"

//Implementación del constructor sin parámetros
Barrera::Barrera() {
    //Inicializar el tamaño máximo de la barrera a 10 (llamar al constructor con un parámetro usando 10 como parámetro)
    Barrera(10);
//Fin del constructor sin parámetros
}


//Implementación del constructor con un parámetro para indicar el tamaño máximo de la barrera
Barrera::Barrera(int num_hilo) {

    //Inicializar el tamaño máximo de la barrera al valor introducido por parámetro
    this->B_num_hilo = num_hilo;
    //Inicializar el número de hilos actuales en la barrera a 0
    this->hilos_en_B = 0;
    //Inicializar el mutex de la clase
    this->B_mutex.Inicializar();
    //Inicializar la variable de condición de la clase asociándole el manejador del mutex de la clases
    this->B_vcond.Inicializar();
    this->B_vcond.AsociarManejadorMutex(this->B_mutex.ObtenerManejador());
//Fin del constructor con un parámetro
}

/*Implementación del método Sincronizar. Este método bloqueará el hilo invocante en la barrera si no se ha alcanzado el número indicado
o liberará a todos los hilos si es el último hilo en llegar. Reseteará también la barrera, dejándola lista para  volver a utilizarla
*/
void Barrera::Sincronizar() {
    //Entrar en el mutex
    this->B_mutex.Lock();
    //Incrementar el número de hilos en la barrera
    this->hilos_en_B++;
    //Si han llegado todos los hilos esperados
    if(this->B_num_hilo == this->hilos_en_B) {
        //Liberar a todos los hilos de la variable de condición
        this->B_vcond.Broadcast();

        //Resetear la barrera
        this->Resetear();
        //Si no
    }else{
        this->B_vcond.Wait();
    }
    //Bloquearse en la variable de condición

    //Liberar el mutex
    this->B_mutex.Unlock();
//Fin del método Sincronizar
}

void Barrera::Resetear() {
/*Implementación del método Resetear sin parámetros. Este método pondrá a cero el número de hilos que han llegado a la barrera.
Si no han llegado todos los hilos esperados a la barrera, no se realizará el reseteo de la misma (esto previene que se resetee la barrera antes de que lleguen todos los hilos)*/

    //Si han llegado a la barrera todos los hilos esperados
    if(this->hilos_en_B == this->B_num_hilo) {
        //Poner a cero el contador de los hilos
        this->hilos_en_B = 0;
    }
//Fin del método Resetear sin parámetros
}

void Barrera::Resetear(int num_hilo) {
/*Implementación del método Resetear con un parámetro. Este método pondrá a cero el número de hilos que han llegado a la barrera.
Si no han llegado todos los hilos esperados a la barrera, no se realizará el reseteo de la misma (esto previene que se resetee la barrera antes de que lleguen todos los hilos).
por último, cambiará el número de hilos por los que se esperará en la barrera*/

    //Si han llegado a la barrera todos los hilos esperados
    if(this->hilos_en_B == this->B_num_hilo) {
        //Poner a cero el contador de los hilos
        this->hilos_en_B = 0;
        //Cambiar el número de hilos por los que se esperará usando el parámetro de entrada del método
        this->B_num_hilo = num_hilo;
    }
//Fin del método Resetear sin parámetros
}