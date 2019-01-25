#version 410 core
in	vec3 ourColor;
in	vec2 TexCoord;
in  vec3 OriPos;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	color = texture(ourTexture, TexCoord);
	
	if (OriPos.x > 128.5 && OriPos.y <128.5)
	{
		color.a = 0.85;
	}
	else
	{
		color.a = 0;	
	}
	
}