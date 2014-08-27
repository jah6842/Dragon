#ifndef __GLPROGRAM_H
#define __GLPROGRAM_H

#include <GL/glew.h>
#include <string>

class GLProgram {
public:
	GLProgram();
	~GLProgram();

	static std::string basePath;

	void LoadVertexShader(const char* name);
	void LoadFragmentShader(const char* name);
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