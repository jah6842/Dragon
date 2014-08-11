#ifndef __GLPROGRAM_H
#define __GLPROGRAM_H

#include <string>
#include <GL\glew.h>

class GLProgram {
public:
	GLProgram();
	~GLProgram();

	void LoadVertexShader(const char* path);
	void LoadFragmentShader(const char* path);
	void Link();
	void Use();
	GLuint GetHandle();
private:

	void LogShaderInfo(GLuint shaderHandle);

	GLuint vShaderHandle;
	GLuint fShaderHandle;
	GLuint programHandle;
};

#endif // __GLPROGRAM_H