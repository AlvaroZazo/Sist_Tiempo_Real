--Inicio del paquete pila_acotada
package pila_acotada is

   --Definir un tipo VectorEntero como un array de tama�o indefinido (usar "integer range <>" como rango) de tipo entero
   type VectorEntero is array (Integer range <>) of Integer;
   -- Definir el objeto protegido PilaAcotada con un par�metro entero que indicar� el tama�o de la pila. Darle 10 como valor por defecto
   protected type PilaAcotada (T:Positive:=10) is
      -- A�adir la entrada PilaAniadir que reciva el dato a insertar
      entry PilaAniadir(Item: in Integer);
      -- A�adir la entrada PilaSacar que devuelva el dato le�do
      entry PilaSacar (Item: out Integer);
   -- definir la zona privada del objeto
      private
      -- Definir e inicializar la constante Tamanio_Pila de tipo entero
      Tamanio_Pila:Positive:=T;
      -- Definir e inicializar el atributo cima de tipo Natural e inicializarlo a 0. Este atributo va a indicar el n�mero de elementos que tiene la pila as� como la posici�n del
      -- �ltimo elemento a�adido (si Cima>0). Por tanto, para a�adir un elemento, se incrementa en la posici�n indicada por Cima y luego se inserta en la posici�n indicada
      -- por dicho atributo y para sacar un elemento se accede a la posici�n indicada por Cima y despu�s se decrementa dicho atributo
      cima: Natural:= 0;
      -- Definir el atributo Pila de tipo VectorEntero estableciendo el rango 1..T para definir el tama�o del array
      Pila:VectorEntero(1..T);
      --Final del objeto protegido PilaAcotada
      end PilaAcotada;
   --Fin del paquete PilaAcotada
end pila_acotada;


