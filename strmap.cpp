// -----------------------------------------------------------------------------
// strmap.cpp
// rawser spicer
// date created 2014/04/10
//
//      this is the source for the strmap class
//
//      the strmap class allows for the conversion betteen text streams(files)
// with the format describe below to be read in to a map that can be accesed
// by the key or the value.
//
//      the streams provided should be in the following fromat
//
//      // <-- this starts a comment line
//      key1::value1
//      key2::value2
//
//
// -----------------------------------------------------------------------------
#include "strmap.h"
#include <string>
using std::string;
#include <sstream>
using std::istringstream;


// -------------------------------------------------------------------------
// stream constructor
//      allows for streams to be mapped in to key value strings
// preconditions:
//      s -- an istringstream that should be in the format decribed in the
//           header of this file
// postconditions:
//      the map owned by this will contain key value pairs of strings
// -------------------------------------------------------------------------
StrMap::StrMap(std::istringstream &iss)
{ 
    string line;
    while(true)
    {
        std::getline(iss, line);
        if(iss.eof())break;
        if(line.substr(0,2) == "//") continue;
        if(line.find("::") == string::npos) continue;
        auto key = (line.substr(0,line.find("::")));
        auto value = line.substr(line.find("::") + 2);
        addItem(key,value);
    }
} // end stream constructor



// -------------------------------------------------------------------------
// addItem
//      this function is to add items to the strmap if the key is not found
// preconditions:
//      key & value -- must be strings
// postconditions:
//      if key is found in the map no item will be added, otherwise it will
//  be. the function will return true if it is added false otherwise
// -------------------------------------------------------------------------
bool StrMap::addItem(const std::string &key, const std::string &value)
{
    if (_theMap.find(key) == _theMap.end())
    {
        _theMap[key] = value;
        return true;
    }
    else return false;
}// end StrMap



// -------------------------------------------------------------------------
// at
//      returns the value at a key
// preconditions:
//      none
// postconditions:
//      a reference the value at key will be returned
// execptions:
//      throws an out of range execption if key is not found
// -------------------------------------------------------------------------
 std::string &StrMap::at(const std::string &key)
{
    if (_theMap.find(key) == _theMap.end())
        throw std::out_of_range("key out of range");
    else
        return _theMap[key];
 }// end at



 // -------------------------------------------------------------------------
 // keyOf
 //      returns the key of a value
 // preconditions:
 //      none
 // postconditions:
 //      the key of value will be returned
 // execptions:
 //      throws an out of range execption if value is not in map
 // -------------------------------------------------------------------------
 std::string StrMap::keyOf(const std::string &value)
 {
     for(auto i = _theMap.begin(); i != _theMap.end(); ++i)
     {
            if((*i).second == value) return i->first;
     }
     throw std::out_of_range("value not found");
}// endkeyOf
