#include "AABB.h"

AABB::AABB( Vector3& center,  Vector3& extents)
    : center(center), extents(extents) {}

bool AABB::isOnOrForwardPlane( Plane& plane) 
{
    float r = extents.x * fabs(plane.normal.x) +
        extents.y * fabs(plane.normal.y) +
        extents.z * fabs(plane.normal.z);

    float d = Vector3DotProduct(plane.normal, center) + plane.distance;

    return d + r >= 0;
}