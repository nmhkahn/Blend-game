#include <string>
using namespace std;

#include "Util.h"

Vec2 gridToPos( const Vec2& grid )
{
    Size size   = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 center = origin + size / 2;
    
    float margin_width = (size.width - 96*num_grid_width)*0.5;
    float margin_height = (size.height - 96*num_grid_height)*0.5;
    
    return Vec2(96*grid.x+margin_width+48, 96*grid.y+margin_height+48);
}

string int_to_string( int input )
{
    ostringstream convStream;
    convStream << input;
    return convStream.str();
}