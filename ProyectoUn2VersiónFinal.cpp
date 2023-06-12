#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

using namespace std;

int GlobalMenu = 0;

struct Guardian
{
    string name;
    int powerLevel;
    string mainMaster;
    string village;
    vector<Guardian*> apprentices;
};

struct Village 
{
    string name;
    vector<string> connectedVillages;
};

struct myGuardian
{
    string name;
    int powerLevel;
    string mainMaster;
    string village;
    string path;
};

struct route
{
	string originVillage;
	vector<string> nextVillages;	
};

/*
La funci�n "readGuardiansFromFile" recibe una referencia constante a un objeto string llamado "filename" que representa el nombre del archivo a leer.
La funci�n crea un vector vac�o llamado "guardians" para almacenar los Guardianes le�dos desde el archivo.
Luego, abre el archivo utilizando un objeto ifstream llamado "file" y verifica si se pudo abrir correctamente.
Si no se puede abrir el archivo, se muestra un mensaje de error y se devuelve el vector vac�o "guardians".
Despu�s, se lee la primera l�nea del archivo (suponiendo que es una l�nea de encabezado) y se descarta.
A continuaci�n, se inicia un bucle mientras se puedan leer l�neas adicionales del archivo.
Dentro del bucle, se crea un objeto Guardian y se declaran las variables "village" y "powerLevelStr".
Se utiliza un objeto stringstream "ss" para separar la l�nea le�da en campos utilizando el delimitador ','.
Los campos separados se asignan a los miembros correspondientes del objeto Guardian, como el nombre, el nivel de poder, el maestro principal y la aldea.
El objeto Guardian se agrega al vector "guardians".
Una vez que se han le�do todas las l�neas del archivo, se cierra el archivo.
Por �ltimo, se devuelve el vector "guardians" que contiene los Guardianes le�dos del archivo.
*/
vector<Guardian> readGuardiansFromFile(const string& filename)
{
    vector<Guardian> guardians;
    ifstream file(filename);

    if (!file)
    {
        cout << "Error al abrir el archivo de guardianes.\n";
        return guardians;
    }

    string line;
    getline(file, line);

    while (getline(file, line))
    {
        Guardian guardian;
        string village;

        stringstream ss(line);
        getline(ss, guardian.name, ',');
        string powerLevelStr;
        getline(ss, powerLevelStr, ',');
        guardian.powerLevel = stoi(powerLevelStr);
        getline(ss, guardian.mainMaster, ',');
        getline(ss, village, ',');
        guardian.village = village;

        guardians.push_back(guardian);
    }

    file.close();
    return guardians;
}

/*
Esta funci�n "insertGuardian" recibe una referencia a un objeto Guardian y un vector de Guardianes.
La funci�n busca un Guardi�n en el vector cuyo nombre coincida con el valor de "guardian.mainMaster".
Si encuentra un Guardi�n con el mismo nombre, agrega el objeto Guardian actual a la lista de aprendices de ese Guardi�n y finaliza la funci�n.
Si no se encuentra ning�n Guardi�n con el mismo nombre, la funci�n simplemente termina sin realizar ninguna acci�n adicional
*/
void insertGuardian(Guardian& guardian, vector<Guardian>& guardians)
{
    for (Guardian& otherGuardian : guardians)
    {
        if (otherGuardian.name == guardian.mainMaster)
        {
            otherGuardian.apprentices.push_back(&guardian);
            return;
        }
    }
}
/*
La funci�n "constructGuardianTree" recibe un vector de Guardianes como par�metro.
Itera a trav�s de cada Guardi�n en el vector utilizando un bucle for-each.
Llama a la funci�n "insertGuardian" pasando el Guardi�n actual y el vector de Guardianes como argumentos.
La funci�n "insertGuardian" se encarga de insertar el Guardi�n actual como aprendiz de otro Guardi�n en el vector, si el nombre del maestro coincide.
*/
void constructGuardianTree(vector<Guardian>& guardians)
{
    for (Guardian& guardian : guardians)
    {
        insertGuardian(guardian, guardians);
    }
}

