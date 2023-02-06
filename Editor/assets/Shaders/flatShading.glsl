!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;
layout(location = 1) in vec3 vertexCol;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexNorml;

smooth out vec3 frgColor;

struct Materials {
	vec3 m_ambient;
	vec3 m_diffuse;
	vec3 m_specular;
	float m_shininess;
};

uniform mat4 transMat;
uniform mat4 viewProj;
uniform mat3 normalMat;

uniform Materials material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	vec3 objPos = vec3(transMat * vec4(vertexPts, 1.f));
	gl_Position = viewProj * vec4(objPos, 1.f);
	vec3 norm = normalize(normalMat * vertexNorml);

	vec3 lightDir = normalize(lightPos - objPos);

	vec3 diffFac = max(dot(norm, lightDir), 0.0f) * material.m_diffuse;

	vec3 specFac = pow(max(dot(reflect(-lightDir, norm), normalize(camPos - objPos)), 0.f), material.m_shininess) * material.m_specular;

	frgColor = (material.m_ambient + diffFac + specFac) * lightColor;
}

!#!shader fragment
#version 450 core

smooth in vec3 frgColor;

void main() {

	gl_FragColor = vec4(frgColor, 1.f);

}