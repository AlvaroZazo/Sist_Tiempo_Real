-- Inicio del cuerpo del paquete paquetebarrera
package body paquetebarrera is
   -- Implementación del procedimiento SincronizarEnBarrera
procedure SincronizarEnBarrera(barrera: in out Barrera_t)is
      begin
      -- Invoca al procedimiento Sincronizar de la barrera recibida por parámetro
      barrera.Sincronizar;
      -- Invoca a la entrada Wait de la barrera
      barrera.Wait;
      -- Fin del procedimiento SincronizarEnBarrera
      end SincronizarEnBarrera;

   -- Implementación del procedimiento ResetearBarrera
procedure ResetearBarrera (barrera: out Barrera_t; limite_barreta: in Positive) is
     begin
      -- Invoca al procedimiento Resetear de la barrera recibida como primer parámetro usando como parámetro el dato recibido como segundo parámetro
      barrera.Resetear(limite_barreta);
   -- Fin del procedimiento ResetearBarrera
      end  ResetearBarrera;
	
   -- Definir el cuerpo del objeto Barrera_t
   protected body Barrera_t is
      -- Implementar el procedimiento Sincronizar sin parámetros del objeto protegido. Se encargará de incrementar el contador de las tareas que han llegado a la barrera
      -- así como de cambiar el valor del atributo booleano cuando llegue la última tarea para permitir que las tareas se desbloqueen
      procedure Sincronizar is
         begin
         -- Incrementar el contador de tareas
         cont_tareas:=cont_tareas + 1;
         -- Si es la última tarea
         if max_tareas = cont_tareas then
            -- Poner el atributo booleano a verdadero
            cond_paso_tarea:= True;
            -- Fin si
             end if;
      -- Fin del procedimiento Sincronizar
      end Sincronizar;

      -- Definir la entrada Wait sin parámetros del objeto protegido. Se encargará de bloquear a la tarea invocante hasta que hayan llegado todas las tareas,
      -- cosa que indica el atributo booleano del objeto. Esta entrada decrementará el contador de tareas (cuando pasen todas el contador quedará a 0) y pondrá el atributo
      -- booleano a falso cuando hayan pasado todas las tareas
      entry Wait when cond_paso_tarea = True is
         begin
         -- Decrementar el contador de tareas
         cont_tareas:= cont_tareas - 1;
         -- Si es la última tarea
         if cont_tareas = 0 then
            -- Poner el atributo booleano a falso
            cond_paso_tarea:= False;
            -- Fin si
         end if;
      -- Fin de la entrada Wait
      end Wait;

      -- Definir el procedimiento Resetear con un parámetro que se encargará de establecer un nuevo límite en el valor recibido por parámetro (no debe haber tareas en la barrera)
      procedure Resetear(reset: in Positive) is
         begin
         -- Si no hay tareas en la barrera
         if cont_tareas = 0 then
            -- Cambiar el límite de tareas en la barrera
            max_tareas:= reset;
            -- Fin si
         end if;
      -- Fin del procedimiento Resetear
      end Resetear;
   -- Fin del objeto protegido
   end Barrera_t;
-- Fin del paquete paquetebarrera
end paquetebarrera;
