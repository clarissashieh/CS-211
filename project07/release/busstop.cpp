/*busstop.cpp*/

//
// A bus stop in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#include <iostream>

#include "busstop.h"
#include "building.h"
#include "curl_util.h"
#include "json.hpp"

using json = nlohmann::json;
using namespace std;


//
// constructor
//
BusStop::BusStop(long long id, int route, string name, string direction, string location, double lat, double lon)
  : ID(id), Route(route), Name(name), Direction(direction), Location(location), Lat(lat), Lon(lon)
{
    
}

//
// printBusPredictions
//
// prints closest bus stops and estimated arrival times for given bus stop
//
void BusStop::printBusPredictions(pair<double, double> location, CURL* curl) {
  // print closest bus stop
  string direction;
  if (this->Direction.compare("Northbound") == 0) direction = "northbound";
  else direction = "southbound";
  
  cout << "Closest " << direction << " bus stop:" << endl;
  cout << "  " << this->ID << ": " << this->Name << ", bus #" 
    << this->Route << ", " << this->Location << ", " << 
      distBetween2Points(this->Lat, this->Lon, location.first, location.second) 
        << " miles" << endl;

  // create url with route and id
  string url = "http://ctabustracker.com/bustime/api/v2/getpredictions?key=NMvHZcsTyYQSgg29Yn5rPkftW&rt=" 
    + to_string(this->Route) + "&stpid=" + to_string(this->ID) + "&format=json";

  // extract predictions
  string response;
  if (!callWebServer(curl, url, response)) {
    cout << "  <<bus predictions unavailable, call failed>>" << endl;
  }
  else {
    auto jsondata = json::parse(response);
    auto bus_response = jsondata ["bustime-response"];
    auto predictions = bus_response["prd"];
    
    if (predictions.size() == 0) {
      cout << "  <<no predictions available>>" << endl;
      return;
    }

    // for each prediction (a map) in the list:
    for (auto& M : predictions) {
      try {
        cout <<  "  vehicle #" << stoi(M["vid"].get_ref<std::string&>()) 
          << " on route " << stoi(M["rt"].get_ref<std::string&>()) 
            << " travelling " << M["rtdir"].get_ref<std::string&>() 
              << " to arrive in " << stoi(M["prdctdn"].get_ref<std::string&>()) 
                << " mins" << endl;
      }
      catch (exception& e) {
        cout << " error" << endl;
        cout << " malformed CTA response, prediction unavailable"
        << " (error: " << e.what() << ")" << endl;
      }
    }
 }
}
