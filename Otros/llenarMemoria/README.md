Opciones de compilación:
- Opción todo a 0: solo ocupa la memoria de las variables globales definidas pero no inicializadas. 822 bytes libres.
- Opción todo a 0 y eliminar el array de 100 Strings global: Dispongo de toda la memoria.
- Opción LOCAL = 1: defino una nueva variable local en el loop pero al inicializarla en cada loop no aumenta la memoria.
- Opción STRINGS = 1: tengo un array de 100 objetos Strings y a medida que los voy inicializando con 10 bytes lleno el heap y acabo llenando la memoria
- Opción GLOBAL = 1: en el momento que inicalizo el array de 1000 long asignando un valor me ocupa 4000 bytes en la RAM y ya de un error de compilación.
