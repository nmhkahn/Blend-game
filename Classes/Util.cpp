#include "Util.h"

Vec2 CRotate( Vec2 target, float dtheta )
{
    return Vec2(target.x*cosf(dtheta) - target.y*sinf(dtheta),
                target.x*sinf(dtheta) + target.y*cosf(dtheta));
}

Vec2 CRotateOnAnchor( Vec2 target, float dtheta, Vec2 anchor )
{
    const Vec2 pos = target - anchor;
    return CRotate(pos, dtheta) - pos;
}