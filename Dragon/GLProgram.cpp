#include "GLProgram.h"
#include "Utilities.h"
#include <string>

const char* GLProgram::basePath = "C:/Users/Scales/Desktop/DragonEngine/Dragon/Shaders/";

GLProgram::GLProgram() : vShaderHandle(0), fShaderHandle(0) {

}

GLProgram::~GLProgram(){
	glDeleteProgram(programHandle);
	glDeleteShader(vShaderHandle);
	glDeleteShader(fShaderHandle);
}

void GLProgram::LoadVertexShader(const char* name){
	// Load shader from file
	std::string data;
	std::string fullPath = basePath; fullPath += name;

	dgnLoadTextFromFile(fullPath.c_str(), data);
	const GLchar* shaderSrc = data.c_str();
	GLint shaderSrcLen = data.length();

	// Create OpenGL Vertex Shader
	vShaderHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShaderHandle, 1, &shaderSrc, &shaderSrcLen);
	glCompileShader(vShaderHandle);

#if _DEBUG
	LogShaderInfo(vShaderHandle);
#endif // _DEBUG

}

void GLProgram::LoadFragmentShader(const char* name){
	// Load shader from file
	std::string data;
	std::string fullPath = basePath; fullPath += name;

	dgnLoadTextFromFile(fullPath.c_str(), data);
	const GLchar* shaderSrc = data.c_str();
	GLint shaderSrcLen = data.length();

	// Create OpenGL Fragment Shader
	fShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShaderHandle, 1, &shaderSrc, &shaderSrcLen);
	glCompileShader(fShaderHandle);

#if _DEBUG
	LogShaderInfo(fShaderHandle);
#endif // _DEBUG

}

void GLProgram::Link(){
	// Setup and link program
	programHandle = glCreateProgram();
	assert(programHandle != 0);

	assert(vShaderHandle != 0);
	glAttachShader(programHandle, vShaderHandle);
	assert(fShaderHandle != 0);
	glAttachShader(programHandle, fShaderHandle);

	glLinkProgram(programHandle);
}

void GLProgram::Use(){
	assert(programHandle != 0);
	glUseProgram(programHandle);
}

GLuint GLProgram::GetHandle(){
	return programHandle;
}

void GLProgram::LogShaderInfo(GLuint shaderHandle){
	GLint status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE){

		int bufferSize;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &bufferSize);

		char* buffer = new char[bufferSize];
		glGetShaderInfoLog(shaderHandle, bufferSize, NULL, buffer);
		printf("%s\n", buffer);
		printf("%d\n", status);

		delete buffer;
	}
}