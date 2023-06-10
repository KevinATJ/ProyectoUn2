#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Guardian 
{
    string name;
    int powerLevel;
    string mainMaster;
    string village;
};

struct Village 
{
    string name;
    vector<string> connectedVillages;
};

struct Graph 
{
    vector<string> vertices;
    vector<vector<string>> adjacencyList;
};

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
        cout << "Guardian: [" << guardian.name << "] powerLevel: [" << guardian.powerLevel << "] mainMaster: [" << guardian.mainMaster << "] village: [" << guardian.village << "]" << endl;
    }

    file.close();
    return guardians;
}

vector<Village> readVillagesFromFile(const string& filename) 
{
    vector<Village> villages;
    ifstream file(filename);

    if (!file) 
    {
        cout << "Error al abrir el archivo de aldeas.\n";
        return villages;
    }

    string line;
    getline(file, line); // Leer y descartar la primera línea de encabezados

    while (getline(file, line)) 
    {
        Village village;
        stringstream ss(line);
        getline(ss, village.name, ',');

        string connectedVillage;
        while (getline(ss, connectedVillage, ','))
        {
            village.connectedVillages.push_back(connectedVillage);
        }
        
        villages.push_back(village);
    }

    file.close();
    return villages;
}


Graph createGraph(const vector<Village>& villages) 
{
    Graph graph;
    graph.vertices.reserve(villages.size());
    graph.adjacencyList = vector<vector<string>>(villages.size());

    for (const Village& village : villages)
    {
        graph.vertices.push_back(village.name);
    }

    return graph;
}

void addEdge(Graph& graph, const string& source, const string& destination) 
{
    auto sourceIter = find(graph.vertices.begin(), graph.vertices.end(), source);
    auto destinationIter = find(graph.vertices.begin(), graph.vertices.end(), destination);

    if (sourceIter != graph.vertices.end() && destinationIter != graph.vertices.end())
    {
        int sourceIndex = distance(graph.vertices.begin(), sourceIter);
        int destinationIndex = distance(graph.vertices.begin(), destinationIter);

        graph.adjacencyList[sourceIndex].push_back(destination);
        graph.adjacencyList[destinationIndex].push_back(source);
    }
}

void buildGraph(Graph& graph, const vector<Village>& villages)
{
    for (const Village& village : villages)
    {
        for (const string& connectedVillage : village.connectedVillages)
        {
            addEdge(graph, village.name, connectedVillage);
        }
    }
}

void printGraph(const Graph& graph, const vector<Village>& villages)
{
    for (const Village& village : villages)
    {
        
        
        if (!village.connectedVillages.empty()) 
		{
			cout << "Village " << village.name << ": ";
            for (const string& connected : village.connectedVillages)
            {
                cout << connected << " ";
            }
        } 
        /*Comprobación para verificar las correctas conexiones
		else 
		{
            cout << "No connected villages";
        }
        */
        cout << endl;
    }
}

int main()
{
    vector<Guardian> guardians = readGuardiansFromFile("Guardians.csv");
    cout << endl;
    vector<Village> villages = readVillagesFromFile("Villages.csv");

    Graph graph = createGraph(villages);
    buildGraph(graph, villages);

    cout << endl;
    cout << endl;
    printGraph(graph, villages);

    return 0;
}



