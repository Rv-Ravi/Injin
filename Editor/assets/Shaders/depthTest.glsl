!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;
layout(location = 1) in vec3 vertexCol;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexNorml;

smooth out vec3 objPos;
smooth out vec3 vNorml;
smooth out vec2 tCord;

uniform mat4 transMat;
uniform mat4 viewProj;
uniform mat3 normalMat;

void main()
{
	objPos = vec3(transMat * vec4(vertexPts, 1.f));
	gl_Position = viewProj * vec4(objPos, 1.f);
	vNorml = normalMat * vertexNorml;
	tCord = texCoord;
}

!#!shader fragment
#version 450 core

struct Materials {
	sampler2D m_diffuse;
	sampler2D m_specular;
	float m_shininess;
};



smooth in vec3 objPos;
smooth in vec3 vNorml;
smooth in vec2 tCord;

uniform Materials material;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

float near = 0.01f;
float far = 10.f;

void main() {

	vec3 norm = normalize(vNorml);
	vec3 lightDir = normalize(lightPos - objPos);

	vec4 diffFac = max(dot(norm, lightDir), 0.0f) * texture(material.m_diffuse, tCord);

	vec4 specFac = pow(max(dot(reflect(-lightDir, norm), normalize(camPos - objPos)), 0.f), material.m_shininess) * texture(material.m_specular, tCord);

	float depth = (2 * far * near) / (far + near - (gl_FragCoord.z * 2.f - 1.f) * (far - near));

	float linearDepth = 1.f - (depth - near) / (far - near);

	gl_FragColor = vec4(linearDepth * lightColor, 1.f);
}