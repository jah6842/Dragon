
#define GLEW_STATIC

#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>
#include <string>
#include <algorithm>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include "SOIL/SOIL.h"

float* loadBMP(std::string path, uint32_t width, uint32_t height){
	float* _image = new float[width * height];
	std::fill(_image, _image + (width * height), 1.0f);
	return _image;
}

const std::string resourceDir = "..\\..\\Resources";

int main(int argc, char* argv[]){

	for (size_t i = 0; i < argc; ++i){
		std::cout << argv[i] << '\n';
	}

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

	//float vertices[] = {
	//	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // Top-left
	//	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // Top-right
	//	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
	//	-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
	//};

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

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height;
	std::string path = ("C:\\Users\\Scales\\Desktop\\DragonEngine\\Resources\\Images\\girl.bmp");
	std::cout << path.c_str() << '\n';
	unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	//SOIL_save_screenshot(path.c_str(), SOIL_SAVE_TYPE_BMP, 0, 0, 1920, 1080);
	
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
	std::string shaderPath = ("C:/Users/Scales/Desktop/DragonEngine/Resources/Shaders/basicvertex.glsl");
	std::ifstream inV(shaderPath.c_str());
	shaderPath = ("C:/Users/Scales/Desktop/DragonEngine/Resources/Shaders/basicpixel.glsl");
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

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

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
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) quit = true;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) quit = true;
		}

		// Clear the screen to black
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
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
