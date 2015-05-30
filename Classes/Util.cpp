#include <string>
using namespace std;

#include "Util.h"

Vec2 gridToPos( const Vec2& grid )
{
    return Vec2(96*grid.x+150, 96*grid.y+85);
}

string int_to_string( int input )
{
    ostringstream convStream;
    convStream << input;
    return convStream.str ();
}