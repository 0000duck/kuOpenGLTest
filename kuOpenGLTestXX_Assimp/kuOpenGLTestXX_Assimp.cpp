#pragma once
#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opencv2/opencv.hpp>

#include "kuShaderHandler.h"
#include "kuModelObject.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

using namespace std;

#define pi 3.1415926

int main()
{
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

	kuModelObject	Model("1.stl");
	kuShaderHandler ModelShader;
	ModelShader.Load("VertexShader.vert", "FragmentShader.frag");
	ModelShader.Use();

	GLuint		ModelMatLoc, ViewMatLoc, ProjMatLoc;
	glm::mat4	ModelMat, ProjMat, ViewMat;

	ProjMat = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
	ProjMatLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ProjMat");
	glUniformMatrix4fv(ProjMatLoc, 1, GL_FALSE, glm::value_ptr(ProjMat));

	//ModelMat = glm::rotate(ModelMat, (GLfloat)pi * -90.0f / 180.0f,
	//					     glm::vec3(1.0f, 0.0f, 0.0f)); // mat, degree, axis. (use radians)

	ViewMat = glm::translate(ViewMat, glm::vec3(0.0f, 0.0f, -200.0f));		// 這邊放外參(世界座標系統轉到攝影機座標系統 Pc = E * Pw)(應該吧 需要實際測試)
	//ViewMat = glm::inverse(ViewMat);										// invert過來就是camera要動的量
	ViewMatLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ViewMat");
	glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(ViewMat));

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		double StartTime = glfwGetTime();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ModelMat = glm::mat4(1.0);
		ModelMat = glm::rotate(ModelMat, (GLfloat)pi * (float)glfwGetTime() * 10.0f / 180.0f,
							   glm::vec3(0.0f, 1.0f, 0.0f)); // mat, degree, axis. (use radians)
		ModelMatLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ModelMat");
		glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

		ModelShader.Use();
		Model.Draw(ModelShader);

		double EndTime = glfwGetTime();

		cout << "FPS: " << 1/(EndTime - StartTime) << endl;

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}