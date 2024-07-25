/*footway.h*/

//
// A footway in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#pragma once

#include <string>
#include <vector>
#include <iostream>

#include "node.h"
#include "nodes.h"

using namespace std;


//
// Footway
//
// Defines a footway with an ID and the IDs of the nodes that
// define the position / outline of the footway.
//
class Footway
{
public:
  long long ID;
  vector<long long> NodeIDs;

  //
  // constructor
  //
  Footway(long long id);

  //
  // adds the given nodeid to the end of the vector.
  //
  void add(long long nodeid);
};
