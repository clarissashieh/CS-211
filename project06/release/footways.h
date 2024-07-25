/*footways.h*/

//
// A collection of footways in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#pragma once

#include <vector>
#include <iostream>

#include "footway.h"
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;


//
// Keeps track of all the footways in the map.
//
class Footways
{
public:
  vector<Footway> MapFootways;

  //
  // readMapFootways
  //
  // Given an XML document, reads through the document and 
  // stores all the footways into the given vector.
  //
  void readMapFootways(XMLDocument& xmldoc);

  //
  // accessors / getters
  //
  int getNumMapFootways();

};


