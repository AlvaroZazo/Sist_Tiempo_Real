//Ejecución: para poder modificar los atributos del hilo, debemos ejecutar como superusuario: sudo ./p5 fichero_tareas

#include <stdio.h>
#include <stdlib.h>
#include "funciones_time.h"
#include <ClasesPosix.h>
//Incluir librería de tiempos
#include <time.h>


//La función Ejecutar mantiene la ejecución durante n milisegundos
void Ejecutar(int n) {
    int i;
    struct timespec ini, fin, dif;
    int finb = 0;
    double y = n / 1000.0;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &ini);
    while (!finb) {
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &fin);
        dif = RestaTimeSpec(fin, ini);
        if (timespec2segundos(dif) >= y)
            finb = 1;
    }
}

void ImprimirInicioRecurso(int id, int recurso, int tiempoExec) {
    printf("\t\t\tHilo %d comienza la ejecución del recurso %d que requiere %d ms.\n", id, recurso, tiempoExec);
}

void ImprimirFinRecurso(int id, int recurso) {
    printf("\t\t\tHilo %d termina con el recurso %d.\n", id, recurso);
}

void ImprimirInicio(int id, int prioridad) {
    printf("Hilo %d con prioridad %d comienza su acción periódica\n", id, prioridad);
}

void ImprimirFinal(int id) {
    printf("Hilo %d termina su acción periódica\n", id);
}

void ImprimirFranjaMS(int x) {
    printf("************************* %d ms *************************\n", x);
}

/*Definir la función EjecutarRecurso con los siguientes parámetros:
	Mutex asociado al recurso que se va a ejecutar de tipo puntero a mutex_t
	Recurso que se va a ejecutar (entero). Los valores que puede tomar este parámetro van de -1 (indica que se trata de un recurso no compartido) a N-1 (valores >=0 indican
		que se trata de un recurso compartido), siendo N el número de recursos existentes en la aplicación
	Tiempo de ejecución del recurso que se va a ejecutar (entero)
	Identificador de la tarea que realiza la ejecución (entero)*/

void EjecutarRecurso(mutex_t *mutex_recurso, int recurso, int tiempo_ejec_recurso, int id_tarea) {

//Si el recurso a ejecutar es un recurso compartido
    if (recurso >= 0) {
//Bloquear el mutex del recurso (mutex recibido por parámetro)
        mutex_recurso->Lock();
//Fin si
    }
/*Imprimir el mensaje "Hilo id comienza la ejecución del recurso x que requiere y ms.", donde "id" es el identificador de la tarea que ejecuta el recurso, "x" es el
identificador del recurso e "y" es el tiempo de ejecución del recurso (usar la función ImprimirInicioRecurso)*/
    ImprimirInicioRecurso(id_tarea, recurso, tiempo_ejec_recurso);
//Ejecutar la función Ejecutar usando como parámetro el tiempo de ejecución recibido por parámetro
    Ejecutar(tiempo_ejec_recurso);
/*Mostrar el mensaje "Hilo id termina con el recurso x.", donde "id" es el identificador de la tarea que ejecuta el recurso y "x" es el identificador del recurso (usar la
funcióin ImprimirFinRecurso)*/
    ImprimirFinRecurso(id_tarea, recurso);
//Si el recurso a ejecutar es un recurso compartido
    if (recurso >= 0) {
//Desbloquear el mutex del recurso (mutex recibido por parámetro)
        mutex_recurso->Unlock();
//Fin si
    }
//Fin EjecutarRecurso
}


