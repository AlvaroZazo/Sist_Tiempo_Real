#include <stdlib.h>
#include <stdio.h>
#include "funciones_time.h"
#include <ClasesPosix.h>
//Añadir la librería de utilización de señales
#include <signal.h>
//Añadir el include de los relojes
#include <time.h>
void imprimir_senial_capturada(int capturadas, int max_capturas) {
	printf("Se han capturado %d señales SIGINT de %d.\n", capturadas, max_capturas);
}

void imprimir_fecha_hora(int dia, int mes, int anio, int hora, int minuto, int segundos, int milisegundos) {
	printf("%d/%d/%d - %d:%d:%d.%d\n", dia, mes, anio, hora, minuto, segundos, milisegundos);
}

//Definir un tipo de datos seniales_t que sea una estructura con dos campos de tipo entero, uno para almacenar las señales recibidas y otro para almacenar las señales esperadas
typedef struct {
    int recibidas;
    int esperadas;
} seniales_t;

/*Definir la función HiloSigInt para la creación de un hilo (seguir el formato indicado para este fin, ver el anexo I de la práctica) que capture la señal SIGINT.
Tendrá un parámetro de entrada de tipo puntero a seniales_t (esta estructura es compartida por todos los hilos, de ahí que sea un puntero)*/
void *HiloSigInt(void *aux) { //Por ejemplo, el parámetro es un puntero a entero

    //Definir una variable de tipo puntero a seniales_t (debe ser puntero porque es compartida por todos los hilos)
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo I de la práctica)
    seniales_t *p_senial = (seniales_t *) aux;
    //Crear un conjunto de señales
    sigset_t setInt;
    //Inicializar el conjunto de señales a vacío
    sigemptyset(&setInt);
    //Añadir SIGINT al conjunto de señales
    sigaddset(&setInt,SIGINT);
    //Añadir SIGUSR1 al conjunto de señales. Es necesario bloquear esta señal porque se usa en otro hilo
    sigaddset(&setInt,SIGUSR1);
    //Añadir el conjunto de señales a la máscara de señales bloqueadas
    pthread_sigmask(SIG_BLOCK, &setInt, NULL);
    //Quitar SIGUSR1 del conjunto de señales. Esto es necesario ya que, aunque bloqueamos las dos señales, en este hilo solamente esperamos por SIGINT
    sigdelset(&setInt,SIGUSR1);
    //Mientras las señales recibidas no lleguen a las esperadas
    while(p_senial->recibidas != p_senial->esperadas) {

        //Esperar por el conjunto de señales, que solamente contiene SIGINT (habrá que crear una variable para usarla como segundo parámetro de la función sigwait)
        int sig;
        sigwait(&setInt, &sig);
        //Incrementar las señales recibidas
        p_senial->recibidas++;
        //Fin mientras
    }
// Fin de la función HiloSigInt
}

