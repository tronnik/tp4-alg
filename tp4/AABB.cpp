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

//#include "AABB.h"
//#include <limits>
//
//// Constructor vacío: inicializa los valores máximos y mínimos posibles para min y max
//AABB::AABB()
//    : min(raylib::Vector3(std::numeric_limits<float>::max())),
//    max(raylib::Vector3(std::numeric_limits<float>::lowest())) {}
//
//// Constructor que recibe un conjunto de vértices y define el AABB
//AABB::AABB(const std::vector<raylib::Vector3>& vertices) {
//    for (const auto& vertex : vertices) {
//        ExpandToFit(vertex);
//    }
//}
//
//// Expande el AABB para incluir un nuevo vértice
//void AABB::ExpandToFit(const raylib::Vector3& point) {
//    if (point.x < min.x) min.x = point.x;
//    if (point.y < min.y) min.y = point.y;
//    if (point.z < min.z) min.z = point.z;
//
//    if (point.x > max.x) max.x = point.x;
//    if (point.y > max.y) max.y = point.y;
//    if (point.z > max.z) max.z = point.z;
//}
//
//// Calcula el centro del AABB
//raylib::Vector3 AABB::GetCenter() const {
//    return (min + max) * 0.5f;
//}
//
//// Calcula las extensiones del AABB
//raylib::Vector3 AABB::GetExtents() const {
//    return (max - min) * 0.5f;
//}
//
//// Verifica si un punto está dentro o en los bordes del AABB
//bool AABB::Contains(const raylib::Vector3& point) const {
//    return (point.x >= min.x && point.x <= max.x) &&
//        (point.y >= min.y && point.y <= max.y) &&
//        (point.z >= min.z && point.z <= max.z);
//}