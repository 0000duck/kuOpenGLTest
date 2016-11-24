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

#define pi			3.1415926
#define WndWidth	1024
#define WndHeight	768

glm::vec3 CameraPos   = glm::vec3(0.0f, 0.0f,  200.0f);
glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 CameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

GLfloat			deltaTime  = 0.0f;
GLfloat			lastFrameT = 0.0f;

bool			keyPressArray[1024];

vector<int>		keySeq;					// 測試過用vector加上switch的方法寫好像不會比較好，留著當提醒吧

GLFWwindow	*	kuGLInit(const char * title, int xRes, int yRes);
void			key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void			do_movement();

int main()
{
	GLFWwindow * window = kuGLInit("kuOpenGLTest", WndWidth, WndHeight);

	kuModelObject	Model("LAI-WEN-HSIEN-big.surf.stl");
	kuShaderHandler ModelShader;
	ModelShader.Load("VertexShader.vert", "FragmentShader.frag");
	ModelShader.Use();

	GLuint		CameraPosLoc;
	GLuint		ModelMatLoc, ViewMatLoc, ProjMatLoc;
	glm::mat4	ModelMat, ProjMat, ViewMat;

	ProjMat = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);
	ProjMatLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ProjMat");
	glUniformMatrix4fv(ProjMatLoc, 1, GL_FALSE, glm::value_ptr(ProjMat));

	//ModelMat = glm::rotate(ModelMat, (GLfloat)pi * -90.0f / 180.0f,
	//					     glm::vec3(1.0f, 0.0f, 0.0f)); // mat, degree, axis. (use radians)

	//ViewMat = glm::translate(ViewMat, glm::vec3(0.0f, 0.0f, -200.0f));	// 這邊放外參(世界座標系統轉到攝影機座標系統 Pc = E * Pw)(應該吧 需要實際測試)
	//ViewMat = glm::inverse(ViewMat);										// invert過來就是camera要動的量

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currFrameT = glfwGetTime();
		deltaTime  = currFrameT - lastFrameT;
		lastFrameT = currFrameT;

		glfwPollEvents();
		do_movement();
		keySeq.clear();

		double StartTime = glfwGetTime();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ModelShader.Use();

		ViewMat = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
		ViewMatLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ViewMat");
		glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(ViewMat));

		//ModelMat = glm::mat4(1.0);
		//ModelMat = glm::rotate(ModelMat, (GLfloat)pi * (float)glfwGetTime() * 10.0f / 180.0f,
		//					   glm::vec3(0.0f, 1.0f, 0.0f)); // mat, degree, axis. (use radians)
		ModelMatLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ModelMat");
		glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));

		CameraPosLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "CameraPos");
		glUniform3fv(CameraPosLoc, 1, glm::value_ptr(CameraPos));

		Model.Draw(ModelShader);

		double EndTime = glfwGetTime();

		//cout << "FPS: " << 1/(EndTime - StartTime) << endl;

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

	glfwSetKeyCallback(window, key_callback);

	// need to create OpenGL window before glew initialization.
	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		cout << "glewInit failed, aborting." << endl;
	}

	// Define the viewport dimensions
	glViewport(0, 0, xRes, yRes);

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

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keyPressArray[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyPressArray[key] = false;
		}		
	}

	if (key == GLFW_KEY_C)
	{
		cout << "CameraPos: (" << CameraPos.x << ", " << CameraPos.y << ", " << CameraPos.z << ")" << endl;
	}
}

void do_movement()
{
	// Camera controls
	GLfloat CameraSpeedX = 2.0f;	// 其實就是一個frame要動的量
	GLfloat CameraSpeedY = 2.0f;
	GLfloat CameraSpeedZ = 2.0f;

	GLfloat CameraSpeed = 100.0f * deltaTime;
	
	if (keyPressArray[GLFW_KEY_W])
	{
		CameraPos += CameraSpeed * CameraFront;
	}	
	if (keyPressArray[GLFW_KEY_S])
	{
		CameraPos -= CameraSpeed * CameraFront;
	}
	if (keyPressArray[GLFW_KEY_A])
	{
		//CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * cameraSpeed;
		CameraPos = glm::vec3(CameraPos.x -= CameraSpeed, CameraPos.y, CameraPos.z);
	}
	if (keyPressArray[GLFW_KEY_D])
	{
		//CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * cameraSpeed;
		CameraPos = glm::vec3(CameraPos.x += CameraSpeed, CameraPos.y, CameraPos.z);
	}
	if (keyPressArray[GLFW_KEY_Z])
	{
		CameraPos = glm::vec3(CameraPos.x, CameraPos.y += CameraSpeed, CameraPos.z);
	}
	if (keyPressArray[GLFW_KEY_X])
	{
		CameraPos = glm::vec3(CameraPos.x, CameraPos.y -= CameraSpeed, CameraPos.z);
	}
}
