#ifndef __TEXTURE2D_H
#define __TEXTURE2D_H

#include <GL/glew.h>

class Texture2D
{

private:
	GLuint _texHandle;
	const char* _filePath;

public:
	// Constructor
	Texture2D();

	// Destructor
	~Texture2D();

	// Copy Constructor
	// Copy Assignment

	// Load Texture
	void LoadTexture(const char* filePath);

	// Bind a texture in a specific slot
	void BindInSlot(GLuint textureUnit);
};

#endif // __TEXTURE2D_H