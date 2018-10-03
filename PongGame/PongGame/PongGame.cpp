// PongGame.cpp: define el punto de entrada de la aplicación de consola.

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <thread>

#include <iostream>
using namespace std;


//static void error_callback(int error, const char* description)
//{
//	fprintf(stderr, "Error: %s\n", description);
//}
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GLFW_TRUE);
//}
//static void glfwError(int id, const char* description)
//{
//	std::cout << description << std::endl;
//}

const char* vertexSource = R"glsl(
    #version 330 core

    in vec2 position;
	in vec3 color;

	out vec3 Color;

	void main()
	{
		Color = color;
		gl_Position = vec4(position, 0.0, 1.0);
	}
)glsl";

const char* fragmentSource = R"glsl(
	#version 330 core
	
	in vec3 Color;

	out vec4 outColor;

	void main()
	{
		outColor = vec4(Color, 1.0);
	}
)glsl";

int main()
{

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr); // Windowed

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);

	printf("Supported GLSL version is %s.\n", (char *)glGetString(GL_SHADING_LANGUAGE_VERSION));


	//// GLEW INIT ////
	glewExperimental = GL_TRUE;
	glewInit();

	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);	//Vertex Array Object
	glBindVertexArray(vao);
	printf("ArrayVertexObject: %u\n", vao);

	// Create Vertex Buffer Object
	GLuint vbo;
	glGenBuffers(1, &vbo); // Generate 1 buffer: Vertex Buffer Object (VBO):
	printf("BufferVertexObject: %u\n", vbo);
	

	float vertices[] = {
		0.0f,  0.5f, 1.0f, 0.0f, 0.0f, // Vertex 1: Red
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Vertex 2: Green
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
	};

	glBindBuffer(GL_ARRAY_BUFFER, vbo);	// Cargamos el espacio del array buffer a la GPU

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //ponemos los vertices en el array

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	//// Error detection in shader
	GLint statusVertex;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &statusVertex);

	if (statusVertex != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
		cout << "Error de compilación del shader vertex: " << buffer << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLint statusFragment;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &statusFragment);

	if (statusFragment != GL_TRUE) {
		char buffer[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
		cout << "Error de compilación del shader Fragment: " << buffer << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	//// Shader y fragment desconectados aun... los juntamos ahora
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Cambiar para multiples buffers
	glBindFragDataLocation(shaderProgram, 0, "outColor");

	//Enlazar el programa con los shaders
	glLinkProgram(shaderProgram);

	//Usar finalmente juntamos los shaders en el programa
	glUseProgram(shaderProgram);

	//GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	//glEnableVertexAttribArray(posAttrib);
	//glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,	5 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//// Draw a triangle from the 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);

		GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

		float time = (float)glfwGetTime();
		glUniform3f(uniColor, (sin(time * 2.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

		glfwPollEvents();
		glfwSwapBuffers(window);

		//// Clear the screen to black
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		////// Draw a triangle from the 3 vertices
		//glDrawArrays(GL_TRIANGLES, 0, 3);


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		//std::this_thread::sleep_for(std::chrono::milliseconds(100));

	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();

	return 0;
}