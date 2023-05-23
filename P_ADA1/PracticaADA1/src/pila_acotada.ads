--Inicio del paquete pila_acotada
package pila_acotada is

   --Definir un tipo VectorEntero como un array de tamaño indefinido (usar "integer range <>" como rango) de tipo entero
   type VectorEntero is array (Integer range <>) of Integer;
   -- Definir el objeto protegido PilaAcotada con un parámetro entero que indicará el tamaño de la pila. Darle 10 como valor por defecto
   protected type PilaAcotada (T:Positive:=10) is
      -- Añadir la entrada PilaAniadir que reciva el dato a insertar
      entry PilaAniadir(Item: in Integer);
      -- Añadir la entrada PilaSacar que devuelva el dato leído
      entry PilaSacar (Item: out Integer);
   -- definir la zona privada del objeto
      private
      -- Definir e inicializar la constante Tamanio_Pila de tipo entero
      Tamanio_Pila:Positive:=T;
      -- Definir e inicializar el atributo cima de tipo Natural e inicializarlo a 0. Este atributo va a indicar el número de elementos que tiene la pila así como la posición del
      -- último elemento añadido (si Cima>0). Por tanto, para añadir un elemento, se incrementa en la posición indicada por Cima y luego se inserta en la posición indicada
      -- por dicho atributo y para sacar un elemento se accede a la posición indicada por Cima y después se decrementa dicho atributo
      cima: Natural:= 0;
      -- Definir el atributo Pila de tipo VectorEntero estableciendo el rango 1..T para definir el tamaño del array
      Pila:VectorEntero(1..T);
      --Final del objeto protegido PilaAcotada
      end PilaAcotada;
   --Fin del paquete PilaAcotada
end pila_acotada;