/*Definir la función HiloPeriodicoConRetardos para la creación de un hilo (seguir el formato indicado para este fin, ver el anexo I de la práctica) que, mediante retardos,
muestre la hora de diferentes relojes. Tendrá un parámetro de entrada de tipo puntero a seniales_t (esta estructura es compartida por todos los hilos, de ahí que sea un puntero)*/
void *HiloPeriodicoConRetardos(void *aux) {
    //Definir una variable de tipo puntero a seniales_t (debe ser puntero porque es compartida por todos los hilos)
    //Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo I de la práctica 3)
    seniales_t *p_senial = (seniales_t *) aux;
    //Crear un conjunto de señales
    sigset_t setRet;
    //Inicializar el conjunto de señales a vacío
    sigemptyset(&setRet);
    //Añadir SIGINT al conjunto de señales. Es necesario bloquear esta señal porque se usa en otro hilo
    sigaddset(&setRet,SIGINT);
    //Añadir SIGUSR1 al conjunto de señales. Es necesario bloquear esta señal porque se usa en otro hilo
    sigaddset(&setRet,SIGUSR1);
    //Añadir el conjunto de señales a la máscara de señales bloqueadas
    pthread_sigmask(SIG_BLOCK, &setRet, NULL);
    //Definir tres variables de tipo struct timespec para almacenar la lectura de la hora actual, el instante siguiente y el periodo de repetición
    timespec hora_actual, instante_sig, periodo_rep;

    /*Definir una variable de tipo puntero a struct tm para almacenar la transformación de la hora en formato calendario (no se debe reservar memoria).
    Debe ser de tipo puntero porque la función localtime devuelve la dirección de memoria donde se almacena la conversión.*/
     tm *trans_hora;
    //Leer la hora actual usando el reloj CLOCK_MONOTONIC
    clock_gettime(CLOCK_MONOTONIC,&hora_actual);
    //Inicializar el periodo de repetición a 5 segundos (hay que inicializar también los nanosegundos)
    periodo_rep.tv_sec = 5;
    periodo_rep.tv_nsec = 0;
    //Calcular el siguiente instante como la suma de la hora actual y el periodo de repetición (usar las funciones definidas en la librería funciones_time)
    instante_sig = SumaTimeSpec(hora_actual,periodo_rep);
    //Mientras las señales recibidas no lleguen a las esperadas
    while(p_senial->recibidas != p_senial->esperadas) {
        //Leemos la hora actual de CLOCK_MONOTONIC
        clock_gettime(CLOCK_MONOTONIC,&hora_actual);
        //Transformamos la hora actual a formato struct tm (usar la variable definida fuera del bucle) mediante la función localtime (usar los segundos de la hora actual)
         trans_hora = localtime(&hora_actual.tv_sec);

        //Mostrar la hora en formato día/mes/año horas:minutos:segundos.milisegundos. Los milisegundos se obtienen al dividir los nanosegundos de la hora actual por 1000000 (usar la función imprimir_fecha_hora)
        imprimir_fecha_hora(trans_hora->tm_mday,trans_hora->tm_mon + 1,trans_hora->tm_year +1900,trans_hora->tm_hour,trans_hora->tm_min,trans_hora->tm_sec,trans_hora->tm_sec/1000000);
        //Leemos la hora actual de CLOCK_REALTIME
        clock_gettime(CLOCK_REALTIME,&hora_actual);
        //Transformamos la hora actual a formato struct tm (usar la variable definida fuera del bucle) mediante la función localtime (usar los segundos de la hora actual)
        trans_hora = localtime(&hora_actual.tv_sec);
        //Mostrar la hora en formato día/mes/año horas:minutos:segundos.milisegundos. Los milisegundos se obtienen al dividir los nanosegundos de la hora actual por 1000000 (usar la función imprimir_fecha_hora)
        imprimir_fecha_hora(trans_hora->tm_mday,trans_hora->tm_mon + 1,trans_hora->tm_year +1900,trans_hora->tm_hour,trans_hora->tm_min,trans_hora->tm_sec,trans_hora->tm_sec/1000000);
        //Leemos la hora actual de CLOCK_THREAD_CPUTIME_ID
        clock_gettime(CLOCK_THREAD_CPUTIME_ID,&hora_actual);
        //Transformamos la hora actual a formato struct tm (usar la variable definida fuera del bucle) mediante la función localtime (usar los segundos de la hora actual)
        trans_hora = localtime(&hora_actual.tv_sec);
        //Mostrar la hora en formato día/mes/año horas:minutos:segundos.milisegundos. Los milisegundos se obtienen al dividir los nanosegundos de la hora actual por 1000000 (usar la función imprimir_fecha_hora)
        imprimir_fecha_hora(trans_hora->tm_mday,trans_hora->tm_mon + 1,trans_hora->tm_year +1900,trans_hora->tm_hour,trans_hora->tm_min,trans_hora->tm_sec,trans_hora->tm_sec/1000000);
        //Crear un bucle que realice 100000000 de vueltas (el cuerpo del bucle estará vacío). Esta acción incrementará el tiempo de procesador del hilo debido a las sumas y comparaciones del buble
        for(int i = 0;i<100000000;i++){}
        //Dormir el hilo hasta el siguiente instante calculado usando CLOCK_MONOTONIC y retardo absoluto
        clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&instante_sig,NULL);
        //Calcular el siguiente instante añadiendo el periodo al valor calculado previamente (usar las funciones definidas en la librería funciones_time)
        instante_sig = SumaTimeSpec(instante_sig,periodo_rep); //**NO ME FIO MUCHO
        //Fin mientras
    }
//Fin de la función HiloPeriodicoConRetardos
}

