#include "AABB.h"

AABB::AABB( Vector3& center,  Vector3& extents)
    : center(center), extents(extents) {}

bool AABB::isOnOrForwardPlane( Plane& plane) 
{
    float r = extents.x * fabs(plane.f_Normal.x) +
        extents.y * fabs(plane.f_Normal.y) +
        extents.z * fabs(plane.f_Normal.z);

    float d = Vector3DotProduct(plane.f_Normal, center) + plane.f_Distance;

    return d + r >= 0;
}