#pragma once

#include <raymath.h>

class Plane
{
public:
    Vector3 f_Normal;
    Vector3 f_Center;
    float f_Distance;

    Plane();
    Plane(const Vector3& point, const Vector3& normal);
    Vector3 GetNormal() const;
    Vector3 GetCenter() const;
    bool isOnOrForward( Vector3& point) ;
};

