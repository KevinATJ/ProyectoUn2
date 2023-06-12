# ProyectoUn2

Desarrollo:

Este proyecto consiste en simples palabras en un viaje de un aprendiz a lo largo de varias aldeas, este puede entrenar en estas aldeas, 
(combatiendo a los guardianes de las aldeas)logrando de esta forma aumentar su nivel de poder para que cuando llegue a Tesla con el poder 
suficiente comienze su enfrentamiento con Stormheart.

Requisitos/recomendaciones:

-Se recomienda un compilador para c++ (en este caso use Dev-c++ 5.11)
-Se recomienda usar el programa sin cambiar nada de los archivos .csv ya que puede llegar a generar problemas(debido a que no se logro implementar todas las
correctas validaciones)

Explicación del código/funcionalidades principales:

-readGuardiansFromFile: Lee los datos de los guardianes desde un archivo CSV y devuelve un vector de objetos Guardian.
-insertGuardian: Inserta un aprendiz en la lista de aprendices de un maestro guardián.
-constructGuardianTree: Construye la jerarquía de guardianes a partir de la lista de guardianes.
-displayGuardianTree: Muestra la jerarquía de guardianes en forma de árbol.
-assingOriginVillage: Asigna una aldea de origen a la estructura de route.
-addNextVillage: Asigna las aldeas del recorrido del jugador en el vector de la estructura route.
-readVillagesFromFile: Lee los datos de las aldeas desde un archivo CSV y devuelve un unordered_map de objetos Village.
-printVillageConnections: Muestra las conexiones entre las aldeas.
-printUniqueVillages: Muestra las aldeas únicas presentes en el sistema(creada en un principio como comprobante del código).
-printVillageSpecific: Muestra las conexiones de una aldea específica(Creada unicamente para verificar si todo iba bien en el código).
-createMyGuardian: Crea un objeto MyGuardian con nombre, nivel de poder y aldea asignados por el usuario.
-printMyGuardian: Muestra los detalles de un objeto MyGuardian.
-displayGuardiansInVillage: Muestra los guardianes presentes en una aldea específica.
-changeVillage: Permite a un aprendiz cambiar de aldea.
-alchemyPortal: Simula un viaje por alquimia con una animación(uso visual).
-alchemy: Permite a un aprendiz viajar a otra aldea por alquimia.
-progressBarAnimation: Muestra una animación de una barra de progreso(uso visual).
-combat: Permite al jugador combatir contra otro guardián, mediante el uso de porcentajes de victoria y aleatoridad.

Link del video:
https://drive.google.com/file/d/1zcby6KL0uaQY_pphc1P0kgwHtjGCpeU5/view?usp=sharing


Comentarios del código:

-No se pudo implementar varias comprobaciones en el programa.
-No se pudo establecer correctamente los guardianes en cada lugar del mapa(es decir 1 maestro y 1 aprendiz por aldea).
-No se identifico maestros y aprendices en las peleas, por lo que si se pelea con alguien no existe una correcta 
asignación del puntaje según maestro o aprendiz, por lo que se decidio otorgar 3 puntos por victoria y 1 puntos por derrota.
-No se desarrollo una función que dictamine una recomendación de con cual guardian luchar ni tampoco la de límite de 
puntos por pelea.

Importante:

-Por más que intente subir como queria los archivos.csv al momento de editarlos aca en el repositorio siempre salen con un espacio en blanco al final,
nose si es por github o por alguna otra cosa, por lo que si le da problema esta linea vacia al final del documento, por favor eliminela al descargarlo.
