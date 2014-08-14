#ifndef __CAMERA_H
#define __CAMERA_H

#include <cinttypes>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

class Camera {
public:
	// Constructor
	Camera(uint32_t width, uint32_t height, 
		float fov = 75.0f, float nearClip = 0.1f, float farClip = 1000.0f);
	// Destructor
	~Camera();

	// Update the camera
	void Update();

	// Getters
	glm::mat4 GetProjectionMatrix();
	glm::mat4 GetViewMatrix();

	// Resize the camera and recalc necessary  matrices
	void Resize(uint32_t width, uint32_t height);

	// Transform object
	Transform* transform;
private:

	glm::mat4x4 _projection;	// Projection Matrix
	glm::mat4x4 _view;			// View Matrix
	//float _frustum[6][4];		// View Frustum

	float _nearClip;			// Near clipping plane
	float _farClip;				// Far clipping plane
	float _fov;					// Field of view
	uint32_t _width;			// Width (in pixels)
	uint32_t _height;			// Height (in pixels)

	// Private helper functions
	void RecalcProjMatrix();
	void RecalcViewMatrix();
	//void RecalcFrustum();
};

#endif // __CAMERA_H