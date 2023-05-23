#include <funciones_time.h>
#include <ClasesPosix.h>
#include "Identificador.h"
using namespace std;

/*
**********************************************************
********************** Clase hilo_t **********************
**********************************************************

Los métodos implementados para esta clase en la Práctica Posix 4 hay que mantenerlos sin modificar
*/

Identificador_t Identificador;

//Implementación del constructor sin parámetros. Este método reservará memoria para los atributos de la clase e inicializará los atributos de creación del hilo
hilo_t::hilo_t() {
    //Guardar el resultado de Identificador.ObtenerId() en el atributo de clase que almacena el identificador del hilo.
    this->identificador = Identificador.ObtenerId();
    //Reserva de memoria para el manejador del hilo
    this->manejador = new pthread_t;
    //Reserva de memoria para los atributos de creación del hilo
    this->attributes = new pthread_attr_t;
    //Inicialización de los atributos de creación del hilo
    pthread_attr_init(this->attributes);
    //Inicialización de la función del hilo a NULL
    start_routine = NULL;
    //Inicialización de los datos de la función del hilo a NULL
    datosHilos = NULL;
//fin del constructor
}

//Implementación del destructor sin parámetros. Este método destruirá los atributos de creación del hilo y liberará la memoria de los atributos de la clase
hilo_t::~hilo_t() {
    //Liberación la memoria del manejador del hilo
    delete this->manejador;
    //Destrucción de los atributos de creación del hilo
    pthread_attr_destroy(this->attributes);
    //Liberación de memoria de los atributos de creación del hilo
    delete this->attributes;
//Fin del destructor
}

//Implementación del método Lanzar. Este método se encargará de lanzar un hilo usando la función y los datos almacenados en la clase
int hilo_t::Lanzar() {

    //Si la función asociada existe (no es NULL)
    if (start_routine == NULL) {
        return -1;
    } else {
        /*Crear un hilo usando como parámetros el manejador del hilo, los atributos de creación del hilo, la función del hilo y los datos del hilo.
   Se devolverá el resultado de la llamada a pthread_create*/

        //Si la función asociada no existe

        //Devolver -1
        return pthread_create(manejador, attributes, start_routine, datosHilos);

    }

//Fin de Lanzar
}


//Implementación del método AsignarFuncion. Asignará el parámetro recibido al atributo que almacena la función del hilo
void hilo_t::AsignarFuncion(void *(*start_routine)(void *)) {
    //Asignar el parámetro del método al atributo de la clase que almacena la función asociada
    this->start_routine = start_routine;
//Fin de AsignarFuncion
}


//Implementación del método AsignarDato. Asignará el parámetro recibido al atributo que almacena los datos que se usarán con la función del hilo
void hilo_t::AsignarDato(void *dato) {
    //Asignar el parámetro del método al atributo de la clase que almacena el parámetro que se usará con la función asociada
    this->datosHilos = dato;
//fin de AsignarDato
}

//Implementación del método AsignarFuncionYDato. Este método llamará a los métodos AsignarFuncion y AsignarDato con cada uno de sus parámetros
void hilo_t::AsignarFuncionYDato(void *(*start_routine)(void *), void *attributes) {
    //Llamar a AsignarFuncion
    AsignarFuncion(start_routine);
    // Llamara AsignarDato
    AsignarDato(attributes);
//Fin de AsignarFuncionYDato
}

//Implementación del método Join sin parámetros. Este método esperará por el hilo indicado por el manejador de la clase y retornará el valor devuelto por dicho hilo.
void *hilo_t::Join() {
    //Definir una variable de tipo puntero a void (debe ser puntero a void para permitir que se pueda devolver cualquier tipo de dato). No hay que reservar memoria.
    void *p;
    /*Esperar por el hilo asociado a la clase usando como parámetros el valor del manejador del hilo (como es un puntero, hay que poner el carácter '*' delante del nombre para acceder a su valor)
    y la dirección de memoria de la variable definida en la línea anterior (poner el carácter '&' antes del nombre de la variable para acceder a su dirección de memoria).*/
    pthread_join(*manejador, (void **) &p);
    //Devolver la variable definida en la primera línea del método.
    return p;
    //Fin de Join
}

