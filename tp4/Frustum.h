#pragma once

#include <array>
#include <raylib-cpp.hpp>

#include "Plane.h"
#include "AABB.h"

class Frustum {
	static float f_CameraH;
private:

	std::array<raylib::Vector3, 8> MakeMesh();

	std::array<raylib::Vector3, 8> f_Mesh;

	std::array<Plane, 6> f_Planes;  // Array para almacenar los seis planos

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
	float f_NearDistance;
	float f_BackHeight;
	float f_BackWidth;


public:
	Frustum(float FOD, const raylib::Camera& Camera);

	void Update(raylib::Camera Camera);
	
	void UpdatePlanes();  // Nueva función para actualizar planos

	std::array<raylib::Vector3, 8> GetMesh() const;


	bool IsObjectVisible(std::array<raylib::Vector3, 8> ObjectMesh) const;  // Método para frustum culling


};
