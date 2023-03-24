!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;
layout(location = 1) in vec3 vertexCol;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexNorml;

smooth out vec2 tCord;

void main()
{
	gl_Position = vec4(vertexPts.x, vertexPts.y,0.f, 1.f);
	tCord = texCoord;
}

!#!shader fragment
#version 450 core
smooth in vec2 tCord;
uniform sampler2D texUnit;


void main()
{
	gl_FragColor = texture(texUnit,tCord);
}