//Implementación de métod ObtenerManejador sin parámetros. Este método devolverá el valor del manejador del hilo asociado a la clase.
pthread_t hilo_t::ObtenerManejador() {
    //Devolver el valor del manejador del hilo (como es un puntero, hay que poner el carácter '*' delante del nombre para acceder a su valor)
    return *manejador;
//Fin de ObtenerManejador
}

//Implementación de método ObtenerIdentificaror sin parámetros. Este método devolverá el valor del identificador del hilo asociado a la clase.
int hilo_t::ObtenerIdentificador() {
    //Devolver el identificador del hilo
    return this->identificador;
//Fin de ObtenerIdentificaror
}

/*
***************************************************************************************************************
*********************** Implementación de los nuevos métodos para la prática de posix 4 ***********************
***************************************************************************************************************
*/

/*Implementación del método EstablecerAtributos. Este método se encargará de guardar los datos pasados como parámetros a sus atributos correspondientes (hay que transformar el
periodo, que se recibe en milisegundos, a struct timespec, ver funciones_time.h). Además, establecerá la herencia, la política y la prioridad del hilo*/
int hilo_t::EstablecerAtributos(int prioridad, int politica, int herecia, int periodo_rep,
                                        int tiempo_exec_tperiodica, struct timespec tiempo_comienzo_tarea) {
//Guardar los parámetros recibidos en los atributos correspondientes, transformando el periodo de milisegundos a struct timespec (usar la función ms2timespec de funciones_time)
    this->priority = prioridad;
    this->policy = politica;
    this->period_rep = ms2timespec(periodo_rep);
    this->t_exec = tiempo_exec_tperiodica;
    this->crit_int = tiempo_comienzo_tarea;

//Establecer la herencia recibida por parámetros en los atributos de creación del hilo de la clase (función pthread_attr_setinheritsched)
    pthread_attr_setinheritsched(this->attributes, herecia);
//Establecer la política recibida por parámetros en los atributos de creación del hilo de la clase (función pthread_attr_setschedpolicy)
    pthread_attr_setschedpolicy(this->attributes, this->policy);
//Definir una variable de tipo struct sched_param
    struct sched_param param;
//Asignar la prioridad al campo sched_priority de la variable anterior
    param.sched_priority = this->priority;
//Asignar la variable anterior a los atributos de creación del hilo de la clase (función pthread_attr_setschedparam)
    pthread_attr_setschedparam(this->attributes, &param);
//Fin EstablecerAtributos
}

//Implementación de la función ObtenerPeriodo. Esta función lo único que hace es devolver el periodo del hilo
struct timespec hilo_t::ObtenerPeriodo() {
    return this->period_rep;
}

//Implementación de la función ObtenerInstanteDeComienzo. Esta función lo único que hace es devolver el instante de comienzo del hilo
struct timespec hilo_t::ObtenerInstanteDeComienzo() {
    return this->crit_int;
}

//Implementación de la función ObtenerTiempoEjecucion. Esta función lo único que hace es devolver el tiempo de ejecución del hilo
int hilo_t::ObtenerTiempoEjecucion() {
    return this->t_exec;
}

//Implementación de la función ObtenerPrioridad. Esta función lo único que hace es devolver la prioridad del hilo
int hilo_t::ObtenerPrioridad() {
    return this->priority;
}

//Implementación de la función ObtenerPolitica. Esta función lo único que hace es devolver la política del hilo
int hilo_t::ObtenerPolitica() {
    return this->policy;
}
/*
***************************************************************************************************************
************************ Implementación de los nuevos métodos para la Prática Posix 5 *************************
***************************************************************************************************************
*/

/*Implementación del método EstablecerAtributos. Este método se encargará de guardar los datos pasados como parámetros a sus atributos correspondientes y de establecer la herencia,
la política y la prioridad del hilo. Para ello, se puede utilizar el método EstablecerAtributos definido en la Práctica Posix 4. Por otra parte, guardará la lista de acciones,
la lista de recursos y la lista de mutex pasados como últimos parámetros del método*/