/*
La funci�n "displayGuardianTree" muestra la informaci�n de un Guardi�n y su estructura de �rbol de aprendices de manera recursiva.
Recibe un objeto Guardian llamado "guardian" como par�metro y un par�metro opcional "depth" que indica la profundidad actual en el �rbol.
Imprime el nombre y nivel de poder del Guardi�n, con un prefijo de espacios proporcionales a la profundidad multiplicada por 4.
Luego, muestra la aldea asociada al Guardi�n, tambi�n con el prefijo de espacios.
Si el Guardi�n tiene aprendices (la lista de aprendices no est� vac�a), muestra la secci�n de "Apprentices" con el mismo prefijo de espacios.
A continuaci�n, itera sobre cada aprendiz en la lista de aprendices del Guardi�n actual y realiza una llamada recursiva a la funci�n "displayGuardianTree" pasando el aprendiz como par�metro y aumentando la profundidad en 1.
Esto permite mostrar la estructura de �rbol completa de los aprendices de forma jer�rquica.
*/
void displayGuardianTree(const Guardian& guardian, int depth = 0)
{
    cout << string(depth * 4, ' ') << "- " << guardian.name << " (Power Level: " << guardian.powerLevel << ")" << endl;
    cout << string(depth * 4, ' ') << "  Associated Village: " << guardian.village << endl;

    if (!guardian.apprentices.empty())
    {
        cout << string(depth * 4, ' ') << "  Apprentices:" << endl;
        for (const Guardian* apprentice : guardian.apprentices)
        {
            displayGuardianTree(*apprentice, depth + 1);  // Llamada recursiva para mostrar a los aprendices
        }
    }
}
/*
La funci�n "assignOriginVillage" recibe una referencia a un objeto "route" y una referencia constante a un objeto string llamado "villageName".
Asigna el valor de "villageName" al miembro "originVillage" del objeto "myRoute".
*/
void assingOriginVillage(route& myRoute, const string& villageName)
{
    myRoute.originVillage = villageName;
}
/*
La funci�n "addNextVillage" recibe una referencia a un objeto "route" y una referencia constante a un objeto string llamado "villageName".
Agrega el valor de "villageName" al final del vector "nextVillages" en el objeto "myRoute".
*/
void addNextVillage(route& myRoute, const string& villageName)
{
    myRoute.nextVillages.push_back(villageName);
}
/*
La funci�n "readVillagesFromFile" lee informaci�n de aldeas desde un archivo y devuelve un unordered_map que mapea el nombre de la aldea a un objeto de tipo "Village".
Crea un unordered_map llamado "villages" para almacenar las aldeas y un unordered_set llamado "uniqueVillages" para mantener un registro de todas las aldeas.
Abre el archivo utilizando un objeto ifstream llamado "file" y verifica si se pudo abrir correctamente.
Si no se puede abrir el archivo, muestra un mensaje de error y devuelve el unordered_map "villages" vac�o.
Lee la primera l�nea del archivo (suponiendo que es una l�nea de encabezado) y la descarta.
Luego, itera sobre cada l�nea restante en el archivo.
Utiliza un objeto stringstream "ss" para separar la l�nea en campos utilizando el delimitador ','.
Lee los campos correspondientes a las aldeas y a la aldea conectada.
Verifica si la aldea ya existe en el unordered_set "uniqueVillages".
Si no existe, la agrega al unordered_set y crea un nuevo objeto "Village" para esa aldea. Luego, agrega la aldea y su objeto "Village" correspondiente al unordered_map "villages".
Si la aldea conectada no est� vac�a, realiza el mismo proceso anterior para esa aldea.
Adem�s, agrega la aldea conectada a la lista de aldeas conectadas de la aldea actual y viceversa.
Una vez que se han procesado todas las l�neas del archivo, se cierra el archivo.
Finalmente, se devuelve el unordered_map "villages" que contiene las aldeas y su informaci�n relacionada.
*/
unordered_map<string, Village> readVillagesFromFile(const string& filename) 
{
    unordered_map<string, Village> villages;
    unordered_set<string> uniqueVillages;  
    ifstream file(filename);

    if (!file) 
    {
        cout << "Error al abrir el archivo de aldeas.\n";
        return villages;
    }

    string line;
    getline(file, line); 

    while (getline(file, line)) 
    {
        stringstream ss(line);
        string village, connectedVillage;

        getline(ss, village, ',');
        getline(ss, connectedVillage, ',');

        if (uniqueVillages.find(village) == uniqueVillages.end()) 
        {
            uniqueVillages.insert(village);  
            Village newVillage;
            newVillage.name = village;
            villages[village] = newVillage;
        }

        if (!connectedVillage.empty())
        {
            if (uniqueVillages.find(connectedVillage) == uniqueVillages.end())
            {
                uniqueVillages.insert(connectedVillage);  
                Village newConnectedVillage;
                newConnectedVillage.name = connectedVillage;
                villages[connectedVillage] = newConnectedVillage;
            }
            villages[village].connectedVillages.push_back(connectedVillage);

            
            villages[connectedVillage].connectedVillages.push_back(village);
        }
    }

    file.close();

    return villages;
}
/*
La funci�n "printVillageConnections" recibe un unordered_map llamado "villages" que contiene informaci�n sobre las aldeas y sus conexiones.
Imprime el encabezado "Village Connections:" para indicar que se mostrar�n las conexiones de las aldeas.
Luego, itera sobre cada par clave-valor en el unordered_map utilizando un bucle for-each.
Para cada aldea, imprime el nombre de la aldea (clave) seguido de dos puntos y un espacio.
A continuaci�n, itera sobre la lista de aldeas conectadas (valor) de la aldea actual utilizando otro bucle for-each.
Imprime el nombre de cada aldea conectada, seguido de un espacio.
Despu�s de imprimir todas las aldeas conectadas para una aldea, se imprime un salto de l�nea para pasar a la siguiente aldea.
*/

