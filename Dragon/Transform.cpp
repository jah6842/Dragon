#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Transform::Transform(){
	_position = glm::vec3(0, 0, 0);
	_rotation = glm::vec3(0, 0, 0);
	_scale = glm::vec3(1, 1, 1);
	_dirty = true;
};

Transform::~Transform(){

};

// Getters
glm::vec3 Transform::Pos(){
	return _position;
};
float Transform::PosX(){
	return _position.x;
};
float Transform::PosY(){
	return _position.y;
};
float Transform::PosZ(){
	return _position.z;
};

// Setters
void Transform::SetPosition(float x, float y, float z){
	_position = glm::vec3(x, y, z);
	_matrix[0][3] = x;
	_matrix[1][3] = y;
	_matrix[2][3] = z;
	//_matrix._14 = x;
	//_matrix._24 = y;
	//_matrix._34 = z;
};
void Transform::SetPosition(glm::vec3 pos){
	_position = pos;
	_matrix[0][3] = pos.x;
	_matrix[1][3] = pos.y;
	_matrix[2][3] = pos.z;
	//_matrix._14 = pos.x;
	//_matrix._24 = pos.y;
	//_matrix._34 = pos.z;
};
void Transform::SetRotation(float xRad, float yRad, float zRad){
	_rotation = glm::vec3(xRad, yRad, zRad);
	_dirty = true;
};
void Transform::SetRotation(glm::vec3 rotRad){
	_rotation = rotRad;
	_dirty = true;
};
void Transform::SetScale(float x, float y, float z){
	_scale = glm::vec3(x, y, z);
	_dirty = true;
};
void Transform::SetScale(glm::vec3 sca){
	_scale = sca;
	_dirty = true;
};

// Movement functions
void Transform::Move(float x, float y, float z){
	_position.x += x;
	_position.y += y;
	_position.z += z;
	_matrix[0][3] += x;
	_matrix[1][3] += y;
	_matrix[2][3] += z;
	//_matrix._14 += x;
	//_matrix._24 += y;
	//_matrix._34 += z;
};
void Transform::Move(glm::vec3 pos){
	Move(pos.x, pos.y, pos.z);
};

// Rotation functions
void Transform::Rotate(float xRad, float yRad, float zRad){
	RotateX(xRad);
	RotateY(yRad);
	RotateZ(zRad);
};
void Transform::Rotate(glm::vec3 rotRad){
	Rotate(rotRad.x, rotRad.y, rotRad.z);
};
void Transform::RotateX(float xRad){
	_rotation.x += xRad;
	_dirty = true;
};
void Transform::RotateY(float yRad){
	_rotation.y += yRad;
	_dirty = true;
};
void Transform::RotateZ(float zRad){
	_rotation.z += zRad;
	_dirty = true;
};

// Returns a model matrix of the pos, rot, and scale
void Transform::RecalcWorldMatrix(){

	// T * R * S
	glm::mat4 scaleMat = glm::scale(_scale);
	glm::mat4 rotMat = glm::mat4(1.0f);
	rotMat = glm::rotate(rotMat, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	rotMat = glm::rotate(rotMat, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	rotMat = glm::rotate(rotMat, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 transMat = glm::translate(glm::vec3(_position));

	//XMMATRIX pos = XMMatrixTranslation(_position.x, _position.y, _position.z);
	//XMMATRIX rot = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
	//XMMATRIX sca = XMMatrixScaling(_scale.x, _scale.y, _scale.z);

	//XMMATRIX model = sca * rot * pos;

	_matrix = transMat * rotMat * scaleMat;

	//XMStoreFloat4x4(&_matrix, XMMatrixTranspose(model));
};

// Returns a model matrix of the pos, rot, and scale
glm::mat4 Transform::WorldMatrix(){
	if (_dirty){
		RecalcWorldMatrix();
		_dirty = false;
	}
	return _matrix;
};

// Returns an XMVECTOR of the position, rotation, or scale. Unused 4th value is 0.0f.
glm::vec3 Transform::PosToVector(){
	//vec3 v = XMVectorSet(_position.x, _position.y, _position.z, 0.0f);
	return _position;
};
glm::vec3 Transform::RotToVector(){
	//XMVECTOR v = XMVectorSet(_rotation.x, _rotation.y, _rotation.z, 0.0f);
	return _rotation;
};
glm::vec3 Transform::ScaleToVector(){
	//XMVECTOR v = XMVectorSet(_scale.x, _scale.y, _scale.z, 0.0f);
	return _scale;
};

// Returns a model matrix of the pos, rot, and scale
//mat4 Transform::WorldMatrix(Transform t){
//	XMMATRIX pos = XMMatrixTranslation(t._position.x, t._position.y, t._position.z);
//	XMMATRIX rot = XMMatrixRotationRollPitchYaw(t._rotation.x, t._rotation.y, t._rotation.z);
//	XMMATRIX sca = XMMatrixScaling(t._scale.x, t._scale.y, t._scale.z);
//
//	XMMATRIX model = sca * rot * pos;
//
//	XMFLOAT4X4 modelMatrix;
//	XMStoreFloat4x4(&modelMatrix, XMMatrixTranspose(model));
//	return modelMatrix;
//};

// Returns an identity transform
// Position/rotation 0,0,0, Scale 1,1,1
//Transform Transform::Identity(){
//	Transform t;
//	t._position = glm::vec3(0, 0, 0);
//	t._rotation = glm::vec3(0, 0, 0);
//	t._scale = glm::vec3(1, 1, 1);
//	return t;
//};