int hilo_t::EstablecerAtributos(int prioridad, int politica,int herecia, int periodo_rep,
                            int tiempo_exec_tperiodica, struct timespec tiempo_comienzo_tarea, vector<int> acciones, vector<int> recursos, vector< mutex_t*> mutex_recursos){

/*Llamar al método EstablecerAtributos definido en la Práctica Posix 4 con la prioridad, la política, la herencia, el periodo de repetición, el tiempo de ejecución de la tarea
    periódica y el tiempo de comienzo del hilo*/
    EstablecerAtributos(prioridad,politica,herecia,periodo_rep,tiempo_exec_tperiodica,tiempo_comienzo_tarea);
    /*Guardar el resto de los parámetros recibidos en los atributos correspondientes. La clase vector tiene definido el operador de asignación, por lo que se pueden hacer
    asignaciones de vectores sin ningún problema*/
    this->acciones = acciones;
    this->recursos = recursos;
    this->mutex_recursos = mutex_recursos;

//Fin del método EstablecerAtributos
}



//Implementar el método ObtenerNumAcciones. Esta función lo único que hace es devolver el tamaño del vector de la lista de acciones
int hilo_t::ObtenerNumAcciones() {
    this->acciones.size();
}

//Implementar el método ObtenerAccion. Esta función lo único que hace es devolver el valor de la lista de acciones indicado por el parámetro de entrada

int hilo_t::ObtenerAccion(int accion) {
    return this->acciones[accion];
}
//Implementar el método ObtenerRecurso. Esta función lo único que hace es devolver el valor de la lista de recursos indicado por el parámetro de entrada
int hilo_t::ObtenerRecurso(int recurso) {
    return this->recursos[recurso];
}

/*Implementar el método ObtenerMutex. Esta función lo único que hace es devolver el valor de la lista de mutex indicado por el parámetro de entrada. En caso de que el parámetro
tenga un valor menor que cero o mayor o igual que el número de mutex, devolverá NULL*/
mutex_t *hilo_t::ObtenerMutex(int mutex) {
    if(mutex < 0)
        return NULL;
    else
        return this->mutex_recursos[mutex];
}

/*
**********************************************************
********************** Clase mutex_t *********************
**********************************************************

Los métodos implementados para esta clase en la Práctica Posix 0 hay que mantenerlos sin modificar
*/


mutex_t::mutex_t() {



    //Reserva de memoria para el manejador del mutex de la clase
    this->mutex_hand = new pthread_mutex_t();
    //Reserva de memoria para los atributos de creación del mutex
    this->mutex_attribute = new pthread_mutexattr_t();
    //Inicialización de los atributos de creación del mutex
    pthread_mutexattr_init(this->mutex_attribute);
//fin del constructor
}


mutex_t::~mutex_t() {
//Implementación del destructor sin parámetros. Este método liberará la memoria de los atributos de la clase y destruirá el mutex y los atributos de creación del mutex

    //Destrucción del mutex
    pthread_mutex_destroy(this->mutex_hand);
    //Liberación de memoria para el manejador del mutex de la clase
    delete this->mutex_hand;
    //Destrucción de los atributos de creación del mutex
    pthread_mutexattr_destroy(this->mutex_attribute);
    //Liberación de memoria para los atributos de creación del mutex
    delete this->mutex_attribute;
//fin del destructor
}

int mutex_t::Inicializar() {
//Implementación del método Inicializar sin parámetros. Este método se encargará de inicializar el mutex usando los atributos de creación del mutex

    /*Inicializar el mutex asociado a la clase usando como parámetros el manejador del mutex y los atributos de creación del mutex.
    Debe retornar el valor que devuelva la función pthread_mutex_init*/
    return pthread_mutex_init(this->mutex_hand, this->mutex_attribute);
//Fin de Inicializar
}


