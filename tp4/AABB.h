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

//#pragma once
//
//#include <raylib-cpp.hpp>
//#include <vector>
//
//class AABB {
//public:
//    raylib::Vector3 min;  // Punto mínimo del AABB
//    raylib::Vector3 max;  // Punto máximo del AABB
//
//    // Constructores
//    AABB();  // Constructor vacío
//    AABB(const std::vector<raylib::Vector3>& vertices);  // Constructor con vértices
//
//    // Métodos
//    void ExpandToFit(const raylib::Vector3& point); // Expande el AABB para incluir un punto
//    raylib::Vector3 GetCenter() const;              // Calcula el centro del AABB
//    raylib::Vector3 GetExtents() const;             // Calcula las extensiones del AABB
//    bool Contains(const raylib::Vector3& point) const; // Verifica si un punto está dentro del AABB
//};