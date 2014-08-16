
#define GLEW_STATIC
//#define GLM_FORCE_RADIANS

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

#include "Utilities.h"
#include "Camera.h"
#include "Texture2D.h"
#include "GLProgram.h"

const std::string resourceDir = "..\\..\\Resources";

Texture2D* girlTex;
Texture2D* dargTex;
GLProgram* basicProgram;
GLFWwindow* window; 
Camera* camera;

int windowWidth = 800;
int windowHeight = 600;
float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

void Draw(float dt){
	// Reset state
	glUseProgram(0);
	// Clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw
	basicProgram->Use();
	girlTex->BindInSlot(GL_TEXTURE0);
	dargTex->BindInSlot(GL_TEXTURE1);

	glm::mat4 defaultModel = glm::mat4(1.0f);
	defaultModel = glm::rotate(defaultModel,
		270.0f,
		glm::vec3(1.0f, 0.0f, 0.0f));

	GLint uniModel = glGetUniformLocation(basicProgram->GetHandle(), "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(defaultModel));

	glm::mat4 view = camera->GetViewMatrix();
	GLint uniView = glGetUniformLocation(basicProgram->GetHandle(), "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = camera->GetProjectionMatrix();
	GLint uniProj = glGetUniformLocation(basicProgram->GetHandle(), "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	float time = (float)clock() / (float)CLOCKS_PER_SEC * 4.0f;
	glUniform1f(glGetUniformLocation(basicProgram->GetHandle(), "time"), time);

	for (size_t i = 0; i < 20; i++){
		for (size_t j = 0; j < 20; j++){
			for (size_t k = 0; k < 20; k++){
				glm::mat4 model = defaultModel;
				model = glm::translate(model, glm::vec3(i * 2, j * 2, k * 2));
				glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

				// Draw cube
				glEnable(GL_DEPTH_TEST);
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

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

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		camera->transform->Move(-1.0f, 0, 0);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		camera->transform->Move(1.0f, 0, 0);
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		camera->transform->Move(0.0f, 0, -1);
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		camera->transform->Move(0.0f, 0, 1);
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

	camera = new Camera(windowWidth, windowHeight);

	// Main event loop
	bool quit = false;

	double t = 0.0;
	const double dt = 0.01;

	double currentTime = dgnGetHighResTimestamp();
	double accumulator = 0.0;

	//State previous;
	//State current;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		double newTime = dgnGetHighResTimestamp();
		double frameTime = newTime - currentTime;
		if (frameTime > 0.25)
			frameTime = 0.25;
		currentTime = newTime;
		glfwSetWindowTitle(window, std::to_string(currentTime).c_str());

		accumulator += frameTime;

		while (accumulator >= dt)
		{
			//previousState = currentState;
			//integrate(currentState, t, dt);
			t += dt;
			accumulator -= dt;
		}

		const double alpha = accumulator / dt;

		//State state = (currentState * alpha) + (previousState * (1.0 - alpha));

		camera->Update();

		Draw(/*state*/dt);
	}

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}
