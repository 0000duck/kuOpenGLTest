#include <iostream>

#include <opencv2/opencv.hpp>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kuShaderHandler.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "opencv_world310d.lib")

using namespace cv;
using namespace std;

#define pi 3.1415926

GLFWwindow	*	kuGLInit(const char * title, int xRes, int yRes);
GLuint			CreateTexturebyImage(Mat Img);

const GLfloat	CubeVertices[]
= {
	// Frontal Face
	-0.5f,  0.5f,  0.5f,			// 0:  Top Left  
	0.5f,  0.5f,  0.5f,			// 1:  Top Right
	0.5f, -0.5f,  0.5f,			// 2:  Bottom Right
	-0.5f, -0.5f,  0.5f,			// 3:  Bottom Left

									// Right Face
									-0.5f,  0.5f, -0.5f,			// 4:  Top Left
									-0.5f,  0.5f,  0.5f,			// 5:  Top Right
									-0.5f, -0.5f,  0.5f,			// 6:  Bottom Right
									-0.5f, -0.5f, -0.5f,			// 7:  Bottom Left

																	// Back face
																	0.5f,  0.5f, -0.5f,			// 8:  Top Left 
																	-0.5f,  0.5f, -0.5f,			// 9:  Top Right
																	-0.5f, -0.5f, -0.5f,			// 10: Bottom Right
																	0.5f, -0.5f, -0.5f,			// 11: Bottom Left

																								// Left Face
																								0.5f,  0.5f,  0.5f,			// 12: Top Left 
																								0.5f,  0.5f, -0.5f, 			// 13: Top Right
																								0.5f, -0.5f, -0.5f, 			// 14: Bottom Right
																								0.5f, -0.5f,  0.5f,  			// 15: Bottom Left

																																// Up Face
																																-0.5f,  0.5f, -0.5f,  		    // 16: Top Left 
																																0.5f,  0.5f, -0.5f,  		    // 17: Top Right
																																0.5f,  0.5f,  0.5f,  		    // 18: Bottom Right
																																-0.5f,  0.5f,  0.5f,  		    // 19: Bottom Left

																																								// Down Face
																																								-0.5f, -0.5f,  0.5f,			// 20: Top Left 
																																								0.5f, -0.5f,  0.5f,  			// 21: Top Right
																																								0.5f, -0.5f, -0.5f,    		// 22: Bottom Right
																																								-0.5f, -0.5f, -0.5f  			// 23: Bottom Left
};

const GLfloat   CubeTexCoords[]
= {
	0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f,
	0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f,
	0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f,
	0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f,
	0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f,
	0.0f, 0.0f,    1.0f, 0.0f,    1.0f, 1.0f,    0.0f, 1.0f
};

const GLuint    CubeIndices[]
= {
	// Frontal 
	0,  1,  3,	  1,  2,  3,
	// Right
	4,  5,  7,	  5,  6,  7,
	// Back
	8,  9, 11,	  9, 10, 11,
	// Left
	12, 13, 15,  13, 14, 15,
	// Up
	16, 17, 19,  17, 18, 19,
	// Down 
	20, 21, 23,  21, 22, 23
};

static const GLfloat BGVertices[]
= {
	1.0f,  1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 0.0f
};

static const GLfloat BGTexCoords[]
= {
	0.0
};

GLuint indices[]
= { 0, 1, 3,
1, 2, 3 };

int main()
{
	Mat					CamFrame;
	VideoCapture	*	CamCapture = NULL;

	GLFWwindow * window = kuGLInit("kuOpenGLTest", 640, 480);

	CamCapture = new VideoCapture(0);

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Clear buffers
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	delete CamCapture;

	return 0;
}

GLFWwindow * kuGLInit(const char * title, int xRes, int yRes)
{
	if (!glfwInit())
	{
		cout << "GLFW initialization failed." << endl;
		return NULL;
	}

	GLFWwindow * window = glfwCreateWindow(xRes, yRes, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		cout << "GLFW window creation failed." << endl;

		return NULL;
	}

	glfwMakeContextCurrent(window);

	// need to create OpenGL window before glew initialization.
	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		cout << "glewInit failed, aborting." << endl;
	}

	// Define the viewport dimensions
	glViewport(0, 0, 640, 480);

	// Setup OpenGL options (z-buffer)
	glEnable(GL_DEPTH_TEST);

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	// get renderer string (graphic card)
	const GLubyte* version = glGetString(GL_VERSION);
	// version as a string (OpenGL supported version and graphic card driver version)
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	return window;
}

GLuint CreateTexturebyImage(Mat Img)
{
	GLuint	texture;

	for (int i = 0; i < Img.cols; i++)
	{
		for (int j = 0; j < Img.rows; j++)
		{
			int		PixelIdx = Img.cols * j + i;
			uchar	temp;

			temp = Img.data[3 * PixelIdx];
			Img.data[3 * PixelIdx] = Img.data[3 * PixelIdx + 2];
			Img.data[3 * PixelIdx + 2] = temp;
		}
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Img.cols, Img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, Img.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}
