#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "kuShaderHandler.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

int  main()
{
	GLfloat	TriangleVertexs[9] = {  0.0,  0.5, 0.0,
									0.5, -0.5, 0.0,
								   -0.5, -0.5, 0.0 };

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

	//glEnable(GL_MULTISAMPLE); // Enabled by default on some drivers, but not all so always enable to make sure

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	// get renderer string (graphic card)
	const GLubyte* version = glGetString(GL_VERSION);
	// version as a string (OpenGL supported version and graphic card driver version)
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	kuShaderHandler ShaderHnadler;
	ShaderHnadler.Load("VertexShader.vert", "FragmentShader.frag");
	ShaderHnadler.Use();

	GLuint VertexArray = 0;
	glGenVertexArrays(1, &VertexArray);
	GLuint VertexBuffer = 0;				// Vertex Buffer Object (VBO)
	glGenBuffers(1, &VertexBuffer);			// give an ID to vertex buffer

	glBindVertexArray(VertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer); // Bind buffer as array buffer
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), TriangleVertexs, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		GLfloat timeValue = glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
		GLint	vertexColorLocation = glGetUniformLocation(ShaderHnadler.ShaderProgramID, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VertexArray);
		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(5);
		glDrawArrays(GL_LINE_LOOP, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	system("pause");

	return 0;
}