#pragma once
#include <iostream>
#include <time.h>

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
#pragma comment(lib, "opencv_world310d.lib")

using namespace std;
using namespace cv;

#define pi			3.1415926
#define WndWidth	1024
#define WndHeight	768

glm::vec3		CameraPos   = glm::vec3(0.0f, 0.0f, 200.0f);
glm::vec3		CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3		CameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

GLfloat			yaw = -90.0f;			// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
GLfloat			pitch = 0.0f;
GLfloat			LastXPos, LastYPos;

GLfloat			deltaTime = 0.0f;
GLfloat			lastFrameT = 0.0f;

bool			keyPressArray[1024];

bool			firstMouse = true;


vector<int>		keySeq;					// ���չL��vector�[�Wswitch����k�g�n�����|����n�A�d�۷����a

GLFWwindow	*	kuGLInit(const char * title, int xRes, int yRes);
void			key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void			mouse_callback(GLFWwindow * window, double xPos, double yPos);
void			do_movement();

GLuint			CreateTexturebyImage(Mat Img);
void			DrawImage(Mat Img, kuShaderHandler ImgShader);

GLfloat	AxiImgVertices[]
= {
	256.99f,	0.0f, 168.5f,	1.0f, 0.0f,
	256.99f, 256.99f, 168.5f,	1.0f, 1.0f,
	   0.0f, 256.99f, 168.5f,	0.0f, 1.0f,
	   0.0f,	0.0f, 168.5f,	0.0f, 0.0f
};

GLfloat	SagImgVertices[]		// need to replace
= {
	128.5f,	0.0f, 168.5f,	1.0f, 0.0f,
	128.5f, 256.99f, 168.5f,	1.0f, 1.0f,
	128.5f,	 256.99f, 168.5f,	0.0f, 1.0f,
	128.5f,		0.0f, 168.5f,	0.0f, 0.0f
};

GLfloat	CorImgVertices[]		// need to replace
= {
	256.99f,	0.0f, 168.5f,	1.0f, 0.0f,
	256.99f, 256.99f, 168.5f,	1.0f, 1.0f,
	0.0f,	 256.99f, 168.5f,	0.0f, 1.0f,
	0.0f,		0.0f, 168.5f,	0.0f, 0.0f
};

GLuint ImgIndices[]
= { 0, 1, 3,
	1, 2, 3 };

