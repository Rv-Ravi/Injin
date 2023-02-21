!#!shader vertex
#version 450 core
layout(location = 0) in vec3 vertexPts;
layout(location = 1) in vec3 vertexCol;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec3 vertexNormal;

out attOut{
	vec2 tCord;
	vec3 vNormal;
	vec3 objPos;
} vOut;

layout (std140) uniform Matrices{
	mat4 transMat;
	mat4 viewProj;
	mat4 normalMat;
};
void main()
{
	vOut.objPos = vec3(transMat * vec4(vertexPts, 1.f));
	gl_Position = viewProj * vec4(vOut.objPos, 1.f);
	vOut.vNormal = mat3(normalMat) * vertexNormal;
	vOut.tCord = texCoord;
}

!#!shader fragment
#version 450 core

in attOut{
	vec2 tCord;
	vec3 vNormal;
	vec3 objPos;
} fIn;

layout (std140) uniform objMaterial{
	
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 SpecularColor;
	float shinniness;
	bool isTextured;

	
} material;

uniform sampler2D diffTexUnit;
uniform sampler2D specTexUnit;
uniform vec3 camPos;

void main()
{
    vec3 normal = normalize(fIn.vNormal);
	vec3 lightDir = normalize(camPos - fIn.objPos);
	if(material.isTextured == false)
	{
		
		vec3 diffFac = max(dot(lightDir,normal),0.f) * material.diffuseColor;
		vec3 specFac = pow(max(dot(reflect(-lightDir,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness) * material.SpecularColor;

		gl_FragColor = vec4((material.ambientColor + diffFac + specFac) * vec3(1.f), 1.f);
	}
	else{
		gl_FragColor = texture(diffTexUnit,fIn.tCord);
	}
	
}
