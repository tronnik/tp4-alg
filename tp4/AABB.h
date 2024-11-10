#pragma once
#include <array>
#include <raylib-cpp.hpp>



class AABB {
public:
  AABB(const float* ObjectsVert, int VertexCount, raylib::Vector3 Origin);

  std::array<raylib::Vector3, 8> GetMesh();

private:

  std::array<raylib::Vector3, 8> f_Mesh;

  raylib::Vector3 f_Origin;

  void MakeMesh(const float* ObjectsVert, int VertexCount);
};