/*Definir la función HiloPeriodicoConTemporizador para la creación de un hilo (seguir el formato indicado para este fin, ver el anexo I de la práctica) que, mediante un temporizador,
muestre las señales capturadas actualmente. Tendrá un parámetro de entrada de tipo puntero a seniales_t (esta estructura es compartida por todos los hilos, de ahí que sea un puntero)*/
void *HiloPeriodicoConTemporizador(void *aux) {
	//Definir una variable de tipo puntero a seniales_t
	//Asignar el contenido del parámetro de entrada a la variable anterior siguiento la forma indicada en clase (ver apuntes del tema 2 o el anexo I de la práctica 3)
    seniales_t *p_senial = (seniales_t *) aux;
	//Crear un conjunto de señales
    sigset_t setTemp;
	//Inicializar el conjunto de señales a vacío
    sigemptyset(&setTemp);
	//Añadir SIGINT al conjunto de señales. Es necesario bloquear esta señal porque se usa en otro hilo
    sigaddset(&setTemp,SIGINT);
	//Añadir SIGUSR1 al conjunto de señales
    sigaddset(&setTemp,SIGUSR1);
	//Añadir el conjunto de señales a la máscara de señales bloqueadas
    pthread_sigmask(SIG_BLOCK, &setTemp, NULL);
	//Quitar SIGINT del conjunto de señales. Esto es necesario ya que, aunque bloqueamos las dos señales, solamente esperamos por SIGUSR1
    sigdelset(&setTemp,SIGINT);
	//Definir una variable tipo timer_t que sirva de manejador del temporizador
    timer_t mane_temp;
	//Definir una variable evento de tipo struct sigevent para indicar la acción del temporizador
    sigevent temp_act;
	//Indicar en la variable evento que el temporizador generará una señal
    temp_act.sigev_notify = SIGEV_SIGNAL;
	//Indicar en la variable evento que la señal generada por el temporizador es SIGUSR1
    temp_act.sigev_signo = SIGUSR1;
	//Crear el temporizador usando CLOCK_MONOTONIC, evento y el manejador del temporizador
    timer_create(CLOCK_MONOTONIC,&temp_act,&mane_temp);
	//Definir una variable datos_temporizador de tipo struct itimerspec
    itimerspec datos_temporizador;
	//Leer la hora actual de CLOCK_MONOTONIC y almacenarla en el campo it_value de datos_temporizador
    clock_gettime(CLOCK_MONOTONIC,&datos_temporizador.it_value);
	//Añadir dos segundos al campo it_value de datos_temporizador
    datos_temporizador.it_value.tv_sec = datos_temporizador.it_value.tv_sec + 2;
	//Establecer el campo it_interval de datos_temporizador a 2 segundos (hay que inicializar también los nanosegundos)
    datos_temporizador.it_interval.tv_sec = 2;
    datos_temporizador.it_interval.tv_nsec = 0;
	/*Armar el temporizador usando el manejador del temporizador, retardos absolutos y datos_temporizador (no es necesario guardar el tiempo restante en caso de salida prematura,
	por lo que se puede usar NULL)*/
    timer_settime(mane_temp,TIMER_ABSTIME,&datos_temporizador,NULL);
	//Mientras las señales recibidas no lleguen a las esperadas
    while(p_senial->recibidas != p_senial->esperadas) {
        //Imprimir el número de señales recibidas actualmente (usar la función imprimir_senial_capturada)
        imprimir_senial_capturada(p_senial->recibidas,p_senial->esperadas);
        //Esperar por el conjunto de señales, que solamente contiene SIGUSR1 (habrá que crear una variable para usarla como segundo parámetro de la función sigwait)
        int sig;
        sigwait(&setTemp, &sig);
        //Fin mientras
    }
//Fin de la función HiloPeriodicoConTemporizador
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
	seniales_t seniales;
	seniales.esperadas= atoi(argv[1]);
	seniales.recibidas=0;
	//Crear un conjunto de señales
    sigset_t set;
	//Inicializar el conjunto de señales a vacío
    sigemptyset(&set);
	//Añadir SIGINT al conjunto de señales. Es necesario bloquear esta señal porque se usa en otro hilo
    sigaddset(&set,SIGINT);
	//Añadir SIGUSR1 al conjunto de señales. Es necesario bloquear esta señal porque se usa en otro hilo
    sigaddset(&set,SIGUSR1);
	//Añadir el conjunto de señales a la máscara de señales bloqueadas
    pthread_sigmask(SIG_BLOCK, &set, NULL);
	//Crear un manejador de hilo para cada uno de los hilos que se van a crear (uno por cada una de las funciones anteriores)
    hilo_t t_INT, t_Ret, t_Temp;
	//Crear un hilo con la función HiloSigInt y el parámetro seniales
    t_INT.AsignarFuncion(HiloSigInt);
    t_INT.AsignarDato(&seniales);
	//Crear un hilo con la función HiloPeriodicoConRetardos y el parámetro seniales
    t_Ret.AsignarFuncion(HiloPeriodicoConRetardos);
    t_Ret.AsignarDato(&seniales);
	//Crear un hilo con la función HiloPeriodicoConTemporizador y el parámetro seniales
    t_Temp.AsignarFuncion(HiloPeriodicoConTemporizador);
    t_Temp.AsignarDato(&seniales);

    t_INT.Lanzar();
    t_Temp.Lanzar();
    t_Ret.Lanzar();


	//Esperar al hilo HiloSigInt
    t_INT.Join();
	//Esperar al hilo HiloPeriodicoConRetardos
    t_Ret.Join();
	//Esperar al hilo HiloPeriodicoConTemporizador
    t_Temp.Join();
}
