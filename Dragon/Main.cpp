
#define GLEW_STATIC

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

int main(int argc, char* argv[]){

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	float vertices[] = {
		0.0f, 0.5f, // Vertex 1 (X, Y)
		0.5f, -0.5f, // Vertex 2 (X, Y)
		-0.5f, -0.5f  // Vertex 3 (X, Y)
	};

	const GLchar* vertexSource =
		"#version 440 core\n \
		in vec2 position; \
		void main() { \
		   gl_Position = vec4(position, 0.0, 1.0); \
		}";
	const GLchar* fragmentSource =
		"#version 440 core\n \
		uniform vec3 triangleColor; \
		out vec4 outColor; \
		void main() { \
		   outColor = vec4(triangleColor, 1.0); \
		}";

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	printf("%s/n", argv[0]);
	//const GLchar* vertexSource = "C:/Users/Scales/Desktop/DragonEngine/Dragon/Dragon/Dragon/Shaders/basicvertex.glsl";
	//const GLchar* fragmentSource = "C:/Users/Scales/Desktop/DragonEngine/Dragon/Dragon/Dragon/Shaders/basicpixel.glsl";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	glCompileShader(fragmentShader);
	glCompileShader(vertexShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");

	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(posAttrib);

	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	printf("%s\n", buffer);
	printf("%d\n", status);
	
	int numFrames = 0;
	uint32_t startTime = SDL_GetTicks();

	SDL_Event windowEvent;
	while (true)
	{
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT) break;
			if (windowEvent.type == SDL_KEYUP &&
				windowEvent.key.keysym.sym == SDLK_ESCAPE) break;
		}

		++numFrames;
		uint32_t elapsedMS = SDL_GetTicks() - startTime; // Time since start of loop
		if (elapsedMS) { // Skip this the first frame
			double elapsedSeconds = elapsedMS / 1000.0; // Convert to seconds
			double fps = numFrames / elapsedSeconds; // FPS is Frames / Seconds
			std::string fpsText = "OpenGL - " + std::to_string(fps);
			SDL_SetWindowTitle(window, fpsText.c_str());
		}

		int32_t randX = rand() % 424 + 600;
		int32_t randY = rand() % 168 + 600;

		float red = sin(SDL_GetTicks());
		printf("%d\n", red);

		glUniform3f(uniColor, red, 0.0f, 0.0f);

		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//SDL_SetWindowSize(window, randX, randY);
		

		glDrawArrays(GL_TRIANGLES, 0, 3);

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