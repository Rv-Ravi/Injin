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


struct LightEntity {
	vec3 m_position, m_direction;

	vec3 m_ambientIntensity, m_diffuseIntensity, m_specularIntensity;

	float m_dist;
};

out vec4 frgColor;

smooth in vec3 objPos;
smooth in vec3 vNorml;
smooth in vec2 tCord;

uniform Materials material;
uniform LightEntity light;
uniform vec3 camPos;

void main() {

	float atenFac;
	float dist = length(light.m_position - objPos);

	//atenFac = 1.f / (1.f + (0.007f * dist) + (dist * dist * 0.0032f));
	atenFac = clamp(light.m_dist / dist,0.f,1.f);

	vec3 norm = normalize(vNorml);
	vec3 lightDir = normalize(light.m_position - objPos);

	vec4 diffFac = max(dot(norm, lightDir), 0.0f) * texture(material.m_diffuse, tCord) * vec4(light.m_diffuseIntensity, 1.f);

	vec4 specFac = pow(max(dot(reflect(-lightDir, norm), normalize(camPos - objPos)), 0.f), material.m_shininess) * texture(material.m_specular, tCord) *
		vec4(light.m_specularIntensity, 1.f);

	frgColor = (vec4(light.m_ambientIntensity, 1.f) + diffFac + specFac) * atenFac;

}