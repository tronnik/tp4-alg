#pragma once

#include <raymath.h>



class Plane {
public:
  Plane();

  Plane(const Vector3& point, const Vector3& normal);

  Vector3 GetNormal() const;

  Vector3 GetCenter() const;

  bool IsOnOrForward(const Vector3& Point) const;

private:
  Vector3 f_Normal;
  Vector3 f_Center;
  float f_D;
};
