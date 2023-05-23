-- Añadir el paquete  de interrupciones de ADA
with Ada.Interrupts;
use Ada.Interrupts;
-- Añadir el paquete  de nombres de interrupciones de ADA
with Ada.Interrupts.Names;
use Ada.Interrupts.Names;

-- Inicio del paquete controlador
package controlador is
   -- Crear un objeto protegido de nombre ControladorSigInt (no pondremos el type, ya que queremos creal el objeto específico y no un tipo)
   protected ControladorSigInt is
      -- Definir un procedimiento Interrupcion sin parámetros
      procedure Interrupcion;
      -- Definir una entrada Wait sin parámetros
      entry Wait;
      -- Incluir el pragma Interrupt_Handler con Interrupcion como parámetro (indica que Interrupcion será un manejador de interrupción)
      pragma Interrupt_Handler(Interrupcion);
      -- Incluir el pragma Attach_Handler con Interrupcion y Sigint como parámetros (indicará que Interrupcion manejará la señal Sigint)
      pragma Attach_Handler(Interrupcion , SIGINT);
      -- Incluir el pragma Unreserve_All_Interrupts sin parámetros (necesario para que se puedan manejar las interrupciones)
      pragma Unreserve_All_Interrupts;
   -- Zona privada del objeto
   private
      -- Definir un atributo booleano e inicializarlo a falso para indicar si se ha producido la ocurrencia de la señal o no
      signal_arrive: Boolean:= False;
   -- Fin del objeto ControladorSigInt
   end ControladorSigInt;
-- Fin del paquete controlador
end controlador;
