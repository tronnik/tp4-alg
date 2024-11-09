#include "Frustum.h"

#include "Plane.h"
#include "AABB.h"

float Frustum::f_CameraH = 1; // check actual value

Frustum::Frustum(float FOD, const raylib::Camera& Camera)
	: f_FOD(FOD), f_FOV(Camera.GetFovy()), f_Aspect(1.69F) {

	Update(Camera);
}

void Frustum::AddObject(const AABB& object)
{
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
	// Calcular la mitad de la altura y el ancho del frustum cercano
	float nearHeight = f_CameraH;
	float nearWidth = f_Aspect * nearHeight;

	// Distancia al plano cercano y lejano desde el origen de la cámara
	Vector3 nearCenter = f_Origin + f_Front * (f_FOD * 0.1f);  // Distancia al plano cercano (10% del FOD)
	Vector3 farCenter = f_Origin + f_Front * f_FOD;            // Distancia al plano lejano

	// Plano cercano
	planes[0] = Plane(nearCenter, f_Front); // Normal hacia adelante

	// Plano lejano
	planes[1] = Plane(farCenter, -f_Front); // Normal hacia atrás

	// Plano izquierdo
	Vector3 leftNormal = (f_Up * nearHeight - f_Right * nearWidth).Normalize();
	planes[2] = Plane(nearCenter, leftNormal);

	// Plano derecho
	Vector3 rightNormal = (f_Right * nearWidth + f_Up * nearHeight).Normalize();
	planes[3] = Plane(nearCenter, Vector3Negate(rightNormal));

	// Plano superior
	Vector3 topNormal = (f_Up * nearHeight + f_Front).Normalize();
	planes[4] = Plane(nearCenter, topNormal);

	// Plano inferior
	Vector3 bottomNormal = (-f_Up * nearHeight + f_Front).Normalize();
	planes[5] = Plane(nearCenter, bottomNormal);
}

//void Frustum::Move(const raylib::Vector3 NewPosition) {
//	this->f_Origin = NewPosition;
//	this->f_Mesh = MakeMesh();
//}

std::array<raylib::Vector3, 8> Frustum::GetMesh() const {
	return this->f_Mesh;
}

void Frustum::drawAABB(const AABB& aabb)
{
	// Calcula las esquinas del AABB
	Vector3 min = Vector3Subtract(aabb.center, aabb.extents);
	Vector3 max = Vector3Add(aabb.center, aabb.extents);

	// Define las 8 esquinas del cubo (AABB)
	Vector3 corners[8] =
	{
		(min.x, min.y, min.z),
		(min.x, min.y, max.z),
		(min.x, max.y, min.z),
		(min.x, max.y, max.z),
		(max.x, min.y, min.z),
		(max.x, min.y, max.z),
		(max.x, max.y, min.z),
		(max.x, max.y, max.z)
	};

	// Conectar las esquinas del AABB con líneas
	for (int i = 0; i < 4; i++) {
		// Frente
		DrawLine3D(corners[i], corners[(i + 1) % 4], WHITE);
		DrawLine3D(corners[i + 4], corners[(i + 1) % 4 + 4], WHITE); // Atrás
		DrawLine3D(corners[i], corners[i + 4], WHITE); // Lados
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

	// this->f_Mesh[0].DrawSphere(3, RED);
	// this->f_Mesh[1].DrawSphere(3, RED);
	// this->f_Mesh[2].DrawSphere(3, RED);
	// this->f_Mesh[3].DrawSphere(3, RED);
	// this->f_Mesh[4].DrawSphere(3, RED);
	// this->f_Mesh[5].DrawSphere(3, RED);
	// this->f_Mesh[6].DrawSphere(3, RED);
	// this->f_Mesh[7].DrawSphere(3, RED);
}

void Frustum::DrawVisibleObjects() 
{
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
	for (auto& plane : planes) {
		if (!aabb.isOnOrForwardPlane(plane)) {
			return false; // El AABB está fuera de la vista del frustum
		}
	}
	return true; // El AABB está dentro o en el borde del frustum
}

std::array<raylib::Vector3, 8> Frustum::MakeMesh() const
{
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
