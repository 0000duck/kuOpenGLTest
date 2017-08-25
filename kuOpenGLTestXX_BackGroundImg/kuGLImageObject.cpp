#include "kuGLImageObject.h"


kuGLImageObject::kuGLImageObject()
{
	CreateBuffers();
}

kuGLImageObject::kuGLImageObject(Mat Image)
{
	CreateBuffers();
	CreateTexture(Image);
}

kuGLImageObject::~kuGLImageObject()
{
	glDeleteTextures(1, &TextureID);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void kuGLImageObject::CreateBuffers()
{
	static const GLfloat BGVertices[]
		= {
		// position	  // texcoord
		1.0f,  1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 0.0f
	};

	GLuint indices[]
		= { 0, 1, 3,
		1, 2, 3 };

	VAO = 0;
	glGenVertexArrays(1, &VAO);
	VBO = 0;						// Vertex Buffer Object (VBO)
	glGenBuffers(1, &VBO);				// give an ID to vertex buffer
	EBO = 0;						// Element Buffer Object (EBO)
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Bind buffer as array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(BGVertices), BGVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Assign vertex position data
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Assign texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GLuint kuGLImageObject::CreateTexture(Mat Image)
{
	GLuint	texture;

	for (int i = 0; i < Image.cols; i++)
	{
		for (int j = 0; j < Image.rows; j++)
		{
			int		PixelIdx = Image.cols * j + i;
			uchar	temp;

			temp = Image.data[3 * PixelIdx];
			Image.data[3 * PixelIdx] = Image.data[3 * PixelIdx + 2];
			Image.data[3 * PixelIdx + 2] = temp;
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

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Image.cols, Image.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, Image.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

GLuint kuGLImageObject::GetTextureID()
{
	return TextureID;
}

void kuGLImageObject::Draw(kuShaderHandler shader, Mat Image)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);

	TextureID = CreateTexture(Image);

	shader.Use();

	glBindTexture(GL_TEXTURE_2D, TextureID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}
