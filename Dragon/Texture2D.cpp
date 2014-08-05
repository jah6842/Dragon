#include "Texture2D.h"

#include "SOIL/SOIL.h"
#include <cassert>

Texture2D::Texture2D(){
	// Initialize GL stuff
	glGenTextures(1, &_texHandle);
	glActiveTexture(GL_MAX_TEXTURE_UNITS - 1);
	glBindTexture(GL_TEXTURE_2D, _texHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	_filePath = nullptr;
}

Texture2D::~Texture2D(){
	glDeleteTextures(1, &_texHandle);

	if (_filePath != nullptr)
		delete _filePath;
}

void Texture2D::LoadTexture(const char* filePath){

	// Load image, upload to GPU
	int width, height;
	unsigned char* image = SOIL_load_image(filePath, &width, &height, 0, SOIL_LOAD_RGB);

	assert(image != nullptr);

	glActiveTexture(GL_MAX_TEXTURE_UNITS - 1);
	glBindTexture(GL_TEXTURE_2D, _texHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	_filePath = filePath;
}

void Texture2D::BindInSlot(GLuint textureUnit){

	// Make sure we pass a valid unit
	assert(textureUnit >= GL_TEXTURE0 && textureUnit < GL_MAX_TEXTURE_UNITS);

	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, _texHandle);
}