//Definir la función TareaPeriodica que reciba un parámetro de tipo puntero a hilo_t (recordemos que el tipo de la función debe ser puntero a void y luego convertimos)
void *TareaPeriodica(void *aux) {
    //Definir una variable de tipo puntero a hilo_t
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el Anexo 1 de la práctica)

    hilo_t *p = (hilo_t *) aux;

    //Definir tres variables de tipo struct timespec para almacenar el instante de comienzo, el instante siguiente y el periodo de repetición
    struct timespec t_comienzo, t_siguiente, t_period_rep;
    //Obtener el periodo de repetición del hilo actual mediante el método ObtenerPeriodo de la clase hilo_t
    t_period_rep = p->ObtenerPeriodo();
    //Obtener el instante de comienzo del hilo actual mediante el método ObtenerInstanteDeComienzo de la clase hilo_t
    t_comienzo = p->ObtenerInstanteDeComienzo();
    //Calcular el siguiente instante sumando el instante de comienzo y el periodo
    t_siguiente = SumaTimeSpec(t_comienzo, t_period_rep);
    //Dormir el hilo hasta el instante de comienzo
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t_comienzo, NULL);
    //Definir un bucle infinito
    while (true) {
        /*Imprimir el mensaje "Hilo id con prioridad P comienza su acción periódica", siendo "id" y "P" el identificador y la prioridad del hilo respectivamente (usar la función
        ImprimirInicio y los métodos ObtenerIdentificador y ObtenerPrioridad de la clase hilo_t para obtener los valores a mostrar)*/
        ImprimirInicio(p->ObtenerIdentificador(), p->ObtenerPrioridad());
        //Para cada acción del hilo (usar el método ObtenerNumAcciones para obtener el número de acciones del hilo)
        for (int i = 0; i < p->ObtenerNumAcciones(); i++) {
            /*Invocar a la función EjecutarRecurso con el mutex asociado al recurso que se va a ejecutar (método ObtenerMutex de la clase hilo_t), el valor del recurso i-ésimo
            del hilo (método ObtenerRecurso de la clase hilo_t), la acción que se va a ejecutar (método ObtenerAccion de la clase hilo_t) y el identificador del hilo (método
            ObtenerIdentificador de la clase hilo_t)*/
            EjecutarRecurso(p->ObtenerMutex(p->ObtenerRecurso(i)), p->ObtenerRecurso(i), p->ObtenerAccion(i),
                            p->ObtenerIdentificador());
            //Fin para
        }
        //Imprimir el mensaje "Hilo id termina su acción periódica", siendo "id" el identificador del hilo (usar la función ImprimirFinal y el método ObtenerIdentificador de la clase hilo_t)
        ImprimirFinal(p->ObtenerIdentificador());
        //Dormir el hilo hasta el siguiente instante de repetición
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t_siguiente, NULL);
        //Incrementar el siguiente instante de repetición con el periodo
        t_siguiente = SumaTimeSpec(t_siguiente, t_period_rep);
        //Fin bucle
    }
//Fin tareaPeriodica
}

/*Definir la función TareaMostrarTiempos que reciba un parámetro de tipo puntero a hilo_t (recordemos que el tipo de la función debe ser puntero a void y luego convertimos). Esta
función es igual a la definida en la Práctica Posix 4, por lo que puede copiarse directamente de esa práctica*/
void *TareaMostrarTiempos(void *aux) {
    //Definir una variable de tipo puntero a hilo_t
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el Anexo 1 de la práctica)
    hilo_t *p = (hilo_t *) aux;
    //Definir tres variables de tipo struct timespec para almacenar el instante de comienzo, el instante siguiente y el periodo de repetición
    struct timespec t_comienzo, t_siguiente, t_period_rep;
    //Obtener el periodo de repetición del hilo actual mediante el método ObtenerPeriodo de la clase hilo_t
    t_period_rep = p->ObtenerPeriodo();
    //Obtener el instante de comienzo del hilo actual mediante el método ObtenerInstanteDeComienzo de la clase hilo_t
    t_comienzo = p->ObtenerInstanteDeComienzo();
    //Calcular el siguiente instante sumando el instante de comienzo y el periodo
    t_siguiente = SumaTimeSpec(t_comienzo, t_period_rep);
    //Dormir el hilo hasta el instante de comienzo
    clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t_comienzo, NULL);
    //Para i desde 0 hasta 100
    for (int i = 0; i <= 100; i++) {
        //Imprimir "************************* x ms *************************\n" (usar ImprimirFranjaMS), siendo x=i*10 los milisegundos actuales
        ImprimirFranjaMS(i * 10);
        //Dormir el hilo hasta sl siguiente instante de repetición
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t_siguiente, NULL);
        //Incrementar el siguiente instante de repetición con el periodo
        t_siguiente = SumaTimeSpec(t_siguiente, t_period_rep);
        //Fin Para
    }
//Fin TareaMostrarTiempos
}

