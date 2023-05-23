#include <vector>

using namespace std;
//Incluir la librería de utilización de hilos posix
#include <pthread.h>

/*
**********************************************************
********************** Clase mutex_t *********************
**********************************************************

Se va a utilizar el tipo mutex_t dentro de la clase hilo_t, por lo que hay que mover la definición de mutex_t por delante de hilo_t
*/

//Definir la clase mutex_t
class mutex_t {
    //Parte privada

private:
    /*
    Los atributos definidos para esta clase en la Práctica Posix 0 hay que mantenerlos sin modificar
    */

    //Definir un atributo de clase de tipo puntero a pthread_mutex_t. Este atributo será el manejador del mutex asociado a la clase
    pthread_mutex_t *mutex_hand;
    //Definir un atributo de clase de tipo puntero a pthread_mutexattr_t para guardar los atributos de creación del mutex
    pthread_mutexattr_t *mutex_attribute;

    //Parte pública
public:
    /*
    Los métodos definidos para esta clase en la Práctica Posix 0 hay que mantenerlos sin modificar
    */

    //Definir el constructor sin parámetros.
    mutex_t();

    //Definir el destructor sin parámetros.
    ~mutex_t();

    //Definir el método Inicializar sin parámetros y que retorne un entero.
    int Inicializar();

    //Definir el método Lock sin parámetros y que retorna un entero.
    int Lock();

    //Definir el método Unlock sin parámetros y que retorna un entero.
    int Unlock();

    //Definir el método ObtenerManejador sin parámetros y que retorna un puntero a pthread_mutex_t
    pthread_mutex_t *ObtenerManejador();

    /*
   ************************************************************************************************
   **************************** Nuevos métodos para la Prática Posix 5 ****************************
   ************************************************************************************************

   Los siguientes métodos hay que añadirlos a los métodos públicos definidos en la Práctica Posix 0
   */
    //Definir el método AsignarProtocolo que retorne un entero y reciba un entero para indicar el protocolo de prioridades dinámicas
    int AsignarProtocolo(int protocolo);

    //Definir el método AsignarTecho que retorne un entero y reciba un entero para indicar el techo de prioridad del mutex
    int AsignarTecho(int techo_prioridad);

    //Definir el método AsignarProtocoloYTecho que retorne un entero y reciba un entero para indicar el protocolo de prioridades dinámicas y otro entero para indicar el techo de prioridad del mutex
    int AsignarProtocoloYTecho(int protocolo, int techp_prioridad);
};

/*
**********************************************************
********************** Clase hilo_t **********************
**********************************************************
*/

//Definir la clase hilo_t
class hilo_t {
    //Parte privada
private:

    /*Los atributos definidos para esta clase en la Práctica Posix 4 hay que mantenerlos sin modificar

/*Los atributos definidos para esta clase en la Práctica Posix 0 hay que mantenerlos sin modificar*/
    //Definir un atributo de clase de tipo entero para almacenar el identificador del hilo

    int identificador;

    //Definir un atributo de clase de tipo puntero a pthread_t. Este atributo será el manejador del hilo asociado a la clase
    pthread_t *manejador;
    //Definir un atributo de clase de tipo puntero a pthread_attr_t para guardar los atributos de creación del hilo
    pthread_attr_t *attributes;

    /*Definir un atributo de clase para almacenar la función asignada al hilo. La definición debe cumplir el formato void *(*Atributo)(void *),
    donde Atributo es el nombre que se le dará al atributo de la clase (se recomienda usar nombres descriptivos y no dejar Atributo como nombre)*/
    void *(*start_routine)(void *);

    //Definir un atributo de clase de tipo puntero a void para almacenar los datos enviados a la función
    void *datosHilos;

    //Definir tres variables enteras para almacenar el tiempo de ejecución, la prioridad y la política del hilo
    int t_exec, priority, policy;
    //Definir dos variables de tipo struct timespec para almacenar el periodo de repetición y el instante crítico del hilo
    struct timespec period_rep, crit_int;


