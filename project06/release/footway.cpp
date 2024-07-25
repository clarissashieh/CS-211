/*footway.cpp*/

//
// A footway in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#include "footway.h"
#include "node.h"
#include "nodes.h"

#include <iostream>
#include <algorithm>

using namespace std;


//
// constructor
//
Footway::Footway(long long id)
  : ID(id)
{
  // vector is default initialized by its constructor
}

//
// adds the given nodeid to the end of the vector.
//
void Footway::add(long long nodeid)
{
  this->NodeIDs.push_back(nodeid);
}