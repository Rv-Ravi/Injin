!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;
layout(location = 1) in vec3 vertexCol;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexNormal;

smooth out vec3 vColor;
smooth out vec2 tCord;

uniform mat4 transMat;
uniform mat4 viewProj;

void main()
{
	gl_Position = viewProj * transMat * vec4(vertexPts, 1.f);
	vColor = vertexCol;
	tCord = texCoord;
}

!#!shader fragment
#version 450 core

smooth in vec3 vColor;
smooth in vec2 tCord;

uniform vec3 uColor;

void main()
{
	gl_FragColor = vec4(uColor,1.f);
}