    /*
   ************************************************************************************************
   *************************** Nuevos atributos para la Prática Posix 5 ***************************
   ************************************************************************************************

   Los siguientes atributos hay que añadirlos a los atributos privados definidos en la Práctica Posix 4
   */

    //Definir un atributo de tipo vector de enteros (usar la clase vector) para guardar la lista de acciones que ejecutará la tarea periódica del hilo
    vector<int> acciones;


    //Definir un atributo de tipo vector de enteros (usar la clase vector) para guardar la lista de recursos que usará la tarea periódica del hilo
    vector<int> recursos;
    /*Definir un atributo de tipo vector de puntero a mutex_t (usar la clase vector) para guardar la lista de mutex de la aplicación (un mutex por cada recurso).
    Usamos punteros para que los mutex sean compartidos por todos los hilos*/
    vector<mutex_t*> mutex_recursos;
    //Parte pública
public:
    /*
    Los métodos definidos para esta clase en la Práctica Posix 5 hay que mantenerlos sin modificar
*/
//Definir el constructor sin parámetros.
    hilo_t();

    //Definir el destructor sin parámetros.
    ~hilo_t();

    //Definir el método Lanzar sin parámetros y que retorna un entero
    int Lanzar();

    /*Definir el método AsignarFuncion con un parámetro que será la función que utilice el hilo. La definición debe cumplir el formato void *(*Atributo)(void *),
    donde Atributo es el nombre que se le dará al atributo de la función (se recomienda usar nombres descriptivos y no dejar Atributo como nombre).
    La función no devuelve ningún dato. */
    void AsignarFuncion(void *(*start_routine)(void *));

    /*Definir el método AsignarDato con un parámetro de tipo puntero a void que será el dato que se usará con la función que utilice el hilo.
    La función no devuelve ningún dato. */
    void AsignarDato(void *dato);

    /*Definir el método AsignarFuncionYDato con dos parámetros, uno con la función que usará el hilo y el otro con el dato que usará dicha función
    (misma definición que en los métodos AsignarFuncion y AsignarDato). La función no devuelve ningún dato.*/
    void AsignarFuncionYDato(void *(*start_routine)(void *), void *attributes);

    //Definir el método Join sin parámetros y que devuelva un puntero a void.
    void *Join();

    //Definir el método ObtenerManejador sin parámetros y que devuelva el manejador del hilo (pthread_t), no el puntero al manejador.
    pthread_t ObtenerManejador();

    //Definir el método ObtenerIdentificador sin parámetros y que devuelva el identificador del hilo (int).
    int ObtenerIdentificador();

    /*Definir el método EstablecerAtributos que devolverá un valor entero y que recibirá los siguientes atributos (mantener el orden descrito): prioridad, política,
    herencia, periodo de repetición (vendrá dado en milisegundos, se transformará a struct timespec dentro del método), tiempo de ejecución de la tarea periódica del hilo
    y el instante de comienzo. Todos los parámetros son de tipo entero excepto el último, que es de tipo struct timespec*/

    int EstablecerAtributos(int prioridad, int politica,int herecia, int periodo_rep,
                            int tiempo_exec_tperiodica, struct timespec tiempo_comienzo_tarea);

    //Definir el método  sin parámetros y que devuelva un struct timespec
    struct timespec ObtenerPeriodo();

    //Definir el método ObtenerInstanteDeComienzo sin parámetros y que devuelva un struct timespec
    struct timespec ObtenerInstanteDeComienzo();

    //Definir el método ObtenerTiempoEjecucion sin parámetros y que devuelva un entero
    int ObtenerTiempoEjecucion();

    //Definir el método ObtenerPrioridad sin parámetros y que devuelva un entero
    int ObtenerPrioridad();

    //Definir el método ObtenerPolitica sin parámetros y que devuelva un entero
    int ObtenerPolitica();



