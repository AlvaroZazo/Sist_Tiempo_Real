-- Inicio del cuerpo del paquete controlador
package body controlador is
   -- Implementación del cuerpo del objeto protegido ControladorSigInt
    protected body ControladorSigInt is
      -- Implementación del procedimiento Interrupcion que lo único que hace es poner el atributo booleano a verdadero (este procedimiento se ejecutará cuando ocurra la interrupción)
   procedure Interrupcion is
      begin
      signal_arrive:=True;
      end Interrupcion;

      -- Implementación de la entrada Wait que esperará a que se produzca una ocurrencia (atributo booleano a verdadero) y lo único que hace es volver a ponerla a falso
      -- para que el objeto pueda volver a capturar señales
      entry Wait when signal_arrive = True is
         begin
         signal_arrive:= False;
      end Wait;

   -- Fin del objeto protegido ControladorSigInt
    end ControladorSigInt;
-- Fin del cuerpo del paquete controlador
end controlador;
