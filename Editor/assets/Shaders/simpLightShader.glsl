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
	vec3 vColor;
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
	vOut.vColor = vertexCol;
}

!#!shader fragment
#version 450 core

in attOut{
	vec2 tCord;
	vec3 vNormal;
	vec3 objPos;
	vec3 vColor;
} fIn;

layout (std140) uniform objMaterial{
	
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 SpecularColor;
	float shinniness;
	bool isTextured;

	
} material;

layout(std140) uniform Light{
	vec3 color;
	float dist;
	vec3 position;
	float innerAngle;
	vec3 direction;
	float outerAngle;
	int type;

}light;

uniform sampler2D diffTexUnit;
uniform sampler2D specTexUnit;
uniform vec3 camPos;


vec4 directionalLitCalc(in vec3 normal);
vec4 pointlLitCalc(in vec3 normal);
vec4 SpotLitCalc(in vec3 normal);

void main()
{
    vec3 normal = normalize(fIn.vNormal);
	vec4 result;
	if(light.type == 0)
	{
		result = directionalLitCalc(normal);
	}
	else if(light.type == 1)
	{
		result = pointlLitCalc(normal);
	}
	else if(light.type == 2)
	{
		result = SpotLitCalc(normal);
	}
	else{
		result = vec4(1.f);
	}
	
	gl_FragColor = result;
}


vec4 directionalLitCalc(in vec3 normal){
	
	if(material.isTextured == false)
	{
		float ambFac = 0.32f;
		float diffFac = max(dot(-light.direction,normal),0.f);
		float specFac = pow(max(dot(reflect(light.direction,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness);

		return vec4((ambFac + diffFac + specFac) * light.color * fIn.vColor, 1.f);
	}
	else{

		vec4 diffFac = max(dot(-light.direction,normal),0.f) * texture(diffTexUnit,fIn.tCord);
		vec4 specFac = pow(max(dot(reflect(light.direction,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness) * texture(specTexUnit,fIn.tCord);

		return (diffFac + specFac) * vec4(light.color, 1.f);
	}

}
vec4 pointlLitCalc(in vec3 normal){
	
	float objLitDist = max(length(fIn.objPos - light.position),0.0001f);
	float attenFac = clamp(1.f - (objLitDist / light.dist),0.f,1.f);
	vec3 lightDir = normalize(fIn.objPos - light.position);
	if(material.isTextured == false)
	{
		float ambFac = 0.32f;
		float diffFac = max(dot(-lightDir,normal),0.f);
		float specFac = pow(max(dot(reflect(lightDir,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness);

		return vec4((ambFac + diffFac + specFac) * light.color * attenFac * fIn.vColor, 1.f);
	}
	else{

		vec4 diffFac = max(dot(-lightDir,normal),0.f) * texture(diffTexUnit,fIn.tCord);
		vec4 specFac = pow(max(dot(reflect(lightDir,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness) * texture(specTexUnit,fIn.tCord);

		return (diffFac + specFac) * vec4(light.color, 1.f) * attenFac;
	}
}
vec4 SpotLitCalc(in vec3 normal){
	vec3 objLitDist = fIn.objPos - light.position;
	float attenFac = clamp(1.f - (length(objLitDist) / light.dist),0.f,1.f);
	vec3 lightDir = normalize(fIn.objPos - light.position);

	float orgVal = dot(light.direction,normalize(objLitDist));

	if(material.isTextured == false)
	{
		float ambFac = 0.32f;
		if(orgVal > light.outerAngle)
		{
			float IntFac = (orgVal - light.outerAngle) / (light.innerAngle - light.outerAngle);
			float diffFac = max(dot(-lightDir,normal),0.f);
			float specFac = pow(max(dot(reflect(lightDir,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness);

			return vec4((ambFac + diffFac + specFac) * light.color * attenFac * IntFac * fIn.vColor, 1.f);
		}
		else{
			return vec4(vec3(ambFac) , 1.f) * vec4(0.1f);
		}
		

	}
	else{
		if(orgVal > light.outerAngle)
		{
			float IntFac = (orgVal - light.outerAngle) / (light.innerAngle - light.outerAngle);
			vec4 diffFac = max(dot(-lightDir,normal),0.f) * texture(diffTexUnit,fIn.tCord);
			vec4 specFac = pow(max(dot(reflect(lightDir,normal),normalize(camPos - fIn.objPos)),0.f),material.shinniness) * texture(specTexUnit,fIn.tCord);

			return (diffFac + specFac) * vec4(light.color, 1.f) * attenFac;
		}
		else
		{
			return texture(diffTexUnit,fIn.tCord) * vec4(0.1f);
		}
	}
}
