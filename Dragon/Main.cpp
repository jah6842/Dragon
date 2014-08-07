
#define GLEW_STATIC

#include <ctime>
#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Texture2D.h"

const std::string resourceDir = "..\\..\\Resources";

Texture2D* girlTex;
Texture2D* dargTex;
GLuint shaderProgram;
SDL_Window* window;

void Draw(){
	// Reset state
	glUseProgram(0);
	// Clear the screen to black
	//glClear(GL_COLOR_BUFFER_BIT);

	// Draw
	glUseProgram(shaderProgram);
	girlTex->BindInSlot(GL_TEXTURE0);
	dargTex->BindInSlot(GL_TEXTURE1);

	glm::mat4 model;
	model = glm::rotate(
		model,
		(float)clock() / (float)CLOCKS_PER_SEC * 180.0f,
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	model = glm::rotate(
		model,
		(float)clock() / (float)CLOCKS_PER_SEC * 180.0f,
		glm::vec3(0.0f, 1.0f, 0.0f)
		);
	GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	float time = (float)clock() / (float)CLOCKS_PER_SEC * 4.0f;
	glUniform1f(glGetUniformLocation(shaderProgram, "time"), time);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Swap buffers
	SDL_GL_SwapWindow(window);
}

int eventFilter(void* userdata, SDL_Event *e)
{
	
	if (e->window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
	{
		glViewport(0, 0, e->window.data1, e->window.data2);
		SDL_UpdateWindowSurface(window);
		Draw();
	}
	return 1; // return 1 so all events are added to queue
}

int main(int argc, char* argv[]){

	for (size_t i = 0; i < argc; ++i){
		std::cout << argv[i] << '\n';
	}

	// INIT SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0);

#ifdef WIN32
	SDL_SetEventFilter(&eventFilter, NULL);
#endif

	// INIT GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	float vertices[] = {
		//  Position      Color             Texcoords
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
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

	// SHADERS
	std::string shaderPath = ("C:/Users/Scales/Desktop/DragonEngine/Dragon/Shaders/basicvertex.glsl");
	std::ifstream inV(shaderPath.c_str());
	shaderPath = ("C:/Users/Scales/Desktop/DragonEngine/Dragon/Shaders/basicpixel.glsl");
	std::ifstream inP(shaderPath.c_str());
	std::string vSrc((std::istreambuf_iterator<char>(inV)), std::istreambuf_iterator<char>());
	std::string pSrc((std::istreambuf_iterator<char>(inP)), std::istreambuf_iterator<char>());

	GLint vSrcLen = static_cast<GLint>(vSrc.length());
	GLint pSrcLen = static_cast<GLint>(pSrc.length());
	const GLchar* vertexSource = vSrc.c_str();
	const GLchar* fragmentSource = pSrc.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, &vSrcLen);
	glShaderSource(fragmentShader, 1, &fragmentSource, &pSrcLen);

	glCompileShader(fragmentShader);
	glCompileShader(vertexShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint strideSize = 7 * sizeof(float);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, strideSize, 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, strideSize, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, strideSize, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(texAttrib);
	
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	printf("%s\n", buffer);
	printf("%d\n", status);
	// END SHADER

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	SDL_Event windowEvent;

	// Main event loop
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) quit = true;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) quit = true;

			//if (windowEvent.type == SDL_WINDOWEVENT){
				//if (windowEvent.window.event == SDL_WINDOWEVENT_RESIZED){
					//glViewport(0, 0, windowEvent.window.data1, windowEvent.window.data2);
					//Draw();
					//glFlush();
					//continue;
				//}
			//}
		}

		Draw();

	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	SDL_GL_DeleteContext(context);
	SDL_Quit();

	return 0;
}
