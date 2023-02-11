!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;


layout (std140) uniform Matrices{
	mat4 transMat;
	mat4 viewProj;
};


void main()
{
	gl_Position = viewProj * transMat * vec4(vertexPts, 1.f);
}

!#!shader fragment
#version 450 core

uniform vec3 uColor;

void main()
{
	gl_FragColor = vec4(uColor,1.f);
}