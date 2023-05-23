//Incluir la librería de utilización de hilos posix
#include <pthread.h>

/*
**********************************************************
********************** Clase hilo_t **********************
**********************************************************
*/

// AQUÍ IRÁ LA DEFINICIÓN DE LA CLASE hilo_t DEFINIDA EN LA PRÁCTICA P0_1
class hilo_t {

    //Parte privada
private:
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
    //Parte pública
public:
    //Definir el constructor sin parámetros.
    hilo_t();

    //Definir el destructor sin parámetros.
    ~hilo_t();

    //Definir el método Lanzar sin parámetros y que retorna un entero
    int Lanzar();

    /*Definir el método AsignarFuncion con un parámetro que será la función que utilice el hilo. La definición debe cumplir el formato void *(*Atributo)(void *),
    donde Atributo es el nombre que se le dará al atributo de la función (se recomienda usar nombres descriptivos y no dejar Atributo como nombre).
    La función no devuelve ningún dato. */
    void AsignarFuncion(void*(*start_routine)(void *));

    /*Definir el método AsignarDato con un parámetro de tipo puntero a void que será el dato que se usará con la función que utilice el hilo.
    La función no devuelve ningún dato. */
    void AsignarDato(void *dato);

    /*Definir el método AsignarFuncionYDato con dos parámetros, uno con la función que usará el hilo y el otro con el dato que usará dicha función
    (misma definición que en los métodos AsignarFuncion y AsignarDato). La función no devuelve ningún dato.*/
    void AsignarFuncionYDato(void*(*start_routine)(void *),void *attributes);

    //Definir el método Join sin parámetros y que devuelva un puntero a void.
    void *Join();
    //Definir el método ObtenerManejador sin parámetros y que devuelva el manejador del hilo (pthread_t), no el puntero al manejador.
    pthread_t ObtenerManejador();
    //Definir el método ObtenerIdentificador sin parámetros y que devuelva el identificador del hilo (int).
    int ObtenerIdentificador();
};

/*
**********************************************************
********************** Clase mutex_t *********************
**********************************************************
*/

//Definir la clase mutex_t
class mutex_t {
	//Parte privada
	private:
		//Definir un atributo de clase de tipo puntero a pthread_mutex_t. Este atributo será el manejador del mutex asociado a la clase
        pthread_mutex_t *mutex_hand;
		//Definir un atributo de clase de tipo puntero a pthread_mutexattr_t para guardar los atributos de creación del mutex
        pthread_mutexattr_t *mutex_attribute;
	//Parte pública
	public:
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
};

/*
**********************************************************
*************** Clase variable_condicion_t ***************
**********************************************************
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