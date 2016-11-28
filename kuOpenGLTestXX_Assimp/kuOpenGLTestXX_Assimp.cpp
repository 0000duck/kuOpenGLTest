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

glm::vec3		CameraPos   = glm::vec3(0.0f, 0.0f,  200.0f);
glm::vec3		CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3		CameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

GLfloat			yaw   = -90.0f;			// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
GLfloat			pitch =  0.0f;
GLfloat			LastXPos, LastYPos;

GLfloat			deltaTime  = 0.0f;
GLfloat			lastFrameT = 0.0f;

bool			keyPressArray[1024];

bool			firstMouse = true;


vector<int>		keySeq;					// 測試過用vector加上switch的方法寫好像不會比較好，留著當提醒吧

GLFWwindow	*	kuGLInit(const char * title, int xRes, int yRes);
void			key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void			mouse_callback(GLFWwindow * window, double xPos, double yPos);
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
	
	//ModelMat = glm::rotate(ModelMat, (GLfloat)pi * -90.0f / 180.0f,
	//					     glm::vec3(1.0f, 0.0f, 0.0f)); // mat, degree, axis. (use radians)

	//ViewMat = glm::translate(ViewMat, glm::vec3(0.0f, 0.0f, -200.0f));	// 這邊放外參(世界座標系統轉到攝影機座標系統 Pc = E * Pw)(應該吧 需要實際測試)
	//ViewMat = glm::inverse(ViewMat);										// invert過來就是camera要動的量
	ProjMatLoc   = glGetUniformLocation(ModelShader.ShaderProgramID, "ProjMat");
	ViewMatLoc   = glGetUniformLocation(ModelShader.ShaderProgramID, "ViewMat");
	ModelMatLoc  = glGetUniformLocation(ModelShader.ShaderProgramID, "ModelMat");
	CameraPosLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "CameraPos");

	glUniformMatrix4fv(ProjMatLoc, 1, GL_FALSE, glm::value_ptr(ProjMat));

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

		glUniformMatrix4fv(ViewMatLoc,  1, GL_FALSE, glm::value_ptr(ViewMat));
		glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
		glUniform3fv(CameraPosLoc, 1, glm::value_ptr(CameraPos));

		glDisable(GL_CULL_FACE);

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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);				// 顧名思義...大概只有位置資訊而沒有button事件資訊吧

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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

void mouse_callback(GLFWwindow * window, double xPos, double yPos)
{
	if (firstMouse)
	{
		LastXPos = xPos;
		LastYPos = yPos;

		firstMouse = false;
	}

	GLfloat xOffset = xPos - LastXPos;
	GLfloat yOffset = yPos - LastYPos;
	LastXPos = xPos;
	LastYPos = yPos;
	
	GLfloat sensitivity = 0.05;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw   += xOffset;
	pitch += yOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	CameraFront = glm::normalize(front);
}

void do_movement()
{
	// Camera controls
	GLfloat CameraSpeedX = 2.0f;	// 其實就是一個frame要動的量
	GLfloat CameraSpeedY = 2.0f;
	GLfloat CameraSpeedZ = 2.0f;

	GLfloat CameraSpeed = 50.0f * deltaTime;
	
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
		CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
		//CameraPos = glm::vec3(CameraPos.x -= CameraSpeed, CameraPos.y, CameraPos.z);
	}
	if (keyPressArray[GLFW_KEY_D])
	{
		CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp)) * CameraSpeed;
		//CameraPos = glm::vec3(CameraPos.x += CameraSpeed, CameraPos.y, CameraPos.z);

		// 不用下面的原因是....當加上mouse來做視角旋轉的時候，position根據CameraFront相對來算才是對的。根據直角坐標直接加會有問題
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
