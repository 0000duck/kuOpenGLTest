#version 410 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;
		
uniform mat4 ProjMat;
uniform mat4 transform;
								   
void main()
{
	gl_Position = vec4(position, 0, 1.0);
	TexCoord = texCoord;
}