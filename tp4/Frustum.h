#pragma once

#include <array>
#include <raylib-cpp.hpp>

#include "Plane.h"
#include "AABB.h"

class Frustum {
	static float f_CameraH;
private:

	std::vector<AABB> f_Objects;

	std::array<raylib::Vector3, 8> MakeMesh() const;

	std::array<raylib::Vector3, 8> f_Mesh;
	std::array<Plane, 6> planes;  // Array para almacenar los seis planos

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


public:
	Frustum(float FOD, const raylib::Camera& Camera);

	void AddObject(const AABB& object);

	void Update(raylib::Camera Camera);
	
	void UpdatePlanes();  // Nueva función para actualizar planos

	void Move(raylib::Vector3 NewPosition);

	std::array<raylib::Vector3, 8> GetMesh() const;

	void drawAABB(const AABB& aabb);
	void DrawWireframe() const;

	// Método para dibujar solo los objetos visibles dentro del frustum
	void DrawVisibleObjects();

	bool isAABBVisible( AABB& aabb) ;  // Método para frustum culling


};
