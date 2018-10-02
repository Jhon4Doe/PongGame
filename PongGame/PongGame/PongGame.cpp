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

int main()
{

	if (!glfwInit())
		exit(EXIT_FAILURE);
	

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

	//// GLEW INIT ////
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);

	printf("%u\n", vertexBuffer);

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

	}

	glfwTerminate();
}