#pragma once
#include <opencv2/opencv.hpp>
#include <GLEW/glew.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kuShaderHandler.h"

using namespace cv;

class kuGLImageObject
{
private:
	GLuint		VAO, VBO, EBO;
	GLuint		TextureID;

	void		CreateBuffers();
	GLuint		CreateTexture(Mat Image);

public:
	
	kuGLImageObject();
	kuGLImageObject(Mat Image);
	~kuGLImageObject();

	GLuint GetTextureID();
	void Draw(kuShaderHandler shader, Mat Image);
};