int main()
{
	GLFWwindow * window = kuGLInit("kuOpenGLTest", WndWidth, WndHeight);

	kuModelObject	FaceModel("kuFace_7d5wf_SG.obj");
	kuModelObject	BoneModel("kuBone_7d5wf_SG.obj");
	kuShaderHandler ModelShader;
	kuShaderHandler ImgShader;
	ImgShader.Load("ImgVertexShader.vert", "ImgFragmentShader.frag");
	ModelShader.Load("VertexShader.vert", "FragmentShader.frag");

	GLuint		CameraPosLoc;
	GLuint		ModelMatLoc, ViewMatLoc, ProjMatLoc;
	GLuint		ObjColorLoc;

	GLuint		ImgModelMatLoc, ImgViewMatLoc, ImgProjMatLoc, TransCT2ModelLoc;

	glm::mat4	ModelMat, ProjMat, ViewMat;				 // model and slice uses same matrices
	glm::mat4	TransCT2Model;

	ProjMat = glm::perspective(45.0f, (GLfloat)640 / (GLfloat)480, 0.1f, 1000.0f);

	ModelMat = glm::rotate(ModelMat, (GLfloat)pi * -90.0f / 180.0f,
						   glm::vec3(1.0f, 0.0f, 0.0f)); // mat, degree, axis. (use radians)

	TransCT2Model = glm::translate(TransCT2Model, glm::vec3(-128.249, -281.249, -287));

	//ViewMat = glm::translate(ViewMat, glm::vec3(0.0f, 0.0f, -200.0f));		// �o���~��(�@�ɮy�Шt�������v���y�Шt�� Pc = E * Pw)(���ӧa �ݭn��ڴ���)
	//ViewMat = glm::inverse(ViewMat);										// invert�L�ӴN�Ocamera�n�ʪ��q
	
	ProjMatLoc   = glGetUniformLocation(ModelShader.ShaderProgramID, "ProjMat");
	ViewMatLoc   = glGetUniformLocation(ModelShader.ShaderProgramID, "ViewMat");
	ModelMatLoc  = glGetUniformLocation(ModelShader.ShaderProgramID, "ModelMat");
	CameraPosLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "CameraPos");

	ImgProjMatLoc    = glGetUniformLocation(ImgShader.ShaderProgramID, "ProjMat");
	ImgViewMatLoc    = glGetUniformLocation(ImgShader.ShaderProgramID, "ViewMat");
	ImgModelMatLoc   = glGetUniformLocation(ImgShader.ShaderProgramID, "ModelMat");
	TransCT2ModelLoc = glGetUniformLocation(ImgShader.ShaderProgramID, "TransCT2Model");

	ObjColorLoc = glGetUniformLocation(ModelShader.ShaderProgramID, "ObjColor");

	/*GLfloat FaceColorVec[4] = { 0.745f, 0.447f, 0.235f, 0.5 };
	GLfloat BoneColorVec[4] = { 1.0f, 1.0f, 1.0f, 1.0 };*/

	GLfloat FaceColorVec[4] = { 0.745f, 0.447f, 0.235f, 0.5f };
	GLfloat BoneColorVec[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Mat AxiImg = imread("HSIEH-CHUNG-HUNG-OrthoSlice.to-byte_axial.bmp", 1);
	Mat SagImg = imread("HSIEH-CHUNG-HUNG-OrthoSlice.to-byte_sagittal.bmp", 1);
	Mat CorImg = imread("HSIEH-CHUNG-HUNG-OrthoSlice.to-byte_conoral.bmp", 1);

	//imshow("NEW SHIT", AxiImg);

	while (!glfwWindowShouldClose(window))
	{
		GLfloat currFrameT = glfwGetTime();
		deltaTime = currFrameT - lastFrameT;
		lastFrameT = currFrameT;

		glfwPollEvents();
		do_movement();

		double StartTime = glfwGetTime();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		
		ViewMat = glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

		ModelShader.Use();
		glUniformMatrix4fv(ProjMatLoc, 1, GL_FALSE, glm::value_ptr(ProjMat));
		glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(ViewMat));
		glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
		glUniform3fv(CameraPosLoc, 1, glm::value_ptr(CameraPos));

		// Inner object first.
		glUniform4fv(ObjColorLoc, 1, BoneColorVec);
		BoneModel.Draw(ModelShader);

		// Draw outside object latter
		glUniform4fv(ObjColorLoc, 1, FaceColorVec);
		FaceModel.Draw(ModelShader);

		glDisable(GL_DEPTH_TEST);

		ImgShader.Use();
		glUniformMatrix4fv(ImgProjMatLoc, 1, GL_FALSE, glm::value_ptr(ProjMat));
		glUniformMatrix4fv(ImgViewMatLoc, 1, GL_FALSE, glm::value_ptr(ViewMat));
		glUniformMatrix4fv(ImgModelMatLoc, 1, GL_FALSE, glm::value_ptr(ModelMat));
		glUniformMatrix4fv(TransCT2ModelLoc, 1, GL_FALSE, glm::value_ptr(TransCT2Model));
		DrawImage(AxiImg, ImgShader);

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
	glfwSetCursorPosCallback(window, mouse_callback);				// �U�W��q...�j���u����m��T�ӨS��button�ƥ��T�a

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
	//glEnable(GL_DEPTH_TEST);

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

	yaw += xOffset;
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
	GLfloat CameraSpeedX = 2.0f;	// ���N�O�@��frame�n�ʪ��q
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

		// ���ΤU������]�O....��[�Wmouse�Ӱ��������઺�ɭԡAposition�ھ�CameraFront�۹�Ӻ�~�O�諸�C�ھڪ������Ъ����[�|�����D
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

void DrawImage(Mat Img, kuShaderHandler ImgShader)
{
	time_t StartTime = clock();

	GLuint ImgVertexArray = 0;
	glGenVertexArrays(1, &ImgVertexArray);
	GLuint ImgVertexBuffer = 0;						// Vertex Buffer Object (VBO)
	glGenBuffers(1, &ImgVertexBuffer);				// give an ID to vertex buffer
	GLuint ImgElementBuffer = 0;					// Element Buffer Object (EBO)
	glGenBuffers(1, &ImgElementBuffer);

	glBindVertexArray(ImgVertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, ImgVertexBuffer);  // Bind buffer as array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(AxiImgVertices), AxiImgVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ImgElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ImgIndices), ImgIndices, GL_STATIC_DRAW);

	// Assign vertex position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Assign texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	time_t EndTime = clock();

	//cout << "time: " << difftime(EndTime, StartTime) << endl;

	GLuint ImgTextureID = CreateTexturebyImage(Img);

	ImgShader.Use();

	glBindTexture(GL_TEXTURE_2D, ImgTextureID);

	glBindVertexArray(ImgVertexArray);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteTextures(1, &ImgTextureID);

	glDeleteVertexArrays(1, &ImgVertexArray);
	glDeleteBuffers(1, &ImgVertexBuffer);
	glDeleteBuffers(1, &ImgElementBuffer);
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