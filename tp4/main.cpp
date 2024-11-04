#include <raylib-cpp.hpp>

#include "Frustum.h"



int main(void) {
  raylib::Window Window;
  raylib::Camera Camera;
  Window.Init();

  // Define the camera to look into our 3d world
  Camera.position = {10.0f, 10.0f, 10.0f};
  Camera.target = {0.0f, 0.0f, 0.0f};
  Camera.up = {0.0f, 1.0f, 0.0f};
  Camera.fovy = 45.0f;
  Camera.projection = CAMERA_PERSPECTIVE;

  Frustum Frustum(20, Camera);

  DisableCursor();

  Window.SetTargetFPS(60);

  // Main game loop
  while (!WindowShouldClose()) {
    Camera.Update(CAMERA_FREE);

    Frustum.Update(Camera);

    Window.BeginDrawing();

    Window.ClearBackground(BLACK);

    Camera.BeginMode();

    Frustum.DrawWireframe();

    DrawGrid(20,1);

    Camera.EndMode();

    Window.EndDrawing();
  }

  Window.Close(); // Close window and OpenGL context


  return 0;
}
