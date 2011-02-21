//          Copyright John W. Wilkinson 2007 - 2009.
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.03

// This demo shows you how to read and write JSON objects and arrays 
// using header files only, i.e. not linking to the JSON Spirit object library.
// In this demo objects are stored as a vector of name/value pairs.

#include <cassert>
#include <fstream>

#include "json.h"

using namespace std;
using namespace json_spirit;


Value file2json( const char* file_name ) {
    Value value;

    ifstream is( file_name );
    read_stream( is, value );

    return value;
}
