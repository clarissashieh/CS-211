/*building.cpp*/

//
// A building in the Open Street Map.
// 
// Clarissa Shieh
// Based on solution by Prof. Joe Hummel
// Northwestern University
// CS 211
// 

#include "building.h"
#include "node.h"
#include "nodes.h"
#include "footway.h"
#include "footways.h"

#include <iostream>
#include <algorithm>

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
// adds the given nodeid to the end of the vector.
//
void Building::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}

//
// print
//
// Prints building information.
//
void Building::print(const Nodes& nodes) const
{
  cout << this->Name << endl;
  cout << "Address: " << this->StreetAddress << endl;
  cout << "Building ID: " << this->ID << endl;

  cout << "Nodes: " << this->NodeIDs.size() << endl;
  for (long long nodeid : this->NodeIDs)
  {
    cout << " " << nodeid << ": ";

    double lat = 0.0;
    double lon = 0.0;
    bool entrance = false;

    bool found = nodes.find(nodeid, lat, lon, entrance);

    if (found) {
      cout << "(" << lat << ", " << lon << ")";

      if (entrance)
        cout << ", is entrance";

      cout << endl;
    }
    else {
      cout << "**NOT FOUND**" << endl;
    }
  }//for
}


// 
// findIntersections
//
// Finds all intersecting footways with building.
//
void Building::findIntersections(const Footways& footways) const {
  vector<long long> intersections;
  for (long long id : this->NodeIDs) { // for each building node ID, look at every footway's nodes
    compareNodes(id, footways, intersections);
  }

  // sort intersections vector & determine size
  size_t size = intersections.size();
  sort(intersections.begin(), intersections.end());

  // remove duplicates
  long long curr = -1;
  for (size_t i = 0; i < intersections.size(); i++) { 
    if (curr == intersections.at(i)) {
      intersections.at(i) = -1;
      size--;
    }
    curr = intersections.at(i);
  }

  // print intersections
  cout << "Footways that intersect: " << size << endl;
  for (long long id : intersections) {
    if (id != -1) cout << " Footway " << id << endl;
  }

  if (size == 0) {
    cout << " None\n";
  }
}

//
// compareNodes
//
// Compares given building node ID with IDs of all footways.
//
void Building::compareNodes(long long id, const Footways& footways, vector<long long>& intersections) const {
  for (Footway f : footways.MapFootways){ // look through each footway
    sort(f.NodeIDs.begin(), f.NodeIDs.end());

    if(binary_search(f.NodeIDs.begin(), f.NodeIDs.end(), id)) { // search for id
      intersections.emplace_back(f.ID);
    }
  }
}