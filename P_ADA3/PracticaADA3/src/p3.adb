with GNAT.IO; use GNAT.IO;
-- A�adir el paquete controlador (usar el implementado en el ejercicio 2 de la Pr�ctica ADA 2)
with controlador;
use controlador;
-- A�adir el paquete de tiempo real de ADA
with Ada.Real_Time;
use Ada.Real_Time;
-- .A�adir el paquete calendario de ADA
with Ada.Calendar;
use Ada.Calendar;


procedure p3 is
   procedure senial_capturada(capturadas: Natural; max_capturas: Positive) is begin
      Put_line("Se han capturado" & integer'image(capturadas) & " se�ales SIGINT de" & integer'image(max_capturas));
   end senial_capturada;
	
   procedure TiempoCumplido is begin
      Put_line("Han pasado 6 segundos sin recibir una senal");
   end TiempoCumplido;
	
   procedure MostrarFechaYHora(d�a:Day_Number; mes:Month_Number; a�o:Year_Number; horas, minutos, segundos:natural) is begin
      Put_line(integer'image(d�a)(2..integer'image(d�a)'length) & "\" & integer'image(mes)(2..integer'image(mes)'length) & "\" & integer'image(a�o)(2..integer'image(a�o)'length) & " --- " & integer'image(horas)(2..integer'image(horas)'length) & ":"& integer'image(minutos)(2..integer'image(minutos)'length) & ":" & integer'image(segundos)(2..integer'image(segundos)'length));
   end MostrarFechaYHora;

   -- Definir un tipo puntero a natural
   type PNatural is access all Natural;

   -- Definir el tipo tarea TareaSigInt con dos par�metros: el primero ser� de tipo puntero a natural (al ser puntero permitir� compartir el valor entre todas las tareas)
   -- que almacenar� el n�mero de se�ales recibidas y el segundo ser� de tipo positive e indicar� el n�mero de se�ales esperadas.
   task type TareaSigInt(num_sig_recibidas: PNatural ; num_sig_esperadas: Positive);
   
   
   --Definir el cuerpo del tipo tarea TareaSigInt
   task body TareaSigInt is
      -- Comienzo de la zona del c�digo de TareaSigInt
      begin
      -- Mientras no se hayan recibido las se�ales esperadas (como el par�metro de las se�ales recibidas es un puntero, debe usarse .all para acceder a su contenido)
      
      while num_sig_esperadas /= num_sig_recibidas.all loop
         -- Como debemos mostrar un mensaje si no se ha pulsado Ctrl+C en 6 segundos, a�adir la cl�usula Select
         select
            -- Invocar a la entrada Wait de ControladorSigInt para esperar a que se pulse Ctrl+C
            ControladorSigInt.Wait;
            -- Incrementar las se�ales recibidas (de nuevo, debemos usar .all para acceder al contenido)
            
             num_sig_recibidas.all:= num_sig_recibidas.all + 1;
         -- cl�usula or
         or
            -- Retardo relativo de 6.0 segundos
              delay(6.0);
            -- Mostrar por pantalla el mensaje "Han pasado 6 segundos sin recibir una senal" (usar el procedimiento TiempoCumplido)
            TiempoCumplido;
         -- Fin select
         end select;
      -- Fin mientras
      end loop;
   -- Fin de TareaSigInt;
   end TareaSigInt;

   -- Definir el tipo tarea MostrarHora con dos par�metros: el primero ser� de tipo puntero a natural (al ser puntero permitir� compartir el valor entre todas las tareas)
   -- que almacenar� el n�mero de se�ales recibidas y el segundo ser� de tipo positive e indicar� el n�mero de se�ales esperadas.
task type MostrarHora(num_sig_recibidas: PNatural ; num_sig_esperadas: Positive);
   -- Definir el cuerpo de MostrarHora

   task body MostrarHora is
      -- Comienzo de la zona de declaraci�n de MostrarHora
      -- Definir variables de tipo Day_Number, Month_Number y Year_Number para almacenar el d�a, el mes y el a�o respectivamente
      dia: Day_Number;
      mes: Month_Number;  
      a�o: Year_Number;
      -- Definir una variable de tipo Day_Duration para almacenar los segundos
      seg: Day_Duration;
      -- Definir tres variables de tipo natural para almacenar la hora, los minutos y los segundos (la variable relativa a los segundos nos servir� para convertir los segundos
      -- de tipo Day_Duration a natural y poder trabajar con ellos)
      hora: Natural;
      minutos: Natural;
      segundos: Natural;
      
      -- Definir una variable de tipo time del paquete calendario para leer la hora del reloj calendario
      caldendar_clock: Ada.Calendar.Time;
      -- Definir una variable de tipo time del paquete de tiempo real para almacenar el siguiente instante de repetici�n
      next_inst: Ada.Real_Time.Time;
      -- Definir una variable de tipo Time_Span para almacenar el periodo de repetici�n y asignarle 5 segundos (usar la funci�n Milliseconds, que transforma un valor num�rico
      -- a milisegundos)
      period_rep: Time_Span := Milliseconds(6000);
   -- Comienzo de la zona del c�digo de MostrarHora
   begin
      -- calculamos el siguiente instante de repetici�n como la hora actual (se optiene con la funci�n Clock del reloj de tiempo real) m�s el periodo de repetici�n
       next_inst := Ada.Real_Time.Clock + period_rep;
      -- Mientras no se hayan recibido las se�ales esperadas (como el par�metro de las se�ales recibidas es un puntero, debe usarse .all para acceder a su contenido)
      while num_sig_esperadas /= num_sig_recibidas.all loop
         -- Leer la hora del reloj calendario
         caldendar_clock := Ada.Calendar.Clock;
         -- Transformar tiempo_calendario a a�os, meses, d�as y segundos. Puede usarse el procedimiento Split para hacerlo todo de golpe o las funciones Year, Month, Day y Seconds
         --para hacerlo paso a paso
         Split(caldendar_clock,a�o,mes,dia,seg);
         -- Transformar los segundos a un valor natural. El casting en ada se hace de la siguiente forma: tipo(valor)
         segundos:= Natural(seg);
         -- Calcular la hora actual como los segundos divididos entre 3600
         hora:= segundos/3600;
         -- Eliminar los segundos correspondientes a las horas (restar a los segundos totales las horas multiplicadas por 3600)
         segundos:= segundos - (hora * 3600);
         -- Calcular los minutos como los segundos divididos entre 60
         minutos:= segundos/60;
         -- Eliminar los segundos correspondientes a los minutos (restar a los segundos totales los minutos multiplicados por 60)
         segundos:= segundos - (minutos * 60);
         -- Mostrar por pantalla dia/mes/a�o --- horas:minutos:segundos. Usar el procedimiento MostrarFechaYHora
         MostrarFechaYHora(dia,mes,a�o,hora,minutos,segundos);
         -- Esperar hasta el siguiente instante usando retardos absolutos
         delay until next_inst;
         -- Calcular el seguiente instante a�adi�ndole al siguiente instante actual el periodo
         next_inst:= next_inst + period_rep;
      -- Fin mientras
      end loop;
   -- Fin MostrarHora
   end MostrarHora;

   -- Definir el tipo tarea mostrarCtrlC con dos par�metros: el primero ser� de tipo puntero a natural (al ser puntero permitir� compartir el valor entre todas las tareas)
   -- que almacenar� el n�mero de se�ales recibidas y el segundo ser� de tipo positive e indicar� el n�mero de se�ales esperadas.
task type mostrarCtrlC(num_sig_recibidas: PNatural ; num_sig_esperadas: Positive );
   -- Definir el cuerpo de mostrarCtrlC
task body mostrarCtrlC is
   -- Comienzo de la zona de declaraci�n de mostrarCtrlC
      -- Definir una variable de tipo time del paquete de tiempo real para almacenar el siguiente instante de repetici�n
   inst_rep: Ada.Real_Time.Time;
      -- Definir una variable de tipo Time_Span para almacenar el periodo de repetici�n y asignarle 2 segundos (usar la funci�n Milliseconds, que transforma un valor num�rico
      -- a milisegundos)
   period_time: Time_Span := Milliseconds(2000);
   -- Comienzo de la zona del c�digo de mostrarCtrlC
   begin
      -- calculamos el siguiente instante de repetici�n como la hora actual (se optiene con la funci�n Clock del reloj de tiempo real) m�s el periodo de repetici�n
      inst_rep := Ada.Real_Time.Clock + period_time;
      -- Mientras no se hayan recibido las se�ales esperadas (como el par�metro de las se�ales recibidas es un puntero, debe usarse .all para acceder a su contenido)
   while num_sig_esperadas /= num_sig_recibidas.all loop
         -- Mostrar el n�mero de se�ales recibidas del total a capturar (usar el procedimiento senial_capturada)
         senial_capturada(num_sig_recibidas.all,num_sig_esperadas);
         -- Esperar hasta el siguiente instante usando retardos absolutos
         delay until inst_rep;
         -- Calcular el seguiente instante a�adi�ndole al siguiente instante actual el periodo
          inst_rep:= inst_rep + period_time;
      -- Fin mientras
      end loop;
   -- Fin mostrarCtrlC
end mostrarCtrlC;

   -- Definir una variable de tipo positive para indicar el n�mero de se�ales esperadas y asignarle un valor
   num_signals: Positive :=3;
   -- Definir una variable de tipo natural para indicar el n�mero de se�ales recibidas permitiendo el acceso a su direcci�n de memoria (cl�usula aliased antes del tipo)
   -- e inicializarla a 0
   num_sig_rec: aliased Natural:= 0;
   -- Definir una tarea de tipo MostrarHora con la direcci�n de memoria de las se�ales recibidas y la variable de se�ales esperadas como par�metros
   TareaHora:MostrarHora(num_sig_rec'Access,num_signals);
   -- Definir una tarea de tipo TareaSigInt con la direcci�n de memoria de las se�ales recibidas y la variable de se�ales esperadas como par�metros
   TareaInt:TareaSigInt(num_sig_rec'Access,num_signals);
   -- Definir una tarea de tipo mostrarCtrlC con la direcci�n de memoria de las se�ales recibidas y la variable de se�ales esperadas como par�metros
   TareaMostrarC:mostrarCtrlC(num_sig_rec'Access,num_signals);
begin
   null;
end p3;
