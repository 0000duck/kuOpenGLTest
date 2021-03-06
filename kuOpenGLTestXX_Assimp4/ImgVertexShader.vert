#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;
out vec3 OriPos;

uniform mat4 TransCT2Model;
uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;
								   
void main()
{
	OriPos.x = position.x;
	OriPos.y = position.y;
	OriPos.z = position.z;

	gl_Position = ProjMat * ViewMat * ModelMat * TransCT2Model * vec4(position, 1.0);
	TexCoord = texCoord;	
}