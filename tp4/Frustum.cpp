#include "Frustum.h"

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



void Frustum::Move(const raylib::Vector3 NewPosition) {
  this->f_Origin = NewPosition;
  this->f_Mesh = MakeMesh();
}



std::array<raylib::Vector3, 8> Frustum::GetMesh() const {
  return this->f_Mesh;
}



void Frustum::DrawWireframe() const {
  //rightside
  this->f_Mesh[0].DrawLine3D(this->f_Mesh[1], raylib::Color::White());
  this->f_Mesh[1].DrawLine3D(this->f_Mesh[2], raylib::Color::White());
  this->f_Mesh[2].DrawLine3D(this->f_Mesh[3], raylib::Color::White());
  this->f_Mesh[3].DrawLine3D(this->f_Mesh[0], raylib::Color::White());

  //leftside
  this->f_Mesh[4].DrawLine3D(this->f_Mesh[5], raylib::Color::White());
  this->f_Mesh[5].DrawLine3D(this->f_Mesh[6], raylib::Color::White());
  this->f_Mesh[6].DrawLine3D(this->f_Mesh[7], raylib::Color::White());
  this->f_Mesh[7].DrawLine3D(this->f_Mesh[4], raylib::Color::White());

  //Front
  //this->f_Mesh[0].DrawLine3D(this->f_Mesh[3], raylib::Color::White());
  this->f_Mesh[3].DrawLine3D(this->f_Mesh[7], raylib::Color::White());
  //this->f_Mesh[7].DrawLine3D(this->f_Mesh[4], raylib::Color::White());
  this->f_Mesh[4].DrawLine3D(this->f_Mesh[0], raylib::Color::White());

  //back
  //this->f_Mesh[1].DrawLine3D(this->f_Mesh[2], raylib::Color::White());
  this->f_Mesh[2].DrawLine3D(this->f_Mesh[6], raylib::Color::White());
  //this->f_Mesh[6].DrawLine3D(this->f_Mesh[5], raylib::Color::White());
  this->f_Mesh[5].DrawLine3D(this->f_Mesh[1], raylib::Color::White());

  // this->f_Mesh[0].DrawSphere(3, RED);
  // this->f_Mesh[1].DrawSphere(3, RED);
  // this->f_Mesh[2].DrawSphere(3, RED);
  // this->f_Mesh[3].DrawSphere(3, RED);
  // this->f_Mesh[4].DrawSphere(3, RED);
  // this->f_Mesh[5].DrawSphere(3, RED);
  // this->f_Mesh[6].DrawSphere(3, RED);
  // this->f_Mesh[7].DrawSphere(3, RED);
}



std::array<raylib::Vector3, 8> Frustum::MakeMesh() const {

  std::array<raylib::Vector3, 8> Mesh;

  const float CameraW = this->f_Aspect * f_CameraH;
  const float Near = (f_CameraH / 2) / tan(this->f_FOV / 2);
  const float BackHeight = 2 * ((this->f_FOD + Near) * tan(this->f_FOV / 2));
  const float BackWidth = BackHeight * this->f_Aspect;

  Mesh[0] = this->f_Origin.Add(this->f_Right.Scale(CameraW / 2)).Add(this->f_Front.Scale(-Near)).Add(f_Up.Scale(-f_CameraH / 2));
  Mesh[1] = this->f_Origin.Add(this->f_Right.Scale(BackWidth / 2)).Add(this->f_Front.Scale(-this->f_FOD)).Add(f_Up.Scale(-BackHeight / 2));
  Mesh[2] = this->f_Origin.Add(this->f_Right.Scale(BackWidth / 2)).Add(this->f_Front.Scale(-this->f_FOD)).Add(f_Up.Scale(BackHeight / 2));
  Mesh[3] = this->f_Origin.Add(this->f_Right.Scale(CameraW / 2)).Add(this->f_Front.Scale(-Near)).Add(f_Up.Scale(f_CameraH / 2));
  Mesh[4] = this->f_Origin.Add(this->f_Right.Scale(-CameraW / 2)).Add(this->f_Front.Scale(-Near)).Add(f_Up.Scale(-f_CameraH / 2));
  Mesh[5] = this->f_Origin.Add(this->f_Right.Scale(-BackWidth / 2)).Add(this->f_Front.Scale(-this->f_FOD)).Add(f_Up.Scale(-BackHeight / 2));
  Mesh[6] = this->f_Origin.Add(this->f_Right.Scale(-BackWidth / 2)).Add(this->f_Front.Scale(-this->f_FOD)).Add(f_Up.Scale(BackHeight / 2));
  Mesh[7] = this->f_Origin.Add(this->f_Right.Scale(-CameraW / 2)).Add(this->f_Front.Scale(-Near)).Add(f_Up.Scale(f_CameraH / 2));

  return Mesh;
}