int mutex_t::Lock() {
//Implementación del método Lock sin parámetros. Este método se encargará de acceder al mutex asociado a la clase usando el manejador del mutex de la clase

    //Acceder al mutex con el manejador de la clase. Debe retornar el valor devuelto por dicha función.
    return pthread_mutex_lock(this->mutex_hand);
//Fin de Lock
}


int mutex_t::Unlock() {
    //Implementación del método Unlock sin parámetros. Este método se encargará de liberar el mutex asociado a la clase usando el manejador del mutex de la clase

    //Liberar el mutex asociado a la clase usando como parámetro el manejador del mutex. Debe retornar el valor devuelto por dicha función.
    return pthread_mutex_unlock(this->mutex_hand);
//Fin de Unlock
}

pthread_mutex_t *mutex_t::ObtenerManejador() {
//Implementación del método ObtenerManejador sin parámetros. Este método se encargará devolver el manejador del mutex de la clase

    //Devolver el manejador del mutex de la clase
    return this->mutex_hand;

//Fin de ObtenerManejador
}


/*
***************************************************************************************************************
************************ Implementación de los nuevos métodos para la Prática Posix 5 *************************
***************************************************************************************************************
*/

//Implementar el método AsignarProtocolo que establezca el protocolo del mutex al valor indicado por el parámetro recibido

int mutex_t::AsignarProtocolo(int protocolo) {

//Establecer el protocolo de los atributos de creación del mutex al valor indicado por el parámetro recibido (usar la función pthread_mutexattr_setprotocol)
    pthread_mutexattr_setprotocol(this->mutex_attribute,protocolo);
//Fin del método AsignarProtocolo
}

/*Implementar el método AsignarTecho que establezca el techo de prioridad del mutex al valor indicado por los
parámetros recibidos*/
int mutex_t::AsignarTecho(int techo_prioridad) {

//Establecer el techo de prioridad de los atributos de creación del mutex usando el valor del techo de prioridad recibido por parámetro
    pthread_mutexattr_setprioceiling(this->mutex_attribute,techo_prioridad);
//Fin del método AsignarTecho
}

/*Implementar el método AsignarProtocoloYTecho con dos parámetros que establezca el protocolo y el techo de prioridad del mutex al valor indicado por los
parámetros recibidos*/
int mutex_t::AsignarProtocoloYTecho(int protocolo, int techp_prioridad) {
//Llamar al método AsignarProtocolo pasándole el valor del protocolo recibido
    AsignarProtocolo(protocolo);
//Llamar al método AsignarTecho pasándole el valor del techo de prioridad recibido
    AsignarTecho(techp_prioridad);
//Fin del método AsignarProtocoloYTecho
}

/*
**********************************************************
*************** Clase variable_condicion_t ***************
**********************************************************

La implementación de la clase variable_condicion_t hay que mantenerla sin modificar
*/

//Implementación del constructor sin parámetros. Este método reservará memoria para los atributos de la clase e inicializará los atributos de creación de la variable de condición
variable_condicion_t::variable_condicion_t() {
    //Reserva de memoria para el manejador de la variable de condición de la clase
    this->vcond_hand = new pthread_cond_t();
    //Reserva de memoria para los atributos de creación de la variable de condición
    this->vcond_atributte = new pthread_condattr_t();
    //Inicialización de los atributos de creación de la variable de condición
    pthread_condattr_init(this->vcond_atributte);
//Fin del constructor
}


variable_condicion_t::~variable_condicion_t() {
/*Implementación del destructor sin parámetros. Este método destruirá la variable de condición y los atributos de creación de la variable de condición
y liberará la memoria de los atributos de la clase*/

    //Destrucción de la variable de condición
    pthread_cond_destroy(this->vcond_hand);
    //Liberación de memoria para el manejador de la variable de condición de la clase
    delete this->vcond_hand;
    //Destrucción de los atributos de creación de la variable de condición
    pthread_condattr_destroy(this->vcond_atributte);
    //Liberación de memoria para los atributos de creación de la variable de condición.
    delete this->vcond_atributte;
//Fin del destructor
}