void printVillageConnections(const unordered_map<string, Village>& villages) 
{
    cout << "Village Connections:" << endl;
    for (const auto& village : villages) 
	{
        cout << village.first << ": ";
        for (const string& connectedVillage : village.second.connectedVillages) 
		{
            cout << connectedVillage << " ";
        }
        cout << endl;
    }
 
}
/*
La funci�n "printUniqueVillages" recibe un unordered_map llamado "villages" que contiene informaci�n sobre las aldeas y sus conexiones.
Crea un unordered_set llamado "uniqueVillages" para almacenar las aldeas �nicas.
Luego, itera sobre cada par clave-valor en el unordered_map utilizando un bucle for-each.
Para cada aldea, inserta su nombre (clave) en el unordered_set "uniqueVillages".
Adem�s, itera sobre la lista de aldeas conectadas (valor) de la aldea actual y las inserta en el unordered_set "uniqueVillages".
Despu�s de procesar todas las aldeas y sus conexiones, muestra el encabezado "Aldeas �nicas:" para indicar que se mostrar�n las aldeas �nicas.
Luego, itera sobre el unordered_set "uniqueVillages" utilizando un bucle for-each.
Imprime el nombre de cada aldea en una nueva l�nea.
*/
void printUniqueVillages(const unordered_map<string, Village>& villages)
{
    unordered_set<string> uniqueVillages;

    for (const auto& village : villages)
    {
        uniqueVillages.insert(village.first); 
        for (const string& connectedVillage : village.second.connectedVillages)
        {
            uniqueVillages.insert(connectedVillage); 
        }
    }

    cout << "Aldeas unicas: " << endl;
    for (const string& village : uniqueVillages)
    {
        cout << village << endl;
    }
}
/*
La funci�n "printVillageSpecific" recibe un unordered_map llamado "villages" que contiene informaci�n sobre las aldeas y sus conexiones, y una referencia constante a un objeto string llamado "villageName" que representa el nombre de la aldea espec�fica que se desea imprimir.
Utiliza el m�todo "find" del unordered_map para buscar la aldea con el nombre "villageName".
Si se encuentra la aldea, se obtiene una referencia constante al objeto "Village" correspondiente utilizando el iterador devuelto por "find".
Imprime un encabezado indicando que se mostrar�n las conexiones de la aldea especificada, seguido del nombre de la aldea.
Luego, itera sobre la lista de aldeas conectadas de la aldea utilizando un bucle for-each.
Imprime el nombre de cada aldea conectada en una nueva l�nea.
Si no se encuentra la aldea en el unordered_map, se muestra un mensaje indicando que la aldea no fue encontrada.
*/
void printVillageSpecific(const unordered_map<string, Village>& villages, const string& villageName)
{
    auto it = villages.find(villageName);
    if (it != villages.end())
    {
        const Village& village = it->second;
        cout << "Conexiones de la aldea" << villageName << ":" << endl;
        for (const string& connectedVillage : village.connectedVillages)
        {
            cout << connectedVillage << endl;
        }
    }
    else
    {
        cout << "Aldea no encontrada" << endl;
    }
}
/*
Se crea un objeto guardian de tipo myGuardian.
Se solicita al usuario que ingrese el nombre del guardi�n utilizando cin.
Se calcula el valor del atributo powerLevel del guardi�n mediante una f�rmula.
A continuaci�n, se muestra una lista de las aldeas disponibles para elegir. Cada aldea se muestra con un n�mero asignado.
El usuario debe seleccionar el n�mero correspondiente a la aldea que desea asignar al guardi�n.
Si la selecci�n es v�lida (entre 1 y el tama�o de villages), se busca la aldea correspondiente en el unordered_map villages utilizando un iterador y se asignan los valores del nombre de la aldea y el nombre del maestro principal al guardi�n.
Si la selecci�n es inv�lida, se muestra un mensaje indicando que la selecci�n es inv�lida y no se asigna ninguna aldea al guardi�n.
Finalmente, se llama a la funci�n assignOriginVillage para asignar la aldea seleccionada como la aldea de origen en la ruta especificada por myRoute.
Se devuelve el objeto guardian creado.
*/
myGuardian createMyGuardian(const unordered_map<string, Village>& villages, route& myRoute)
{
    myGuardian guardian;
    
    
    cout << "Ingrese el nombre del guardian: ";
    cin >> guardian.name;
    
    guardian.powerLevel = static_cast<int>(0.9 * 100 * 0.5);
    
    cout << "Estas son las aldeas disponibles para escoger:" << endl << endl;
    int num = 1;
    for (const auto& village : villages)
    {
        cout << num << ". " << village.first << endl;
        num++;
    }
    
    int choice;
    cout << "Seleccione el numero de la aldea para ser asignada: " << endl;
    cin >> choice;
    
    if (choice >= 1 && choice <= villages.size())
    {
        auto it = villages.begin();
        advance(it, choice - 1);
        guardian.village = it->first;
        guardian.mainMaster = it->second.name;
    }
    else
    {
        cout << "Seleccion invalida. No se asigno una aldea al guardian." << endl;
    }
    assingOriginVillage(myRoute, guardian.village);
    
   
    return guardian;
}
/*
Imprime el nombre del guardi�n utilizando guardian.name.
Imprime el nivel de poder del guardi�n utilizando guardian.powerLevel.
Imprime el nombre de la aldea asignada al guardi�n utilizando guardian.village.
*/
void printMyGuardian(const myGuardian& guardian)
{
    cout << "Nombre: " << guardian.name << endl;
    cout << "Nivel de Poder: " << guardian.powerLevel << endl;
    cout << "Aldea: " << guardian.village << endl;
}
/*
mprime un encabezado indicando que se mostrar�n los guardianes en la aldea.
Se declara una variable bool foundGuardian y se inicializa como falsa. Esta variable se utiliza para verificar si se encontraron guardianes en la aldea.
Se itera sobre el vector de guardianes utilizando un bucle for-each.
Para cada guardi�n, se verifica si el nombre de la aldea (guardian.village) coincide con el nombre de la aldea proporcionado. Si coinciden, se imprime el nombre del guardi�n y su nivel de poder.
Si se encuentra al menos un guardi�n en la aldea, se actualiza foundGuardian a verdadero.
Despu�s del bucle, se verifica el valor de foundGuardian. Si es falso, se muestra un mensaje indicando que no hay guardianes en la aldea.
*/
void displayGuardiansInVillage(const vector<Guardian>& guardians, const string& village)
{
    cout << "Guardianes en tu aldea:" << endl;
    
    bool foundGuardian = false;
    
    for (const Guardian& guardian : guardians)
    {
        if (guardian.village == village)
        {
            cout << "- " << guardian.name << " (Power Level: " << guardian.powerLevel << ")" << endl;
            foundGuardian = true;
        }
    }
    
    if (!foundGuardian)
    {
        cout << "No hay guardianes en la aldea" << endl;
    }
}
/*
Imprime un encabezado indicando las aldeas cercanas y conectadas a la aldea actual del aprendiz (apprentice.village).
Busca la aldea actual del aprendiz en el mapa de aldeas (villages) utilizando villages.find(apprentice.village).
Si se encuentra la aldea actual en el mapa de aldeas:
Obtiene una referencia constante a la aldea actual mediante const Village& currentVillage = it->second.
Verifica si la aldea actual tiene aldeas conectadas, es decir, si currentVillage.connectedVillages no est� vac�o.
Si hay aldeas conectadas:
Itera sobre las aldeas conectadas utilizando un bucle for y muestra su n�mero y nombre.
Solicita al usuario que ingrese el n�mero de la aldea a la que desea viajar.
Si la elecci�n es v�lida (un n�mero entre 1 y el tama�o de currentVillage.connectedVillages), actualiza la aldea del aprendiz (apprentice.village) con la aldea seleccionada y aumenta su nivel de poder en 1 si es menor a 100.
Si la elecci�n no es v�lida, muestra un mensaje indicando que la elecci�n no es v�lida.
Si no hay aldeas conectadas, muestra un mensaje indicando que no hay aldeas cercanas conectadas.
Si no se encuentra la aldea actual en el mapa de aldeas, muestra un mensaje indicando que la aldea actual no se encontr�.
Agrega la siguiente aldea a myRoute utilizando la funci�n addNextVillage.
Si la aldea del aprendiz es "Tesla" y su nivel de poder es igual o mayor a 90, muestra un mensaje indicando que el aprendiz ha llegado a su destino con los puntos suficientes y se prepara para enfrentar a "Stormheart", y actualiza la variable GlobalMenu a 1.
*/
void changeVillage(myGuardian& apprentice, const unordered_map<string, Village>& villages, route& myRoute)
{

    cout << "Aldeas cercanas y conectadas a " << apprentice.village << ":" << endl;
    
    auto it = villages.find(apprentice.village);

    if (it != villages.end())
    {
        const Village& currentVillage = it->second;

        if (!currentVillage.connectedVillages.empty())
        {
            int num = 1;

            for (const string& connectedVillage : currentVillage.connectedVillages)
            {
                cout << num << ". " << connectedVillage << endl;
                num++;
            }
            
            int choice;
            cout << "Ingresa el numero de la aldea para viajar: ";
            cin >> choice;
            
            if (choice >= 1 && choice <= currentVillage.connectedVillages.size())
            {
                auto it = currentVillage.connectedVillages.begin();
                advance(it, choice - 1);
                apprentice.village = *it;
                if(apprentice.powerLevel < 100)
                {
                	apprentice.powerLevel++;
				}    
            }
            else
            {
                cout << "Su eleccion no es valida" << endl;
            }
        }
        else
        {
            cout << "No hay aldeas cercanas conectadas" << endl;
        }
    }
    else
    {
        cout << "la aldea actual no se encontro" << endl;
    }
    addNextVillage(myRoute, apprentice.village);
    if(apprentice.village == "Tesla" && apprentice.powerLevel >= 90)
    {
    	cout << apprentice.name << " Ha llegado a su destino con los puntos suficientes y se prepara para enfrentar a Stormheart" << endl;
    	GlobalMenu = 1;
    	
	}
    
    
}
/*
La mayor importancia de esta funcion es solamante crear la "animaci�n" de un portal.
*/
void alchemyPortal()
{
    string portal = "O"; 
    int width = 50;
    int duration = 3;
    
    cout << "Empezando viaje por alquimia" << endl;
    
    for (int i = 0; i < width; i++)
    {
        string spaces(i, ' '); 
        string portalLine = spaces + portal; 
        
        cout << portalLine << '\r';
        cout.flush();
        
        usleep(100000); 

        cout << string(portalLine.length(), ' ') << '\r';
        cout.flush();
    }
    
    cout << "Estamos llegado a la mitad del recorrido" << endl;
    
    sleep(duration);
    
    cout << "Ahora vamos directos a su destino" << endl;
    
    for (int i = width; i >= 0; i--)
    {
        string spaces(i, ' '); 
        string portalLine = spaces + portal; 
        
        cout << portalLine << '\r';
        cout.flush();
        
        usleep(100000); 

        cout << string(portalLine.length(), ' ') << '\r';
        cout.flush();
    }
    
    cout << "Hemos llegado al destino" << endl;
}
/*
Se muestra una lista de aldeas disponibles para el viaje por alquimia, enumeradas con n�meros.
Se solicita al usuario que ingrese el n�mero de la aldea a la que desea viajar por alquimia.
Si la selecci�n es v�lida, se encuentra la aldea correspondiente en el mapa de aldeas y se asigna al aprendiz de guardi�n.
Si la selecci�n no es v�lida, se muestra un mensaje indicando que no se pudo hacer el viaje.
Se llama a la funci�n alchemyPortal para simular el viaje a trav�s del portal de alquimia.
Si el aprendiz de guardi�n ha llegado a la aldea "Tesla" y su nivel de poder es mayor o igual a 90, se muestra un mensaje indicando que ha llegado al destino y se prepara para enfrentar a Stormheart. Adem�s, se actualiza la variable GlobalMenu a 1.
Se agrega la aldea de destino al vector de aldeas siguientes en la ruta myRoute.
Se genera un valor aleatorio entre 2 y 4 (inclusive) y se reduce el nivel de poder del aprendiz de guardi�n por ese valor.
*/
void alchemy(myGuardian& apprentice, const unordered_map<string, Village>& villages, route& myRoute)
{
	
	
    cout << "Aldeas disponibles para el viaje por alquimia" << endl << endl;
    int num = 1;
    for (const auto& village : villages)
    {
        cout << num << ". " << village.first << endl;
        num++;
    }

    int choice;
    cout << "Ingresa el numero de aldea a la que deseas viajar por alquimia: ";
    cin >> choice;

    if (choice >= 1 && choice <= villages.size())
    {
        auto it = villages.begin();
        advance(it, choice - 1);
        apprentice.village = it->first;
    }
    else
    {
        cout << "La seleccion no es valida, no pudiste hacer el viaje" << endl;
    }
    alchemyPortal();

    if (apprentice.village == "Tesla" && apprentice.powerLevel >= 90)
    {
        cout << apprentice.name << " Ha llegado a su destino con los puntos suficientes y se prepara para enfrentar a Stormheart" << endl;
        GlobalMenu = 1;
    }
    addNextVillage(myRoute, apprentice.village);

    int reduction = rand() % 3 + 2; 
    apprentice.powerLevel -= reduction;

}
/*
Su funcion principal es mostrar la animaci�n de una barra de progreso
*/
void progressBarAnimation() 
{
    int width = 50; 
    
    for (int i = 0; i <= width; i++) 
	{
        cout << "[";

        for (int j = 0; j < i; j++) 
		{
            cout << "=";
        }

        if (i < width) {
            cout << ">";
        }

        for (int j = i + 1; j < width; j++) 
		{
            cout << " ";
        }
        
        cout << "] " << i * 2 << "%\r"; 
        cout.flush(); 
        
        usleep(100000); 
    }
    
    cout << "[";
    for (int i = 0; i < width; i++) 
	{
        cout << " ";
    }
    cout << "] 100%" << endl;
}
/*
Se llama a la funci�n displayGuardiansInVillage para mostrar la lista de guardianes en la aldea.
Se solicita al jugador que ingrese el nombre del oponente con el que desea luchar.
Si el oponente se llama "Stormheart", se muestra un mensaje indicando que no se puede luchar contra Stormheart en este momento.
Si el oponente tiene otro nombre, se busca en la lista de guardianes para encontrar su informaci�n.
Si el oponente no se encuentra en la lista, se muestra un mensaje indicando que no se encontr� el oponente.
Se calcula la probabilidad de ganar del jugador (playerProbability) como la relaci�n entre el nivel de poder del jugador y la suma del nivel de poder del jugador y del oponente.
Se genera un n�mero aleatorio entre 0 y 1 (exclusivo) utilizando la funci�n rand().
Se muestra la informaci�n del jugador y del oponente, se simula una animaci�n de progreso con la funci�n progressBarAnimation, y se muestra la probabilidad de ganar del jugador.
Si el n�mero aleatorio generado es menor que la probabilidad de ganar del jugador m�s un 10%, se muestra un mensaje indicando que el jugador gan� el combate. Adem�s, si el nivel de poder del aprendiz de guardi�n es menor que 100, se incrementa en 3.
Si el n�mero aleatorio generado es igual o mayor que la probabilidad de ganar del jugador m�s un 10%, se muestra un mensaje indicando que el jugador perdi� el combate. Adem�s, si el nivel de poder del aprendiz de guardi�n es menor que 100, se incrementa en 1.
*/
void combat(const vector<Guardian>& guardians, const string& village, const myGuardian& guardian, myGuardian& apprentice)
{
    displayGuardiansInVillage(guardians, village);
    
    cout << "Ingresa el nombre del oponente con el que luchar: ";
    string opponentName;
    cin.ignore();
    getline(cin, opponentName);
    
	if(opponentName == "Stormheart")
	{
		cout << "No puedes luchar aun contra Stormheart, aun no es tu momento viajero" << endl;
	}
	else
	{
		Guardian opponent;
	    bool foundOpponent = false;
	    for (const Guardian& g : guardians)
	    {
	        if (g.name == opponentName)
	        {
	            opponent = g;
	            foundOpponent = true;
	            break;
	        }
	    }
	
	    if (!foundOpponent)
	    {
	        cout << "No se encontro el oponente" << endl;
	        return;
	    }
	
	    double playerProbability = double(guardian.powerLevel) / (guardian.powerLevel + opponent.powerLevel);
	
	    srand(time(0)); 
	    double roll = static_cast<double>(rand()) / RAND_MAX; 
	    
	    cout << "Jugador [Nivel de Poder]: " << guardian.powerLevel << endl;
	    cout << "Oponente [" << opponent.name << ", Nivel de Poder]: " << opponent.powerLevel << endl;
	    cout << "Calculando probabilidades" << endl;
	    progressBarAnimation();
	    system("pause");
	    cout << "Tus probabilidades de ganar son: " << ((playerProbability* 100 )+ (10))<< "%" << endl;
	    system("pause");
	    
	    if (roll < playerProbability+(10))
	    {
	        cout << "Lo lograste! Ganaste el combate." << endl;
	        if(apprentice.powerLevel < 100)
	        {
	        	apprentice.powerLevel+3;
			}
	        cout << endl;
	    }
	    else
	    {
	        cout << "Oops! Perdiste. Quizas este combate fue demasiado para ti." << endl;
	        if(apprentice.powerLevel < 100)
	        {
	        	apprentice.powerLevel+1;
			}
	        cout << endl;
	    }
		
	}
    
}