int main(int argc, char *argv[]) {
    int num_tareas, i, j, num_recursos, C, T, Ta, P, num_acc, accion;
    FILE *f;
    vector<int> Acciones;
    vector<int> Recursos;
    if (argc != 2) {
        printf("El formato de ejecución correcto es %s vfichero_tareas\n", argv[0]);
        return 0;
    }
    f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("Error al abrir el fichero %s\n", argv[1]);
        return 0;
    }
    fscanf(f, "%d\n", &num_tareas);
    fscanf(f, "%d", &num_recursos);
    int techos_prioridad[num_recursos];
    for (i = 0; i < num_recursos; i++) {
        fscanf(f, " %d", &techos_prioridad[i]);
        printf("Techo recurso %d: %d\n", i, techos_prioridad[i]);
    }
    /*
    *******************************************************************************************************************
    ****************** IMPORTANTE: LEER PARA COMPRENDER LAS VARIABLES PREDEFINIDAS DE LA APLICACIÓN *******************
    *******************************************************************************************************************
    Variables predefinidas y su significado:
        num_tareas: número de tareas que se van a ejecutar
        num_recursos: número de recursos compartidos existentes
        techos_prioridad: vector de enteros con los techos de prioridad de los recursos compartidos
        Acciones: Vector de enteros que almacenará las acciones que ejecutarán los hilos. Para cada hilo, se limpia el vector y se rellena con sus acciones, siendo, por tanto,
            una variable de almecenamiento temporal (solo se puede usar para inicializar los hilos)
        Recursos: Vector de enteros que almacenará los recursos que usarán los hilos. Para cada hilo, se limpia el vector y se rellena con sus recursos, siendo, por tanto,
            una variable de almecenamiento temporal (solo se puede usar para inicializar los hilos)
    */

/*
*******************************************************************************************************************
********************************** Zona de implementación de la Práctica Posix 5 **********************************
*******************************************************************************************************************
*/
    //Definir un vector de tipo puntero a mutex_t (usar la clase vector)
    vector<mutex_t*> mutex_vec;
    //Para cada recurso (la variable num_recursos indica cuántos recursos hay)

    for (int i = 0; i < num_recursos; i++) {
        //Definir una variable de tupo puntero a mutex_t y reservarle memoria (hay que hacerlo dentro del bucle para que se cree un nuevo mutex para cada recurso)
        mutex_t *p_mutex = new mutex_t();
        //Inicializar el mutex recién definido usando el techo de prioridad inmediato (PTHREAD_PRIO_PROTECT) y el techo de prioridad del recurso i-ésimo
        p_mutex->AsignarProtocoloYTecho(PTHREAD_PRIO_PROTECT, techos_prioridad[i]);
        p_mutex->Inicializar();
        //Añadir el mutex recién creado al vector de mutex (usar el método push_back de la clase vector)
        mutex_vec.push_back(p_mutex);  //No me pilla la clase vector

        //Fin para
    }

    //Definir un vector de tamaño num_tareas de tipo hilo_t (no usar la clase vector). Este vector permitirá manejar los hilos que ejecutarán las tareas periódicas.
    hilo_t hilos_tareas[num_tareas];


    //Definir una variable de tipo timespec para almacenar el instante de comienzo
    timespec t_comienzo;
    //Leer la hora actual y almacenarla en la variable anterior
    clock_gettime(CLOCK_MONOTONIC, &t_comienzo);
    //Sumar 2 segundos al tiempo de comienzo (se puede sumar 2 directamente al campo tv_sec de la variable)
    t_comienzo.tv_sec = t_comienzo.tv_sec + 2;
