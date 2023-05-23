with GNAT.IO; use GNAT.IO;
-- Añadir el paquete controlador
with controlador;
use controlador;

procedure p2_2 is
   procedure senial_capturada(capturadas: Natural; max_capturas: Positive) is begin
      Put_line("Se han capturado" & integer'image(capturadas) & " señales SIGINT de" & integer'image(max_capturas));
   end senial_capturada;

   -- Definir el tipo tarea EsperarCtrlC con un parámetro de tipo positive (tiene que ser mayor que 0) para indicar el número de señales esperadas
task type EsperarCtrlC(num_señales: Positive);
   -- Implementación el cuerpo del tipo tarea EsperarCtrlC
   task body EsperarCtrlC is
   -- Zona de declaración
      -- Declarar una variable de tipo Natural (puede contener valores mayores o iguales que 0) para contar el número de ocurrencias de la señal SIGINT e inicializarla a 0
      signal_cont: Natural:= 0;
   -- Zona de código
   begin
      -- Mientras no se hayan recibido las señales esperadas
      while num_señales /= signal_cont loop
         -- Invocar a la entrada Wait de ControladorSigInt para esperar a que se pulse Ctrl+C
         ControladorSigInt.Wait;
         -- Incrementar las señales recibidas
         signal_cont:=signal_cont + 1 ;
         -- Mostrar el mensaje "Se han capturado xxxx señales SIGINT de yyyy" siendo xxxx el número de señales capturadas e yyyy el número de señales esperadas (usar el procedimiento
         -- senial_capturada)
         senial_capturada(signal_cont,num_señales);
      -- Fin mientras
      end loop;
   -- Fin del cuerpo del tipo tarea EsperarCtrlC
   end EsperarCtrlC;
   -- Crear una tarea de tipo EsperarCtrlC pasando por parámetros el número de señales SIGINT que debe recibir
   Tarea:EsperarCtrlC(3);
begin
   null;
end p2_2;
