// -----------------------------------------------------------------------------
// strmap.h
// rawser spicer
// date created 2014/04/10
//
//      this is the header for the strmap class
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
#ifndef STRMAP_H
#define STRMAP_H

#include <string>
using std::string;
#include <map>
using std::map;
#include <sstream>
using std::istringstream;



// -----------------------------------------------------------------------------
// class StrMap
//      this class provieds the interface for loading maped strings from streams
// -----------------------------------------------------------------------------
class StrMap
{
public:

    // -------------------------------------------------------------------------
    // default constructor
    //      does nothing
    // -------------------------------------------------------------------------
    StrMap(){}

    // -------------------------------------------------------------------------
    // stream constructor
    //      allows for streams to be mapped in to key value strings
    // preconditions:
    //      s -- an istringstream that should be in the format decribed in the
    //           header of this file
    // postconditions:
    //      the map owned by this will contain key value pairs of strings
    // -------------------------------------------------------------------------
    StrMap(istringstream &s);

    // -------------------------------------------------------------------------
    // addItem
    //      this function is to add items to the strmap if the key is not found
    // preconditions:
    //      key & value -- must be strings
    // postconditions:
    //      if key is found in the map no item will be added, otherwise it will
    //  be. the function will return true if it is added false otherwise
    // -------------------------------------------------------------------------
    bool addItem(const string & key,const string & value);

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
    std::string &at(const string & key);

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
    std::string keyOf(const string & value);

    // -------------------------------------------------------------------------
    // destructor
    //      does nothing
    // -------------------------------------------------------------------------
    ~StrMap(){}

private:
    // the internal map
    map<string, string> _theMap;

}; // end of class StrMap


#endif // STRMAP_H
