-- Inicio del cuerpo del paquete paquetebarrera
package body paquetebarrera is
   -- Implementaci�n del procedimiento SincronizarEnBarrera
procedure SincronizarEnBarrera(barrera: in out Barrera_t)is
      begin
      -- Invoca al procedimiento Sincronizar de la barrera recibida por par�metro
      barrera.Sincronizar;
      -- Invoca a la entrada Wait de la barrera
      barrera.Wait;
      -- Fin del procedimiento SincronizarEnBarrera
      end SincronizarEnBarrera;

   -- Implementaci�n del procedimiento ResetearBarrera
procedure ResetearBarrera (barrera: out Barrera_t; limite_barreta: in Positive) is
     begin
      -- Invoca al procedimiento Resetear de la barrera recibida como primer par�metro usando como par�metro el dato recibido como segundo par�metro
      barrera.Resetear(limite_barreta);
   -- Fin del procedimiento ResetearBarrera
      end  ResetearBarrera;
	
   -- Definir el cuerpo del objeto Barrera_t
   protected body Barrera_t is
      -- Implementar el procedimiento Sincronizar sin par�metros del objeto protegido. Se encargar� de incrementar el contador de las tareas que han llegado a la barrera
      -- as� como de cambiar el valor del atributo booleano cuando llegue la �ltima tarea para permitir que las tareas se desbloqueen
      procedure Sincronizar is
         begin
         -- Incrementar el contador de tareas
         cont_tareas:=cont_tareas + 1;
         -- Si es la �ltima tarea
         if max_tareas = cont_tareas then
            -- Poner el atributo booleano a verdadero
            cond_paso_tarea:= True;
            -- Fin si
             end if;
      -- Fin del procedimiento Sincronizar
      end Sincronizar;

      -- Definir la entrada Wait sin par�metros del objeto protegido. Se encargar� de bloquear a la tarea invocante hasta que hayan llegado todas las tareas,
      -- cosa que indica el atributo booleano del objeto. Esta entrada decrementar� el contador de tareas (cuando pasen todas el contador quedar� a 0) y pondr� el atributo
      -- booleano a falso cuando hayan pasado todas las tareas
      entry Wait when cond_paso_tarea = True is
         begin
         -- Decrementar el contador de tareas
         cont_tareas:= cont_tareas - 1;
         -- Si es la �ltima tarea
         if cont_tareas = 0 then
            -- Poner el atributo booleano a falso
            cond_paso_tarea:= False;
            -- Fin si
         end if;
      -- Fin de la entrada Wait
      end Wait;

      -- Definir el procedimiento Resetear con un par�metro que se encargar� de establecer un nuevo l�mite en el valor recibido por par�metro (no debe haber tareas en la barrera)
      procedure Resetear(reset: in Positive) is
         begin
         -- Si no hay tareas en la barrera
         if cont_tareas = 0 then
            -- Cambiar el l�mite de tareas en la barrera
            max_tareas:= reset;
            -- Fin si
         end if;
      -- Fin del procedimiento Resetear
      end Resetear;
   -- Fin del objeto protegido
   end Barrera_t;
-- Fin del paquete paquetebarrera
end paquetebarrera;
