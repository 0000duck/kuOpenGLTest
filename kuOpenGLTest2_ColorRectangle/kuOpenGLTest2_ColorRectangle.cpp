#pragma once
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

GLFWwindow * GLWnd;

GLFWwindow * initOpenGL();

// Shaders
const GLchar* vertexShaderSource = "#version 410 core\n"
								   "in vec3 position;\n"
								   "in vec3 color;\n"
								   "out vec3 vertexColor;\n"
								   "void main()\n"
								   "{\n"
								   "gl_Position = vec4(position, 1.0);\n"
								   "vertexColor = color;\n"
								   "}\0";
const GLchar* fragmentShaderSource = "#version 410 core\n"
									 "in vec3 vertexColor;\n"
									 "out vec4 outColor;\n"
									 "void main()\n"
									 "{\n"
									 "outColor = vec4(vertexColor, 1.0);\n"
									 "}\n\0";

GLfloat vertices[] = {
	// position            // color
	-0.5f,  0.5f, 0.0f,	   1.0f, 0.0f, 0.0f,		// Top Left 
	 0.5f,  0.5f, 0.0f,	   0.0f, 1.0f, 0.0f,		// Top Right
	 0.5f, -0.5f, 0.0f,	   0.0f, 0.0f, 0.1f,		// Bottom Right
	-0.5f, -0.5f, 0.0f,	   1.0f, 1.0f, 1.0f			// Bottom Left	
};
GLuint indices[] = {
	0, 1, 2,   // First Triangle
	2, 3, 0    // Second Triangle
};

void main()
{
	GLWnd = initOpenGL();

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GLuint VertexShader;
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(VertexShader);

	GLuint FragmentShader;
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(FragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint ShaderProgram;
	ShaderProgram = glCreateProgram();
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	glBindFragDataLocation(ShaderProgram, 0, "outColor");
	glLinkProgram(ShaderProgram);
	glUseProgram(ShaderProgram);

	GLint posAttrib = glGetAttribLocation(ShaderProgram, "position");	// 如果在shader code裡面，position前面有加
																		// layout (location=0)
																		// 好像可以不用像這樣get，0就會代表position 
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);

	GLint colorAttrib = glGetAttribLocation(ShaderProgram, "color");
	glEnableVertexAttribArray(colorAttrib);
	glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(GLWnd))
	{
		glClearColor(0.1f, 0.5f, 0.3f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(GLWnd);
		glfwPollEvents();	// This function processes only those events that are already 
							// in the event queue and then returns immediately
	}

	system("pause");
}

GLFWwindow * initOpenGL()
{
	if (!glfwInit())
	{
		cout << "GLFW initialization failed." << endl;
		return nullptr;
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

	return window;
}
