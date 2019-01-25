#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};

in vec3 FragPos;
in vec3 Normal;
in float ZCoord;
in vec3 OriPos;

out vec4 color;

uniform Material material;
uniform vec3 CameraPos;
uniform vec4 ObjColor;

void main()
{    
	vec3 LightColor = vec3(1.0, 1.0, 1.0);
	vec3 LightPos   = CameraPos;
	vec3 viewPos    = CameraPos;

	// Ambient
	vec3 ambient = LightColor * material.ambient *2;

	// Diffuse
	vec3  norm     = normalize(Normal);
	vec3  lightDir = normalize(LightPos - FragPos);
	float diff     = max(dot(norm, lightDir), 0.0);
	vec3  diffuse  = diff * LightColor * material.diffuse;

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0);
	//vec3 specular = spec * LightColor * material.specular;
	vec3 specular = vec3(0.0, 0.0, 0.0);

	/*
	if (OriPos.z < -118.5)
	{
					 // lighting color					   // object color
		color = vec4(ambient + diffuse + specular, 1.0f) * ObjColor;
		//color = vec4(0.6f, 0.6f, 0.6f, 1.0f);
	}
	else
	{
		color = vec4(ambient + diffuse + specular, 0.0f) * ObjColor;
	}
	*/

	if (OriPos.z > -118.5 && OriPos.x > 0 && OriPos.y < -152.754)
	{
		color = vec4(ambient + diffuse + specular, 0.0f) * ObjColor;
	}
	else
	{
		// lighting color					   // object color
		color = vec4(ambient + diffuse + specular, 1.0f) * ObjColor;
	}
}