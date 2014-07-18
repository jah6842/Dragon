
#define GLEW_STATIC

#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "SOIL\SOIL.h"
#include "FPSCounter.h"

int main(int argc, char* argv[]){

	// INIT SDL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL );
	SDL_GLContext context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0);

	// INIT GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	float vertices[] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};
	
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
	std::ifstream inV("../Dragon/Shaders/basicvertex.glsl");
	std::ifstream inP("../Dragon/Shaders/basicpixel.glsl");
	std::string vSrc((std::istreambuf_iterator<char>(inV)), std::istreambuf_iterator<char>());
	std::string pSrc((std::istreambuf_iterator<char>(inP)), std::istreambuf_iterator<char>());

	GLint vSrcLen = vSrc.length();
	GLint pSrcLen = pSrc.length();
	const GLchar* vertexSource = vSrc.c_str();
	const GLchar* fragmentSource = pSrc.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, &vSrcLen);
	glShaderSource(fragmentShader, 1, &fragmentSource, &pSrcLen);

	glCompileShader(fragmentShader);
	glCompileShader(vertexShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
		5 * sizeof(float), (void*)(2 * sizeof(float)));

	glEnableVertexAttribArray(posAttrib);
	glEnableVertexAttribArray(colAttrib);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	//printf("%s\n", buffer);
	//printf("%d\n", status);
	// END SHADER

	int numFrames = 0;
	uint32_t startTime = SDL_GetTicks();

	FPSCounter fps = FPSCounter();

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

		fps.Update();

		std::string fpsText = "OpenGL - " + std::to_string(fps.GetFPS());
		SDL_SetWindowTitle(window, fpsText.c_str());

		++numFrames;
		uint32_t elapsedMS = SDL_GetTicks() - startTime; // Time since start of loop
		if (elapsedMS) { // Skip this the first frame
			double elapsedSeconds = elapsedMS / 1000.0; // Convert to seconds
			double fpss = numFrames / elapsedSeconds; // FPS is Frames / Seconds
			std::string fpsText = "OpenGL - " + std::to_string(fpss);
			SDL_SetWindowTitle(window, fpsText.c_str());
		}

		Sleep(1);

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
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