/*busstops.cpp*/

//
// A collection of bus stops in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>

#include "busstops.h"
#include "nodes.h"
#include "osm.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// readMapBusStops
//
// Takes in the filename of a CSV file, inputs the data, and 
// stores all the buildings into the given vector.
void BusStops::readMapBusStops(string filename)
{
  ifstream inputFile(filename);

  if(!inputFile) return;

  string line;
  while(getline(inputFile, line)) {
    stringstream parser(line);

    string id, route, name, direction, location, lat, lon;

    getline(parser, id, ',');
    getline(parser, route, ',');
    getline(parser, name, ',');
    getline(parser, direction, ',');
    getline(parser, location, ',');
    getline(parser, lat, ',');
    getline(parser, lon);

    MapBusStops.emplace_back(BusStop(stoll(id), stoi(route), name, direction, 
        location, stod(lat), stod(lon)));
  }
}


//
// comparator
//
// custom comparator to sort MapBusStops by stop ID
//
bool comparator1(BusStop a, BusStop b) {
  return a.ID < b.ID;
}

//
// print
//
// prints each bus stop (id, route, name, direction, location, position) 
// to the console.
//
void BusStops::print() {
  sort(MapBusStops.begin(), MapBusStops.end(), comparator1);
  for (const BusStop& B : this->MapBusStops) {
    cout << B.ID << ": bus " << B.Route << ", " << B.Name << ", " 
        << B.Direction << ", " << B.Location << ", location (" <<
            B.Lat << ", " << B.Lon << ")" << endl;
  }
}


//
// comparator
//
// custom comparator to sort bus stop and distance pairs in closestStops()
//
bool comparator2 (pair<BusStop, double> a, pair<BusStop, double> b) {
    return a.second < b.second;
}

//
// closestStops
//
// finds closest northbound and southbound stop to given building
//
pair<BusStop, BusStop> BusStops::closestStops(Nodes& nodes, pair<double, double>& location) {
    vector<pair<BusStop, double>> northDists;
    vector<pair<BusStop, double>> southDists;

    // for each bus stop, calculate distance between building and bus stop,
    // add to vector of distances based on direction along with bus stop
    for (BusStop B : this->MapBusStops) { 
        double dist = distBetween2Points(location.first, location.second, B.Lat, B.Lon);
        if (B.Direction.compare("Northbound") == 0) {
            northDists.emplace_back(make_pair(B, dist));
        }
        else if (B.Direction.compare("Southbound") == 0) {
            southDists.emplace_back(make_pair(B, dist));
        }
    }

    // sort both vectors
    sort(northDists.begin(), northDists.end(), comparator2);
    sort(southDists.begin(), southDists.end(), comparator2);

    // return a pair of the bus stops
    return make_pair(northDists.at(0).first, southDists.at(0).first);
}

//
// accessors / getters
//
int BusStops::getNumMapBusStops() {
  return (int) this->MapBusStops.size();
}
