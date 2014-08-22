#ifndef __GLPROGRAM_H
#define __GLPROGRAM_H

#include <GL/glew.h>

class GLProgram {
public:
	GLProgram();
	~GLProgram();

	void LoadVertexShader(const char* name);
	void LoadFragmentShader(const char* name);
	void Link();
	void Use();
	GLuint GetHandle();
private:

	static const char* basePath;

	void LogShaderInfo(GLuint shaderHandle);

	GLuint vShaderHandle;
	GLuint fShaderHandle;
	GLuint programHandle;
};

#endif // __GLPROGRAM_H