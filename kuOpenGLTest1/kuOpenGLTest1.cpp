#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

int  main()
{
	float	TriangleVertexs[9] =   {0.0, 0.5, 0.0, 0.5, -0.5, 0.0, -0.5, -0.5, 0.0};

	if (!glfwInit())
	{
		cout << "GLFW initialization failed." << endl;
		return 1;
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

	GLuint VertexBuffer = 0;  // Vertex Buffer Object (VBO)
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), TriangleVertexs, GL_STATIC_DRAW);
	
	while (!glfwWindowShouldClose(window))
	{
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();	// This function processes only those events that are already 
							// in the event queue and then returns immediately
	}
	
	system("pause");

	return 0;
}