int main()
{
	int option = 0;
	
	srand(static_cast<unsigned int>(time(nullptr)));
	
    vector<Guardian> guardians = readGuardiansFromFile("Guardians.csv");
    unordered_map<string, Village> villages = readVillagesFromFile("Villages.csv");
    constructGuardianTree(guardians);
    vector<Guardian*> rootGuardians;
    myGuardian apprentice;
    route myRoute;
    
    
    cout << "\t\tBienvenido a Guardian The Guardian Journey" << endl << endl;
    cout << "Estas son tus opciones viajero" << endl;
    cout << "[1]Crear un aprendiz\n[2]Seleccionar un aprendiz(finalmente no creada, por lo que no esta disponible)" << endl;
	cin >> option;
	
	// Se deja de esta manera el if ya que no se implemento la funcion de seleccionar aprendiz, entonces asi se evitan otros problemas 
	if(option == 1 || option != 1)
	{
		apprentice = createMyGuardian(villages, myRoute);
		cout << endl;
		cout << "Bienvenido a " << apprentice.village << " viajero" << endl;
		cout << "\nEste es el aprendiz que partira a su viaje:" << endl;
		cout << endl;
		printMyGuardian(apprentice);
		cout << endl;
		cout << "Estos son los guardianes en tu aldea " << endl;
		displayGuardiansInVillage(guardians, apprentice.village);
		cout << endl;
	}
	
	cout << "Deberas entrenar muy bien si deseas combatir contra Stormheart" << endl;
    
    while(GlobalMenu == 0)
    {

		cout << "En este momento tienes estas opciones:\n[1]Viajar\n[2]Entrenar en la aldea\n[3]Alquimia\n[4]Revisar estadisticas\n[5]Mostrar camino\n" << endl;
		cin >> option;
		
		if(option == 1)
		{		
			cout << endl;
			changeVillage(apprentice, villages, myRoute);
			cout << endl;
			if(GlobalMenu == 0)
			{
				cout << "Estos son los guardianes en tu aldea actual" << endl << endl;
				displayGuardiansInVillage(guardians, apprentice.village);	
			}
			else
			{
				cout << "Llegaste al final de tu camino";
			}
			
			cout << endl;
		}
		else if(option == 2)
		{
			combat(guardians, apprentice.village, apprentice, apprentice);
			cout << endl;
		}
		else if(option == 3)
		{
			alchemy(apprentice, villages, myRoute);
			if(GlobalMenu != 0)
			{
				cout << "Llegaste al final de tu camino";
			}
			cout << endl;
		}
		else if(option == 4)
		{
			cout << "Estas son tus estadisticas: " << endl << endl;
			printMyGuardian(apprentice);
		}
		else if (option == 5)
		{
		    cout << "Origen del viaje: " << myRoute.originVillage << endl;
			cout << "Lugares visitados: ";
			for (const string& village : myRoute.nextVillages)
			{
			    cout << village << " " << endl;
			}
			cout << endl;
		}	
	}
    return 0;
}
