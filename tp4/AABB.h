#pragma once
#include <raymath.h>
#include "Plane.h"

class AABB
{
public:
    Vector3 center;
    Vector3 extents;

    AABB( Vector3& center,  Vector3& extents);
    bool isOnOrForwardPlane( Plane& plane) ;
};

