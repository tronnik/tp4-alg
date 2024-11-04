#pragma once

#include <array>
#include <raylib-cpp.hpp>



class Frustum {
  static float f_CameraH;

public:
  Frustum(float FOD, const raylib::Camera& Camera);

  void Update(raylib::Camera Camera);

  void Move(raylib::Vector3 NewPosition);

  std::array<raylib::Vector3, 8> GetMesh() const;

  void DrawWireframe() const;

private:
  std::array<raylib::Vector3, 8> MakeMesh() const;

  std::array<raylib::Vector3, 8> f_Mesh;

  raylib::Vector3 f_Origin;

  /**
   * @brief points to target
   */
  raylib::Vector3 f_Front;

  /**
   * @brief points up
   */
  raylib::Vector3 f_Up;

  /**
   * @brief points right
   */
  raylib::Vector3 f_Right;
  float f_FOD;
  float f_FOV;
  float f_Aspect;
};
