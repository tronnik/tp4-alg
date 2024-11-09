#pragma once

#include <raymath.h>

class Plane
{
public:
    Vector3 normal;
    float distance;

    Plane();
    Plane(const Vector3& point, const Vector3& normal);
    bool isOnOrForward( Vector3& point) ;
};