int variable_condicion_t::Inicializar() {
//Implementación del método Inicializar sin parámetros. Este método se encargará de inicializar la variable de condición usando los atributos de creación la variable de condición

    /*Inicializar la variable de condición usando los atributos de clase manejador y atributos de creación la variable de condición.
    Debe retornar el valor que devuelva dicha función*/
    return pthread_cond_init(this->vcond_hand, this->vcond_atributte);
//Fin de Inicializar
}

int variable_condicion_t::Inicializar(pthread_mutex_t *vmutex) {
/*Implementación del método Inicializar con un parámetro de tipo puntero a pthread_mutex_t. Este método se encargará de asociar el mutex recibido por parámetro a la clase
(usar el método AsociarManejadorMutex) así como de inicializar la variable de condición usando los atributos de creación la variable de condición*/

    //Llamar a AsociarManejadorMutex con el parámetro de entrada.
    this->AsociarManejadorMutex(vmutex);
    /*Inicializar la variable de condición usando los atributos de clase manejador y atributos de creación la variable de condición.
    Debe retornar el valor que devuelva dicha función*/
    return pthread_cond_init(this->vcond_hand, this->vcond_atributte);
//Fin de inicializar
}



void variable_condicion_t::AsociarManejadorMutex(pthread_mutex_t *mutex_hand) {
/*Implementación del método AsociarManejadorMutex con un parámetro de tipo puntero a pthread_mutex_t. Este método se encargará de asignar el parámetro de entrada al
atributo de la clase que va a almacenar el mutex asociado. No es necesario reservar memoria para dicho atributo ya que, aunque es un puntero, también lo es el
parámetro de entrada. Al ser ambas variables punteros, el mutex asociado y el parámetro de entrada acabarán apuntando a la misma dirección de memoria.*/

    //Asignar el parámetro de entrada al parámetro que almacenará el mutex asociado.
    this->vmutex = mutex_hand;
//Fin de AsociarManejadorMutex
}

int variable_condicion_t::Wait() {
//Implementación del método Wait sin parámetros. Este método se encargará de bloquear el hilo invocante en la variable de condición de la clase.

    /*Llamar a la función de bloqueo en una variable de condición con los atributos de la clase manejador de la variable de condición y mutex asociado.
    El método retornará el valor que devuelva dicha función.*/
    return pthread_cond_wait(this->vcond_hand, this->vmutex);
//Fin de Wait
}

int variable_condicion_t::TimeWait(struct timespec time) {
/*Implementación del método TimedWait con un parámetro de tipo struct timespec. Este método se encargará de bloquear el hilo en la variable de condición de la clase
como máximo hasta el instante indicado por el parámetro de entrada.*/

    /*Llamar a la función de bloqueo en una variable de cindición con límite temporal con los atributos de la clase manejador de la variable de condición, el mutex asociado
    junto con el paránetro de entrada. El método retornará el valor que devuelva dicha función.*/
    return pthread_cond_timedwait(this->vcond_hand, this->vmutex, &time);
//Fin de TimedWait
}


int variable_condicion_t::Signal() {
//Implementación del método Signal sin parámetros. Este método se encargará de liberar (al menos) un hilo bloqueado en la variable de condición de la clase.

    /*Llamar a la función de liberación de (al menos) un hilo bloqueado en la variable de condición con el atributo de la clase manejador de la variable.
    El método retornará el valor que devuelva dicha función.*/
    return pthread_cond_signal(this->vcond_hand);
//Fin de Signal
}

int variable_condicion_t::Broadcast() {
//Implementación del método Broadcast sin parámetros. Este método se encargará de liberar todos los hilos bloqueados en la variable de condición de la clase.

    /*Llamar a la de liberación de todos los hilos bloqueados en cla variable de condición de la clase con el atributo de la clase manejador de la variable de condición.
    El método retornará el valor que devuelva dicha función.*/
    return pthread_cond_broadcast(this->vcond_hand);
//Fin de Broadcast
}