#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <opencv2/opencv.hpp>

#pragma comment(lib, "glfw3")
#pragma comment(lib, "glew32")
#pragma comment(lib, "opengl32")
#pragma comment(lib, "opencv_world310d")

using namespace cv;
using namespace std;

GLFWwindow	*	GLWnd;

Mat				TextureImg;

GLuint			loadBMP_custom(const char * imagepath);
GLFWwindow	*	initOpenGL();


GLfloat vertices[] = {
	0.5f,  0.5f, 0.0f,  // Top Right
	0.5f, -0.5f, 0.0f,  // Bottom Right
   -0.5f, -0.5f, 0.0f,  // Bottom Left
   -0.5f,  0.5f, 0.0f   // Top Left 
};

void main()
{
	GLWnd = initOpenGL();

	while (!glfwWindowShouldClose(GLWnd))
	{
		glClearColor(0.1f, 0.5f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(GLWnd);
		glfwPollEvents();	// This function processes only those events that are already 
							// in the event queue and then returns immediately
	}

	system("pause");
}

GLFWwindow * initOpenGL()
{
	if (!glfwInit())
	{
		cout << "GLFW initialization failed." << endl;
		return nullptr;
	}

	GLFWwindow * window = glfwCreateWindow(640, 480, "GLFW window", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cout << "GLFW window creation failed." << endl;
	}

	glfwMakeContextCurrent(window);

	// need to create OpenGL window before glew initialization.
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		cout << "glewInit failed, aborting." << endl;
	}

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	// get renderer string (graphic card)
	const GLubyte* version = glGetString(GL_VERSION);
	// version as a string (OpenGL supported version and graphic card driver version)
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	return window;
}

GLuint loadBMP_custom(const char * imagepath)
{
	
	return GLuint();
}
