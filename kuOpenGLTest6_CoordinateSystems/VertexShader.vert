#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
								   
void main()
{
	gl_Position = ProjMat * ViewMat * ModelMat * vec4(position, 1.0);
	ourColor = color;
	TexCoord = texCoord;
}