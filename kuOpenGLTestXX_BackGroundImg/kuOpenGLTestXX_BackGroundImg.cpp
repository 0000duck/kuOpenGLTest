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

GLFWwindow	*	kuGLInit(const char * title, int xRes, int yRes);
GLuint			CreateTexturebyImage(Mat Img);

GLfloat	vertices[]
= {
	// Positions          // Colors           // Texture Coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // Top Right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // Bottom Right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // Bottom Left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // Top Left  
};

GLuint indices[]
= { 0, 1, 3,
	1, 2, 3 };

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

int main()
{
	Mat					CamFrame;
	VideoCapture	*	CamCapture = NULL;

	GLFWwindow * window = kuGLInit("kuOpenGLTest", 640, 480);

	CamCapture = new VideoCapture(0);

	kuShaderHandler BGImgShaderHnadler;
	BGImgShaderHnadler.Load("BGImgVertexShader.vert", "BGImgFragmentShader.frag");

	GLuint VertexArray = 0;
	glGenVertexArrays(1, &VertexArray);
	GLuint VertexBuffer = 0;				// Vertex Buffer Object (VBO)
	glGenBuffers(1, &VertexBuffer);			// give an ID to vertex buffer
	GLuint ElementBuffer = 0;				// Element Buffer Object (EBO)
	glGenBuffers(1, &ElementBuffer);

	glBindVertexArray(VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer); // Bind buffer as array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(BGVertices), BGVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//GLuint TextureID = CreateTexturebyImage("TexImage.jpg");

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		CamCapture->read(CamFrame);

		imshow("CamFrame", CamFrame);

		BGImgShaderHnadler.Use();

		GLuint TextureID = CreateTexturebyImage(CamFrame);

		glBindTexture(GL_TEXTURE_2D, TextureID);

		glBindVertexArray(VertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

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
