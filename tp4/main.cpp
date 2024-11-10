#include <raylib-cpp.hpp>

#include "Frustum.h"



void DrawWireframe(const std::array<raylib::Vector3, 8>& Mesh,
                   const bool DrawVertices = false) {
  //rightside
  Mesh[0].DrawLine3D(Mesh[1], raylib::Color::White());
  Mesh[1].DrawLine3D(Mesh[2], raylib::Color::White());
  Mesh[2].DrawLine3D(Mesh[3], raylib::Color::White());
  Mesh[3].DrawLine3D(Mesh[0], raylib::Color::White());

  //leftside
  Mesh[4].DrawLine3D(Mesh[5], raylib::Color::White());
  Mesh[5].DrawLine3D(Mesh[6], raylib::Color::White());
  Mesh[6].DrawLine3D(Mesh[7], raylib::Color::White());
  Mesh[7].DrawLine3D(Mesh[4], raylib::Color::White());

  //Front
  Mesh[3].DrawLine3D(Mesh[7], raylib::Color::White());
  Mesh[4].DrawLine3D(Mesh[0], raylib::Color::White());

  //back
  Mesh[2].DrawLine3D(Mesh[6], raylib::Color::White());
  Mesh[5].DrawLine3D(Mesh[1], raylib::Color::White());

  if (DrawVertices) {
    Mesh[0].DrawSphere(0.5, RED);
    Mesh[1].DrawSphere(0.5, RED);
    Mesh[2].DrawSphere(0.5, RED);
    Mesh[3].DrawSphere(0.5, RED);
    Mesh[4].DrawSphere(0.5, RED);
    Mesh[5].DrawSphere(0.5, RED);
    Mesh[6].DrawSphere(0.5, RED);
    Mesh[7].DrawSphere(0.5, RED);
  }
}



int main(void) {
  raylib::Window Window;
  raylib::Camera RealCam;
  raylib::Camera FakeCam;
  Window.Init();

  // Define the camera to look into our 3d world
  RealCam.position = {10.0f, 10.0f, 10.0f};
  RealCam.target = {0.0f, 0.0f, 0.0f};
  RealCam.up = {0.0f, 1.0f, 0.0f};
  RealCam.fovy = 45.0f;
  RealCam.projection = CAMERA_PERSPECTIVE;

  FakeCam.position = {10.0f, 10.0f, 10.0f};
  FakeCam.target = {0.0f, 0.0f, 0.0f};
  FakeCam.up = {0.0f, 1.0f, 0.0f};
  FakeCam.fovy = 45.0f;
  FakeCam.projection = CAMERA_PERSPECTIVE;

  Frustum Frustum(20, FakeCam);

  DisableCursor();

  Window.SetTargetFPS(60);

  raylib::Model Knot = LoadModelFromMesh(GenMeshKnot(1.0f, 2.0f, 16, 128));

  auto Origin = raylib::Vector3(0, 0, 0);

  auto KnotAABB = AABB(Knot.GetMeshes()->vertices,
                       Knot.GetMeshes()->vertexCount, Origin);

  // Main game loop
  while (!WindowShouldClose()) {
    RealCam.Update(CAMERA_FREE);

    Frustum.Update(FakeCam);

    Frustum.UpdatePlanes();

    Window.BeginDrawing();

    RealCam.BeginMode();

    Window.ClearBackground(BLACK);

    DrawWireframe(Frustum.GetMesh());

    DrawWireframe(KnotAABB.GetMesh());

    if (Frustum.IsObjectVisible(KnotAABB.GetMesh())) {
      DrawModel(Knot, Origin, 1.0f, RED);
    }

    DrawGrid(20, 1);

    RealCam.EndMode();

    Window.EndDrawing();
  }

  Window.Close(); // Close window and OpenGL context

  return 0;
}
