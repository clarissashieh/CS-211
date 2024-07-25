/*busstops.h*/

//
// A collection of bus stops in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "node.h"
#include "nodes.h"
#include "busstop.h"
#include "dist.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// Keeps track of all the buildings in the map.
//
class BusStops
{
public:
  vector<BusStop> MapBusStops;

  //
  // readMapBusStops
  //
  // Takes in the filename of a CSV file, inputs the data, and 
  // stores all the buildings into the given vector.
  //
  void readMapBusStops(string filename);

  //
  // print
  //
  // prints each bus stop (id, route, name, direction, location, position) 
  // to the console.
  //
  void print();

  //
  // closestStops
  //
  // finds closest northbound and southbound stop to given building
  pair<BusStop, BusStop> closestStops(Nodes& nodes, pair<double, double>& location);

  //
  // accessors / getters
  //
  int getNumMapBusStops();
};


