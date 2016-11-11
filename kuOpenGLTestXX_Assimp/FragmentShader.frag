#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

//uniform vec4 ambient;
//uniform vec4 diffuse;
//uniform vec4 specular;

void main()
{    
    //color = vec4(texture(texture_diffuse1, TexCoords));
	color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
}