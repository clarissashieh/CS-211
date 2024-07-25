/*main.cpp*/
//
// Main function to prompt and read contents of XML document. Prints building information.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#include <iostream>
#include <string>

#include "building.h"
#include "node.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"
#include "buildings.h"

using namespace std;
using namespace tinyxml2;

int main()
{
    string filename;
    cout << "** NU open street map **\n";

    cout << "\nEnter map filename>\n";
    getline(cin, filename);
    
    // load given file
    XMLDocument doc;
    bool success = osmLoadMapFile(filename, doc);
    if(!success)
    {
        cout << "Error loading map file\n";
        return 0;
    }

    // determine # of nodes in map
    Nodes nodes;
    nodes.readMapNodes(doc);
    int numNodes = nodes.getNumMapNodes();
    cout << "# of nodes: " << numNodes << endl;

    // determine # of buildings in map
    Buildings buildings;
    buildings.readMapBuildings(doc);
    int numBuildings = buildings.getNumMapBuildings();
    cout << "# of buildings: " << numBuildings << endl;
    
    string command;
    cout << "\nEnter building (partial or complete), or * to list, or $ to end>\n";
    getline(cin, command);
    while (command != "$"){ // while the user hasn't stopped the program
        if (command == "*"){ 
            for (Building b : buildings.MapBuildings) // list each building
                cout << b.ID << ": " << b.Name << ", " << b.StreetAddress << endl;
        }
        else {
            for (Building b : buildings.MapBuildings){ // loop through buildings
                if (b.Name.find(command) != string::npos){ // look for building names that contain inputted name
                    cout << b.Name << "\nAddress: " << b.StreetAddress << "\nBuilding ID: " << b.ID << endl;
                    cout << "Nodes:\n";
                    for (long long ID : b.NodeIDs){ // loop through each ID
                        double lat;
                        double lon;
                        bool isEntrance;

                        if (nodes.find(ID, lat, lon, isEntrance)) { // if ID is found, print info
                            cout << ID << ": (" << lat << ", " << lon << ")";
                            if (isEntrance)
                                cout << ", is entrance\n";
                            else
                                cout << "\n";
                        }
                        else 
                            cout << ID << ": ** NOT FOUND **\n";
                    }
                    cout << "\n";
                }
            }
        }
        command = "";
        getline(cin, command);
    }
    
    cout << "\n** Done **\n";
    cout << "# of calls to getID(): " << Node::getCallsToGetID() << endl;
    cout << "# of Nodes created: " << Node::getCreated() << endl;
    cout << "# of Nodes copied: " << Node::getCopied() << endl;
    return 0;
}