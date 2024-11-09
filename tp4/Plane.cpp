#include "Plane.h"



Plane::Plane()
  : f_Normal({0.0f, 0.0f, 0.0f}), f_Center({0,0,0}), f_Distance(0.0f) {
}



Plane::Plane(const Vector3& point, const Vector3& normal)
  : f_Normal(Vector3Normalize(normal)),
    f_Center(point),
    f_Distance(Vector3DotProduct(this->f_Normal, point)) {
}



Vector3 Plane::GetNormal() const {
  return this->f_Normal;
}



Vector3 Plane::GetCenter() const {
  return this->f_Center;
}



bool Plane::isOnOrForward(Vector3& point) {
  return Vector3DotProduct(f_Normal, point) + f_Distance >= 0;
}
