/*buildings.cpp*/
//
// A collection of buildings in the Open Street Map.
// 
// Clarissa Shieh
// Northwestern University
// CS 211
// 

#include "buildings.h"
#include "osm.h"

#include <iostream>
#include <cassert>

void Buildings::readMapBuildings(XMLDocument& xmldoc)
{
    XMLElement* osm = xmldoc.FirstChildElement("osm");
    XMLElement* way = osm->FirstChildElement("way");
    while(way != 0){ // traverse XML document
        if(osmContainsKeyValue(way, "building","university")) {
            long long id = way->FindAttribute("id")->Int64Value();
            string name = osmGetKeyValue(way, "name");
            string streetAddr = osmGetKeyValue(way, "addr:housenumber")
                + " "
                + osmGetKeyValue(way, "addr:street");
            
            XMLElement* nd = way->FirstChildElement("nd");

            Building B = Building(id, name, streetAddr);
            while(nd != nullptr) // loop through nd references
            {
                const XMLAttribute* ndref = nd->FindAttribute("ref");
                assert(ndref !=nullptr);
                long long id = ndref->Int64Value();

                B.add(id); // add each node reference to NodeIDs

                // advance to next node ref:
                nd = nd->NextSiblingElement("nd");
            }
            this->MapBuildings.push_back(B);
        }
        way = way->NextSiblingElement("way");
    }
}

int Buildings::getNumMapBuildings() const
{
    return (int)this->MapBuildings.size();
}