     /*
    ************************************************************************************************
    **************************** Nuevos métodos para la Prática Posix 5 ****************************
    ************************************************************************************************

    Los siguientes métodos hay que añadirlos a los métodos públicos definidos en la Práctica Posix 5
    */

    /*Definir el método EstablecerAtributos que devolverá un valor entero y que recibirá los siguientes atributos (mantener el orden descrito):
        - Prioridad, política, herencia, periodo de repetición (vendrá dado en milisegundos, se transformará a struct timespec dentro del método), tiempo de ejecución de la
            tarea periódica del hilo e identificador del hilo (todos de tipo entero)
        - Instante de comienzo (de tipo struct timespec)
        - Vector de acciones del hilo y vector de recursos del hilo (de tipo vector de enteros, usar la clase vector)
        - Vector de mutex de la aplicación (de tipo vector de punteros a mutex_t, usar la clase vector)*/


    int EstablecerAtributos(int prioridad, int politica,int herecia, int periodo_rep,
                                    int tiempo_exec_tperiodica, struct timespec tiempo_comienzo_tarea, vector<int> acciones, vector<int> recursos, vector< mutex_t*> mutex_recursos);
    //Definir el método ObtenerNumAcciones sin parámetros que retorne un valor entero que será el número de acciones del vector de acciones del hilo.
    int ObtenerNumAcciones();
    /*Definir el método ObtenerAccion con un parámetro entero que indicará la acción que se desea obtener y que retorne un valor entero que será la acción de la lista de
    acciones indicada por el parámetro de entrada*/
    int ObtenerAccion(int accion);
    /*Definir el método ObtenerRecurso con un parámetro entero que indicará el recurso que se desea obtener y que retorne un valor entero que será el recurso de la lista de
    recursos indicado por el parámetro de entrada*/
    int ObtenerRecurso(int recurso);
    /*Definir el método ObtenerMutex con un parámetro entero que indicará el mutex que se desea obtener y que retorne un puntero a mutex_t que será el mutex de la lista de
    mutex indicado por el parámetro de entrada*/
    mutex_t* ObtenerMutex(int mutex);

};

/*
**********************************************************
*************** Clase variable_condicion_t ***************
**********************************************************

La clase variable_condicion_t hay que mantenerla sin modificar
*/

//Definir la clase variable_condicion_t
class variable_condicion_t {
    //Parte privada
private:
    //Definir un atributo de clase de tipo puntero a pthread_cond_t. Este atributo será el manejador de la variable de condición asociada a la clase.
    pthread_cond_t *vcond_hand;
    //Definir un atributo de clase de tipo puntero a pthread_condattr_t para guardar los atributos de creación de la variable de condición.
    pthread_condattr_t *vcond_atributte;
    //Definir un atributo de clase de tipo puntero a pthread_mutex_t para guardar el mutex al que se asociará la variable de condición.
    pthread_mutex_t *vmutex;
    //Parte pública
public:
    //Definir el constructor sin parámetros.
    variable_condicion_t();

    //Definir el destructor sin parámetros.
    ~variable_condicion_t();

    //Definir el método Inicializar sin parámetros y que retorne un entero.
    int Inicializar();

    //Definir el método Inicializar con un parámetro de tipo puntero a pthread_mutex_t y que retorne un entero.
    int Inicializar(pthread_mutex_t *vmutex);

    //Definir el método AsociarManejadorMutex con un parámetro de tipo puntero a pthread_mutex_t y que no retorne nada.
    void AsociarManejadorMutex(pthread_mutex_t *mutex_hand);

    //Definir el método Wait sin parámetros y que retorna un entero.
    int Wait();

    //Definir el método TimedWait con un parámetro de tipo struct timespec y que retorna un entero.
    int TimeWait(struct timespec time);

    //Definir el método Signal sin parámetros y que retorna un entero.
    int Signal();

    //Definir el método Broadcast sin parámetros y que retorna un entero.
    int Broadcast();

};