#include <stdlib.h>
#include <stdio.h>
#include <ClasesPosix.h>
//Añadir la librería de utilización de señales
#include <signal.h>

void senial_capturada(int capturadas, int max_capturas) {
    printf("Se han capturado %d señales SIGINT de %d.\n", capturadas, max_capturas);
}

/*Definir la función HiloSigInt para la creación de un hilo que capture la señal SIGINT siguiendo el formato específico de este tipo de funciones (ver apuntes del tema 2
o el anexo de la práctica). Tendrá un parámetro de entrada que indicará el número de señales a capturar*/
void *HiloSigInt(void *aux) {

    //Definir una variable de tipo entero para almacenar el número de señales a capturar
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo de la práctica)
    int *num_sig = (int *)aux;
    //Definir una variable para almacenar el número de señales capturadas
    int sig_capture = 0;
    //Crear un conjunto de señales
    sigset_t set;
    //Inicializar el conjunto de señales a vacío
    sigemptyset(&set);
    //Añadir SIGINT al conjunto de señales
    sigaddset(&set,SIGINT);
    //Añadir el conjunto de señales a la máscara de señales bloqueadas
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    //Mientras no se hayan capturado las señales esperadas
    while (*num_sig != sig_capture) {
        //Esperar por el conjunto de señales, que solamente contiene SIGINT (habrá que crear una variable para usarla como segundo parámetro de la función sigwait)
        int sig;
        sigwait(&set, &sig);
        //Incrementar el número de señales capturadas
        sig_capture++;
        //Mostrar un mensaje indicando cuántas señales se han capturado y cuántas se espera capturar
        senial_capturada(sig_capture,*num_sig);
    }
//Fin de la función HiloSigInt
}

int main (int argc, char *argv[]) {
    if (argc!=2) {
        printf("Error en la ejecución. El formato correcto es %s num_capturas\n", argv[0]);
        return 0;
    }
    int num_capturas=atoi(argv[1]);
    if (num_capturas<1) {
        printf("Error en la ejecución. El número mínimo señales a capturar debe ser 1\n");
        return 0;
    }
    //Crear un conjunto de señales
    sigset_t set;
    //Inicializar el conjunto de señales a vacío
    sigemptyset(&set);
    //Añadir SIGINT al conjunto de señales
    sigaddset(&set,SIGINT);
    //Añadir el conjunto de señales a la máscara de señales bloqueadas
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    //Crear un manejador de hilo usando la clase hilo_t
    hilo_t t;
    //Asignar la función HiloSigInt y la dirección de memoria del número de capturas por las que se desea esperar (num_capturas) al manejador definido en el punto anterior
    t.AsignarFuncion(HiloSigInt);
    t.AsignarDato(&num_capturas);
    //Lanzar el hilo
    t.Lanzar();
    //Esperar por el hilo
    t.Join();
}