/*
*******************************************************************************************************************
****************************** Fin de zona de implementación de la Práctica Posix 5 *******************************
*******************************************************************************************************************
*/
    printf("Tarea\tC\tT\tTa\tP\tAcciones\n");
    for (i = 0; i < num_tareas; i++) {
        Acciones.clear();
        Recursos.clear();
        fscanf(f, "%d %d %d %d %d ", &C, &T, &Ta, &P, &num_acc);
        printf("t%d:\t%d\t%d\t%d\t%d\t%d - ", i + 1, C, T, Ta, P, num_acc);
        char c;
        for (j = 0; j < num_acc; j++) {
            fscanf(f, " %c_%d", &c, &accion);
            Acciones.push_back(accion);
            if (c == 'n' || c == 'N')
                Recursos.push_back(-1);
            else
                Recursos.push_back(c - 48);
            printf("R%d %dms ", Recursos[j], Acciones[j]);
        }
        fscanf(f, "\n");
        printf("\n");
/*
*******************************************************************************************************************
********************************** Zona de implementación de la Práctica Posix 5 **********************************
*******************************************************************************************************************
*/
        /*Asignar al hilo i-ésimo la prioridad (P), la política FIFO (SCHED_FIFO), la no herencia de atributos (PTHREAD_EXPLICIT_SCHED), el periodo (T), el tiempo de ejecución (C),
        el instante de comienzo particular de la tarea (se calcula como el instante de comienzo leído previamente más el tiempo de acceso (Ta) de la tarea; para realizar la suma, hay que convertir Ta a timespec),
        la lista de acciones del hilo (Acciones), la lista de recursos del hilo (Recursos) y el vector de mutex. Para ello, usar el nuevo método EstablecerAtributos de la clase hilo_t*/
            hilos_tareas[i].EstablecerAtributos(P,SCHED_FIFO,PTHREAD_EXPLICIT_SCHED, T, C, SumaTimeSpec(t_comienzo , ms2timespec(Ta)), Acciones,
                                                Recursos, mutex_vec);



/*
 *
 *
 *
*******************************************************************************************************************
****************************** Fin de zona de implementación de la Práctica Posix 5 *******************************
*******************************************************************************************************************
*/
    }
    printf("\n");
/*
*******************************************************************************************************************
********************************** Zona de implementación de la Práctica Posix 5 **********************************
*******************************************************************************************************************
*/
    //Para cada tarea (num_tareas indica cuántas tareas se van a ejecutar)
    for (int i = 0; i < num_tareas; i++) {
        //Establecer en el hilo i-ésimo la función y el dato que va a utilizar usando como parámetros la función TareaPeriodica y el propio manejador del hilo i-ésimo
        hilos_tareas[i].AsignarFuncionYDato(TareaPeriodica, &hilos_tareas[i]);
        //Lanzar el hilo i-ésimo (método Lanzar de la clase hilo_t)
        hilos_tareas[i].Lanzar();
        //Fin para
    }
    //Definir una variable de tipo hilo_t para manejar el hilo que mostrará las franjas de tiempo
    hilo_t hilo_f_tiempo;

    /*Asignar al hilo que mostrará las franjas de tiempo la prioridad máxima para la política FIFO (usar la función sched_get_priority_max), la política FIFO (SCHED_FIFO),
    la no herencia de atributos (PTHREAD_EXPLICIT_SCHED), el periodo (10), el tiempo de ejecución (0, no es necesario para este hilo pero hay que darle un valor)
    y el instante de comienzo. Para ello, usar el método EstablecerAtributos de la clase hilo_t definido en la práctica 4*/
    hilo_f_tiempo.EstablecerAtributos(sched_get_priority_max(SCHED_FIFO), SCHED_FIFO, PTHREAD_EXPLICIT_SCHED, 10, 0,
                                      t_comienzo); //
    //Establecer en el hilo que mostrará las franjas de tiempo la función y el dato que va a utilizar usando como parámetros la función tareaMostrarTiempos y el propio manejador del hilo i-ésimo
    hilo_f_tiempo.AsignarFuncionYDato(TareaMostrarTiempos, &hilo_f_tiempo);
    //Lanzar el hilo que mostrará las franjas de tiempo
    hilo_f_tiempo.Lanzar();
    //Esperar únicamente por la finalización del hilo que mostrará las franjas de tiempo
    hilo_f_tiempo.Join();
    //Como los mutex fueron definidos como punteros, debemos liberar la memoria de todos ellos. Para ello, ver las líneas siguientes
    //Para cada mutex en el vector de mutex (el método size de la clase vector indica cuántos elementos hay en el vector)
    for(int i = 0; i< num_recursos; i++) {
        //Liberar la memoria del mutex i-ésimo
        delete mutex_vec[i];
        //Fin para
    }
/*
*******************************************************************************************************************
****************************** Fin de zona de implementación de la Práctica Posix 5 *******************************
*******************************************************************************************************************
*/
    return 0;
}
