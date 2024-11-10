#include "AABB.h"



AABB::AABB(const float* ObjectsVert, const int VertexCount, const raylib::Vector3 Origin) : f_Origin(Origin) {
  MakeMesh(ObjectsVert, VertexCount);
}



std::array<raylib::Vector3, 8> AABB::GetMesh() {
  return f_Mesh;
}



void AABB::MakeMesh(const float* ObjectsVert, int VertexCount) {

  raylib::Vector3 Min = {ObjectsVert[0], ObjectsVert[1], ObjectsVert[2],};
  raylib::Vector3 Max = {ObjectsVert[0], ObjectsVert[1], ObjectsVert[2],};

  f_Mesh.fill({0, 0, 0});

  for (int I = 1; I < VertexCount; I++) {
    Min = Min.Min({
        ObjectsVert[I * 3],
        ObjectsVert[I * 3 + 1],
        ObjectsVert[I * 3 + 2]});
    Max = Max.Max({
        ObjectsVert[I * 3],
        ObjectsVert[I * 3 + 1],
        ObjectsVert[I * 3 + 2]});
  }

  f_Mesh[4] = Min;
  f_Mesh[2] = Max;

  f_Mesh[0] = raylib::Vector3(Max.x, Min.y, Min.z);
  f_Mesh[1] = raylib::Vector3(Max.x, Min.y, Max.z);
  f_Mesh[3] = raylib::Vector3(Max.x, Max.x, Min.z);
  f_Mesh[5] = raylib::Vector3(Min.x, Min.y, Max.z);
  f_Mesh[6] = raylib::Vector3(Min.x, Max.y, Max.z);
  f_Mesh[7] = raylib::Vector3(Min.x, Max.y, Min.z);

  for(auto& Vertex: f_Mesh) {
    Vertex = Vertex.Add(f_Origin);
  }
}
