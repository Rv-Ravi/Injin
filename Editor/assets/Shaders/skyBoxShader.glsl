!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;

smooth out vec3 tCord;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	vec4 pos = proj * view * vec4(vertexPts, 1.f);
	gl_Position = pos.xyww;
	tCord = vertexPts;
}

!#!shader fragment
#version 450 core

smooth in vec3 tCord;
uniform samplerCube texUnit;


void main()
{
	gl_FragColor = texture(texUnit, tCord);
}