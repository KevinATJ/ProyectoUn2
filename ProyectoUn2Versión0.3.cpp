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
    vector<Guardian*> apprentices;
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
    cout << "Graph of villages:" << endl;

    for (const string& vertex : graph.vertices)
    {
        auto villageIt = find_if(villages.begin(), villages.end(), [&](const Village& village) {
            return village.name == vertex;
        });

        if (villageIt != villages.end())
        {
            const Village& village = *villageIt;
            

            for (const string& connectedVillage : village.connectedVillages)
            {
            	cout << village.name << " -> ";
                cout << connectedVillage << " ";
            }

            cout << endl;
        }
    }
}

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

void constructGuardianTree(vector<Guardian>& guardians)
{
    for (Guardian& guardian : guardians)
    {
        insertGuardian(guardian, guardians);
    }
}

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

int main()
{
    vector<Guardian> guardians = readGuardiansFromFile("Guardians.csv");
    vector<Village> villages = readVillagesFromFile("Villages.csv");

    constructGuardianTree(guardians);

    vector<Guardian*> rootGuardians;

    for (Guardian& guardian : guardians)
    {
        bool isRoot = true;
        for (const Guardian& otherGuardian : guardians)
        {
            if (otherGuardian.apprentices.size() > 0 && &guardian == otherGuardian.apprentices.front())
            {
                isRoot = false;
                break;
            }
        }

        if (isRoot)
        {
            rootGuardians.push_back(&guardian);
        }
    }
    
    if (rootGuardians.empty())
    {
        cout << "No root guardians found." << endl;
    }
    else
    {
        cout << "Guardian Tree:" << endl;
        for (const Guardian* rootGuardian : rootGuardians)
        {
            displayGuardianTree(*rootGuardian);
        }
    }
    cout << endl;
    Graph graph = createGraph(villages);
    buildGraph(graph, villages);
    printGraph(graph, villages);

    return 0;
}
