/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include <iostream>

#include "building.h"
#include "busstops.h"

using namespace std;


//
// constructor
//
Building::Building(long long id, string name, string streetAddr)
  : ID(id), Name(name), StreetAddress(streetAddr)
{
  //
  // the proper technique is to use member initialization list above,
  // in the same order as the data members are declared:
  //
  //this->ID = id;
  //this->Name = name;
  //this->StreetAddress = streetAddr;

  // vector is default initialized by its constructor
}

//
// print
// 
// prints information about a building --- id, name, etc. -- to
// the console. The function is passed the Nodes for searching 
// purposes.
//
void Building::print(Nodes& nodes, BusStops& busstops, CURL* curl)
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;
  cout << "# perimeter nodes: " << this->NodeIDs.size() << endl;

  pair<double, double> location = this->getLocation(nodes);
  cout << "Location: (" << location.first << ", " << location.second << ")" << endl;

  pair<BusStop, BusStop> closestStops = busstops.closestStops(nodes, location);
  closestStops.second.printBusPredictions(location, curl);
  closestStops.first.printBusPredictions(location, curl);
  // cout << "Nodes:" << endl;
  // for (long long nodeid : this->NodeIDs)
  // {
  //   cout << "  " << nodeid << ": ";

  //   double lat = 0.0;
  //   double lon = 0.0;
  //   bool entrance = false;

  //   bool found = nodes.find(nodeid, lat, lon, entrance);

  //   if (found) {
  //     cout << "(" << lat << ", " << lon << ")";

  //     if (entrance)
  //       cout << ", is entrance";

  //     cout << endl;
  //   }
  //   else {
  //     cout << "**NOT FOUND**" << endl;
  //   }
  // }//for
}

//
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

//
// gets the center (lat, lon) of the building based
// on the nodes that form the perimeter
//
pair<double, double> Building::getLocation(const Nodes& nodes) 
{
  double avgLat = 0;
  double avgLon = 0;

  for(long long nodeid : this->NodeIDs) {
    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    bool found = nodes.find(nodeid, lat, lon, entrance);

    if(found) {
      avgLat += lat;
      avgLon += lon;
    }
  }

  avgLat /= this->NodeIDs.size();
  avgLon /= this->NodeIDs.size();

  return make_pair(avgLat, avgLon);
}