#include "Camera.h"

// Constructor
Camera::Camera(uint32_t width, uint32_t height, float fov, float nearClip, float farClip) 
	: _width(width), _height(height), _fov(fov), _nearClip(nearClip), _farClip(farClip) {

	transform = new Transform();
	RecalcProjMatrix();
}

// Destructor
Camera::~Camera(){

}

// Update the camera
void Camera::Update(){
	RecalcViewMatrix();
}

// Returns the camera's projection matrix
glm::mat4 Camera::GetProjectionMatrix(){
	return _projection;
};

// Returns the camera's view matrix
glm::mat4 Camera::GetViewMatrix(){
	return _view;
};

// Recalculate the view matrix
void Camera::RecalcViewMatrix(){
	// Set up view matrix (camera)
	//XMVECTOR position = transform.PosToVector();
	//XMVECTOR target = XMVectorSet(0, 0, 1, 0);
	//XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	//XMMATRIX V = XMMatrixLookToLH(position, target, up);
	//XMStoreFloat4x4(&_view, XMMatrixTranspose(V));

	//glm::vec3 camera_look_at = transform->Pos() + glm::

	glm::vec3 position = transform->Pos();
	glm::vec3 target = position + glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	_view = glm::lookAt(position, target, up);

	//RecalcFrustum();
};

// Recalculate the viewing frustum
// TODO
/*
void Camera::RecalcFrustum(){

	XMMATRIX m_view = XMLoadFloat4x4(&_view);
	XMMATRIX m_projection = XMLoadFloat4x4(&_projection);
	XMMATRIX viewProjection = XMMatrixMultiply(m_projection, m_view);

	XMFLOAT4X4 vp;// = _view * _projection;
	XMStoreFloat4x4(&vp, XMMatrixTranspose(viewProjection));

	// Left Frustum Plane
	// Add first column of the matrix to the fourth column
	_frustum[0][0] = vp._14 + vp._11;
	_frustum[0][1] = vp._24 + vp._21;
	_frustum[0][2] = vp._34 + vp._31;
	_frustum[0][3] = vp._44 + vp._41;

	// Right Frustum Plane
	// Subtract first column of matrix from the fourth column
	_frustum[1][0] = vp._14 - vp._11;
	_frustum[1][1] = vp._24 - vp._21;
	_frustum[1][2] = vp._34 - vp._31;
	_frustum[1][3] = vp._44 - vp._41;

	// Top Frustum Plane
	// Subtract second column of matrix from the fourth column
	_frustum[2][0] = vp._14 - vp._12;
	_frustum[2][1] = vp._24 - vp._22;
	_frustum[2][2] = vp._34 - vp._32;
	_frustum[2][3] = vp._44 - vp._42;

	// Bottom Frustum Plane
	// Add second column of the matrix to the fourth column
	_frustum[3][0] = vp._14 + vp._12;
	_frustum[3][1] = vp._24 + vp._22;
	_frustum[3][2] = vp._34 + vp._32;
	_frustum[3][3] = vp._44 + vp._42;

	// Near Frustum Plane
	// We could add the third column to the fourth column to get the near plane,
	// but we don't have to do this because the third column IS the near plane
	_frustum[4][0] = vp._13;
	_frustum[4][1] = vp._23;
	_frustum[4][2] = vp._33;
	_frustum[4][3] = vp._43;

	// Far Frustum Plane
	// Subtract third column of matrix from the fourth column
	_frustum[5][0] = vp._14 - vp._13;
	_frustum[5][1] = vp._24 - vp._23;
	_frustum[5][2] = vp._34 - vp._33;
	_frustum[5][3] = vp._44 - vp._43;

	for (int i = 0; i < 6; ++i)
	{
		float length = sqrt((_frustum[i][0] * _frustum[i][0]) + (_frustum[i][1] * _frustum[i][1]) + (_frustum[i][2] * _frustum[i][2]));
		_frustum[i][0] /= length;
		_frustum[i][1] /= length;
		_frustum[i][2] /= length;
		_frustum[i][3] /= length;
	}

};

// Returns true if a point lies within the camera's frustrum
bool Camera::PointInFrustum(float x, float y, float z){
	int p;
	for (p = 0; p < 6; p++)
		if (_frustum[p][0] * x + _frustum[p][1] * y + _frustum[p][2] * z + _frustum[p][3] <= 0)
			return false;
	return true;
};

// Returns true if a point lies within the camera's frustrum
bool Camera::PointInFrustum(XMFLOAT3 position){
	return PointInFrustum(position.x, position.y, position.z);
};
*/

// Recalculate the projection matrix
void Camera::RecalcProjMatrix(){
	_projection = glm::perspective(	_fov,
		static_cast<float>(_width) / static_cast<float>(_height),
		_nearClip, _farClip);

	/*XMMATRIX P = XMMatrixPerspectiveFovLH(
		_fov,
		(float)_width / (float)_height,
		_nearClip,
		_farClip);
	XMStoreFloat4x4(&_projection, XMMatrixTranspose(P));*/
};

// Resize the camera
void Camera::Resize(uint32_t width, uint32_t height){
	_width = width;
	_height = height;
	RecalcProjMatrix();
};