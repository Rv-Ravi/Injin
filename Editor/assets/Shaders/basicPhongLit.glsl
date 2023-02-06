!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;
layout(location = 1) in vec3 vertexCol;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexNorml;

smooth out vec3 vColor;
smooth out vec2 tCord;
smooth out vec3 objPos;
smooth out vec3 vNorml;

uniform mat4 transMat;
uniform mat4 viewProj;
uniform mat3 normalMat;

void main()
{
	objPos = vec3(transMat * vec4(vertexPts, 1.f));
	gl_Position = viewProj * vec4(objPos, 1.f);
	vColor = vertexCol;
	tCord = texCoord;
	vNorml = normalMat * vertexNorml;
}

!#!shader fragment
#version 450 core

out vec4 frgColor;

smooth in vec3 vColor;
smooth in vec2 tCord;
smooth in vec3 objPos;
smooth in vec3 vNorml;

uniform vec3 lightColor;
uniform vec3 uColor;
uniform vec3 lightPos;
uniform sampler2D texUnit;
uniform vec3 camPos;

void main() {

	vec3 norm = normalize(vNorml);
	vec3 lightDir = normalize(lightPos - objPos);

	float ambientFac = 0.45f;

	float diffFac = max(dot(norm, lightDir), 0.0f);

	float specFac = pow(max(dot(reflect(-lightDir, norm), normalize(camPos - objPos)), 0.f), 20.f);

	vec3 lightFac = (ambientFac + diffFac + specFac) * lightColor;

	frgColor = texture(texUnit, tCord) * vec4(lightFac, 1.f);

}