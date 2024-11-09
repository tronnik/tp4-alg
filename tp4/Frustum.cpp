#include "Frustum.h"

#include "Plane.h"
#include "AABB.h"

float Frustum::f_CameraH = 1; // check actual value

Frustum::Frustum(float FOD, const raylib::Camera& Camera)
  : f_FOD(FOD), f_FOV(Camera.GetFovy()), f_Aspect(1.69F) {

  Update(Camera);
}



void Frustum::AddObject(const AABB& object) {
  f_Objects.push_back(object);
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
  const Vector3 FarCenter = f_Origin + (-f_Front * (this->f_NearDistance + f_FOD));

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



//void Frustum::Move(const raylib::Vector3 NewPosition) {
//	this->f_Origin = NewPosition;
//	this->f_Mesh = MakeMesh();
//}

std::array<raylib::Vector3, 8> Frustum::GetMesh() const {
  return this->f_Mesh;
}



void Frustum::drawAABB(const AABB& aabb) {
  // Calcula las esquinas del AABB
  Vector3 min = Vector3Subtract(aabb.center, aabb.extents);
  Vector3 max = Vector3Add(aabb.center, aabb.extents);

  // Define las 8 esquinas del cubo (AABB)
  Vector3 corners[8] = {
      (min.x, min.y, min.z),
      (min.x, min.y, max.z),
      (min.x, max.y, min.z),
      (min.x, max.y, max.z),
      (max.x, min.y, min.z),
      (max.x, min.y, max.z),
      (max.x, max.y, min.z),
      (max.x, max.y, max.z)};

  // Conectar las esquinas del AABB con líneas
  for (int i = 0; i < 4; i++) {
    // Frente
    DrawLine3D(corners[i], corners[(i + 1) % 4], WHITE);
    DrawLine3D(corners[i + 4], corners[(i + 1) % 4 + 4], WHITE); // Atrás
    DrawLine3D(corners[i], corners[i + 4], WHITE);               // Lados
  }
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

  // this->f_Mesh[0].DrawSphere(0.5, RED);
  // this->f_Mesh[1].DrawSphere(0.5, RED);
  // this->f_Mesh[2].DrawSphere(0.5, RED);
  // this->f_Mesh[3].DrawSphere(0.5, RED);
   //this->f_Mesh[4].DrawSphere(0.5, RED);
   //this->f_Mesh[5].DrawSphere(0.5, RED);
  // this->f_Mesh[6].DrawSphere(0.5, RED);
   //this->f_Mesh[7].DrawSphere(0.5, RED);
}



void Frustum::DrawVisibleObjects() {
  for (auto& obj : f_Objects) {
    if (this->isAABBVisible(obj)) {
      drawAABB(obj);
    }
  }

  //for ( auto& obj : objects) {
  //	if (this->isAABBVisible(obj)) {
  //		// Dibujar el objeto, ya que está dentro del frustum
  //		drawAABB(obj);
  //	}
  //}
}



bool Frustum::isAABBVisible(AABB& aabb) {
  // Verificar si el AABB está al frente de cada plano del frustum
  for (auto& plane : f_Planes) {
    if (!aabb.isOnOrForwardPlane(plane)) {
      return false; // El AABB está fuera de la vista del frustum
    }
  }
  return true; // El AABB está dentro o en el borde del frustum
}



void Frustum::DrawPlaneNormals() {
  //DrawLine3D(f_Planes[0].f_Center, Vector3Add(Vector3Scale(f_Planes[0].f_Normal, 10),f_Planes[0].f_Center ), RED);
  //DrawLine3D(f_Planes[1].f_Center, Vector3Add(Vector3Scale(f_Planes[1].f_Normal, 10),f_Planes[1].f_Center ), RED);
  //DrawLine3D(f_Planes[2].f_Center, Vector3Add(Vector3Scale(f_Planes[2].f_Normal, 10),f_Planes[2].f_Center ), RED);
  //DrawLine3D(f_Planes[3].f_Center, Vector3Add(Vector3Scale(f_Planes[3].f_Normal, 10),f_Planes[3].f_Center ), RED);
  DrawLine3D(f_Planes[4].f_Center, Vector3Add(Vector3Scale(f_Planes[4].f_Normal, 10),f_Planes[4].f_Center ), RED);
  //DrawLine3D(f_Planes[5].f_Center, Vector3Add(Vector3Scale(f_Planes[5].f_Normal, 10),f_Planes[5].f_Center ), RED);

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
