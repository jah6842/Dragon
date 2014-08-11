
#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture2D.h"
#include "GLProgram.h"

const std::string resourceDir = "..\\..\\Resources";

Texture2D* girlTex;
Texture2D* dargTex;
GLProgram* basicProgram;
GLFWwindow* window; 

int windowWidth, windowHeight;
float aspectRatio = 16.0f / 9.0f;

void Draw(){
	// Reset state
	glUseProgram(0);
	// Clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw
	basicProgram->Use();
	girlTex->BindInSlot(GL_TEXTURE0);
	dargTex->BindInSlot(GL_TEXTURE1);

	glm::mat4 model;

	model = glm::rotate(
		model,
		(float)clock() / (float)CLOCKS_PER_SEC * 180.0f,
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	GLint uniModel = glGetUniformLocation(basicProgram->GetHandle(), "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::lookAt(
		glm::vec3(2.0f, 3.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	GLint uniView = glGetUniformLocation(basicProgram->GetHandle(), "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(75.0f, aspectRatio, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(basicProgram->GetHandle(), "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	float time = (float)clock() / (float)CLOCKS_PER_SEC * 4.0f;
	glUniform1f(glGetUniformLocation(basicProgram->GetHandle(), "time"), time);

	// Draw cube
	glEnable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glEnable(GL_STENCIL_TEST);

		// Draw floor
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0xFF);
		glDepthMask(GL_FALSE);
		glClear(GL_STENCIL_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 36, 6);

		// Draw cube reflection
		glStencilFunc(GL_EQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDepthMask(GL_TRUE);

		model = glm::scale(glm::translate(model, glm::vec3(0, 0, -1)), glm::vec3(1, 1, -1));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		GLint uniColor = glGetUniformLocation(basicProgram->GetHandle(), "overrideColor");
		glUniform3f(uniColor, 0.3f, 0.3f, 0.3f);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

	glDisable(GL_STENCIL_TEST);

	// Swap buffers
	glfwSwapBuffers(window);
}

static void error_callback(int error, const char* description)
{
	std::cout << description << '\n';
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void resize_callback(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
	aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
}

int main(int argc, char* argv[]){

	for (size_t i = 0; i < argc; ++i){
		std::cout << argv[i] << '\n';
	}

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_REFRESH_RATE, 0);
	window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
	if (!window){
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// INIT GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
	};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	girlTex = new Texture2D();
	girlTex->LoadTexture("C:\\Users\\Scales\\Desktop\\DragonEngine\\Resources\\Images\\girl.bmp");
	girlTex->BindInSlot(GL_TEXTURE0);

	dargTex = new Texture2D();
	dargTex->LoadTexture("C:\\Users\\Scales\\Desktop\\DragonEngine\\Resources\\Images\\darg.bmp");
	dargTex->BindInSlot(GL_TEXTURE1);
	
	// BUFFERS
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	basicProgram = new GLProgram();
	basicProgram->LoadVertexShader("C:/Users/Scales/Desktop/DragonEngine/Dragon/Shaders/basicvertex.glsl");
	basicProgram->LoadFragmentShader("C:/Users/Scales/Desktop/DragonEngine/Dragon/Shaders/basicpixel.glsl");
	basicProgram->Link();
	basicProgram->Use();

	GLint strideSize = 8 * sizeof(float);

	GLint posAttrib = glGetAttribLocation(basicProgram->GetHandle(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, strideSize, 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(basicProgram->GetHandle(), "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, strideSize, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	GLint texAttrib = glGetAttribLocation(basicProgram->GetHandle(), "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, strideSize, (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(texAttrib);

	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, resize_callback);

	// Main event loop
	bool quit = false;
	int count = 0;
	while (!glfwWindowShouldClose(window))
	{
		count++;
		glfwPollEvents();

		glfwSetWindowTitle(window, std::to_string(count).c_str());

		Draw();
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}
