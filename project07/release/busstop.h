/*busstop.h*/

//
// A bus stop in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#pragma once

#include <string>
#include <vector>
#include <utility>

#include "node.h"
#include "nodes.h"
#include "curl_util.h"

using namespace std;


//
// BusStop
//
// Defines a bus stop with a stop ID, a bus route (e.g. "201"),
// a stop name (e.g. "Central & Sheridan"), the direction of travel
// (e.g. "Eastbound"), the location of the stop (e.g. "SW corner"), 
// and the position in latitude and longitude.
// 
//
class BusStop
{
public:
  long long ID;
  int Route;
  string Name;
  string Direction;
  string Location;
  double Lat;
  double Lon;
  vector<long long> NodeIDs;

  //
  // constructor
  //
  BusStop(long long id, int route, string name, string direction, string location, double lat, double lon);

  //
  // printBusPredictions
  //
  // prints closest bus stops and estimated arrival times for given bus stop
  //
  void printBusPredictions(pair<double, double> location, CURL* curl);
};
