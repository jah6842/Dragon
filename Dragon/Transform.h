#ifndef __TRANSFORM_H
#define __TRANSFORM_H

#include <glm/glm.hpp>

class Transform {
public:
	Transform();
	~Transform();

	// Getters
	glm::vec3 Pos();
	float PosX();
	float PosY();
	float PosZ();

	// Setters
	void SetPosition(float x, float y, float z);
	void SetPosition(glm::vec3 pos);
	void SetRotation(float xRad, float yRad, float zRad);
	void SetRotation(glm::vec3 rotRad);
	void SetScale(float x, float y, float z);
	void SetScale(glm::vec3 sca);

	// Movement functions
	void Move(float x, float y, float z);
	void Move(glm::vec3 pos);

	// Rotation functions
	void Rotate(float x, float y, float z);
	void Rotate(glm::vec3 rotRad);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);

	// Returns a model matrix of the pos, rot, and scale
	glm::mat4 WorldMatrix();
	void RecalcWorldMatrix();

	// Returns an XMVECTOR of the position, rotation, or scale. Unused 4th value is 0.0f.
	glm::vec3 PosToVector();
	glm::vec3 RotToVector();
	glm::vec3 ScaleToVector();

	// Returns an identity transform
	// Position/rotation 0,0,0, Scale 1,1,1
	//static Transform Identity();

	// Returns a world matrix of the pos, rot, and scale
	//static mat4 WorldMatrix(Transform t);

private:
	// Internals
	glm::mat4 _matrix;
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _scale;
	bool _dirty;
};

#endif // __TRANSFORM_H