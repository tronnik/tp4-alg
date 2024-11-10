#include "Frustum.h"

#include "Plane.h"
#include "AABB.h"

float Frustum::f_CameraH = 1; // check actual value

Frustum::Frustum(float FOD, const raylib::Camera& Camera)
  : f_FOD(FOD), f_FOV(Camera.GetFovy()), f_Aspect(1.69F) {

  Update(Camera);
}



void Frustum::Update(raylib::Camera Camera) {

  Matrix Matrix = Camera.GetMatrix();

  this->f_Origin = Camera.GetPosition();

  this->f_Up = raylib::Vector3(Matrix.m1, Matrix.m5, Matrix.m9);

  this->f_Front = raylib::Vector3(Matrix.m2, Matrix.m6, Matrix.m10);

  this->f_Right = raylib::Vector3(Matrix.m0, Matrix.m4, Matrix.m8);

  this->f_Mesh = MakeMesh();
}



void Frustum::UpdatePlanes() {

  Vector3 Normal;
  Vector3 Center;

  const float NearHeight = f_CameraH;
  const float NearWidth = f_Aspect * NearHeight;

  const Vector3 NearCenter = f_Origin + (-f_Front * this->f_NearDistance);
  const Vector3 FarCenter =
      f_Origin + (-f_Front * (this->f_NearDistance + f_FOD));

  // Plano cercano
  f_Planes[0] = Plane(NearCenter, -f_Front); // Normal hacia adelante

  // Plano lejano
  f_Planes[1] = Plane(FarCenter, f_Front); // Normal hacia atrás

  // Plano izquierdo
  Normal = Vector3Normalize(Vector3CrossProduct(f_Mesh[4] - f_Mesh[5],
                                                f_Mesh[4] - f_Mesh[7]));
  Center = {
      (f_Mesh[6].x + f_Mesh[4].x) / 2,
      (f_Mesh[6].y + f_Mesh[4].y) / 2,
      (f_Mesh[6].z + f_Mesh[4].z) / 2};

  f_Planes[2] = Plane(Center, Normal);

  // Plano derecho
  Normal = Vector3Normalize(Vector3CrossProduct(f_Mesh[2] - f_Mesh[3],
                                                f_Mesh[1] - f_Mesh[3]));
  Center = {
      (f_Mesh[0].x + f_Mesh[2].x) / 2,
      (f_Mesh[0].y + f_Mesh[2].y) / 2,
      (f_Mesh[0].z + f_Mesh[2].z) / 2};

  f_Planes[3] = Plane(Center, Normal);

  // Plano superior
  Normal = Vector3Normalize(Vector3CrossProduct(f_Mesh[3] - f_Mesh[2],
                                                f_Mesh[6] - f_Mesh[2]));
  Center = {
      (f_Mesh[3].x + f_Mesh[6].x) / 2,
      (f_Mesh[3].y + f_Mesh[6].y) / 2,
      (f_Mesh[3].z + f_Mesh[6].z) / 2};
  f_Planes[4] = Plane(Center, Normal);

  // Plano inferior
  Normal = Vector3Normalize(Vector3CrossProduct(f_Mesh[0] - f_Mesh[4],
                                                f_Mesh[5] - f_Mesh[4]));

  Center = {
      (f_Mesh[0].x + f_Mesh[5].x) / 2,
      (f_Mesh[0].y + f_Mesh[5].y) / 2,
      (f_Mesh[0].z + f_Mesh[5].z) / 2};
  f_Planes[5] = Plane(Center, Normal);
}



std::array<raylib::Vector3, 8> Frustum::GetMesh() const {
  return this->f_Mesh;
}



bool Frustum::IsObjectVisible(std::array<raylib::Vector3, 8> ObjectMesh) const {
  std::array<bool, 6> InPlane;
  for (const auto& Point : ObjectMesh) {
    for (int I = 0; I < f_Planes.size(); I++) {
      if (f_Planes.at(I).IsOnOrForward(Point)) {
        InPlane.at(I) = true;
      }
      else {
        InPlane.at(I) = false;
      }
    }
    if(InPlane.at(0) && InPlane.at(1) && InPlane.at(2) && InPlane.at(3) &&
         InPlane.at(4) && InPlane.at(5)) {
      return true;
    }
  }
  return false;
}



std::array<raylib::Vector3, 8> Frustum::MakeMesh() {
  std::array<raylib::Vector3, 8> Mesh;

  const float CameraW = this->f_Aspect * f_CameraH;
  this->f_NearDistance = (f_CameraH / 2) / tan(this->f_FOV / 2);
  this->f_BackHeight = 2 * ((this->f_FOD + this->f_NearDistance) * tan(
                                this->f_FOV / 2));
  this->f_BackWidth = this->f_BackHeight * this->f_Aspect;

  Mesh[0] = this->f_Origin.Add(this->f_Right.Scale(CameraW / 2)).Add(
      this->f_Front.Scale(-this->f_NearDistance)).Add(
      f_Up.Scale(-f_CameraH / 2));
  Mesh[1] = this->f_Origin.Add(this->f_Right.Scale(this->f_BackWidth / 2)).
                  Add(this->f_Front.Scale(-this->f_FOD)).Add(
                      f_Up.Scale(-this->f_BackHeight / 2));
  Mesh[2] = this->f_Origin.Add(this->f_Right.Scale(this->f_BackWidth / 2)).
                  Add(this->f_Front.Scale(-this->f_FOD)).Add(
                      f_Up.Scale(this->f_BackHeight / 2));
  Mesh[3] = this->f_Origin.Add(this->f_Right.Scale(CameraW / 2)).Add(
                      this->f_Front.Scale(-this->f_NearDistance)).
                  Add(f_Up.Scale(f_CameraH / 2));
  Mesh[4] = this->f_Origin.Add(this->f_Right.Scale(-CameraW / 2)).Add(
      this->f_Front.Scale(-this->f_NearDistance)).Add(
      f_Up.Scale(-f_CameraH / 2));
  Mesh[5] = this->f_Origin.Add(this->f_Right.Scale(-this->f_BackWidth / 2)).
                  Add(this->f_Front.Scale(-this->f_FOD)).Add(
                      f_Up.Scale(-this->f_BackHeight / 2));
  Mesh[6] = this->f_Origin.Add(this->f_Right.Scale(-this->f_BackWidth / 2)).
                  Add(this->f_Front.Scale(-this->f_FOD)).Add(
                      f_Up.Scale(this->f_BackHeight / 2));
  Mesh[7] = this->f_Origin.Add(this->f_Right.Scale(-CameraW / 2)).Add(
                      this->f_Front.Scale(-this->f_NearDistance)).
                  Add(f_Up.Scale(f_CameraH / 2));

  return Mesh;
}
