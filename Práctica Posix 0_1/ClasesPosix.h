//Incluir la librería de utilización de hilos posix
#include <pthread.h>

/*
**********************************************************
********************** Clase hilo_t **********************
**********************************************************
*/

//Definir la clase hilo_t
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
