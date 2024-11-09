#include "Plane.h"

Plane::Plane() : normal({ 0.0f, 0.0f, 0.0f }), distance(0.0f) {}

Plane::Plane(const Vector3& point, const Vector3& normal)
	: normal(Vector3Normalize(normal)), distance(Vector3DotProduct(this->normal, point)) {}

bool Plane::isOnOrForward(Vector3& point) 
{
	return Vector3DotProduct(normal, point) + distance >= 0;
}

