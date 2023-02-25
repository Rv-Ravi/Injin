#include "SceneGraph.h"

struct Material {

	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 SpecularColor;
	float shinniness;
	int isTextured;
};

struct alignas(16) vec3f  {
	float x,y,z;
};

std::ostream& operator<<(std::ostream& out,glm::vec3 value)
{
	out<< std::fixed << value.x << " " << value.y << " " << value.z;
	return out;
}

struct Matrices
{
	glm::mat4 transMat;
	glm::mat4 viewProjMat;
};

engin::SceneRenderer::SceneRenderer()
{
	createProgram();
	auto curtPath = std::filesystem::current_path();
	for (auto& file : std::filesystem::directory_iterator(curtPath.string() + std::string("\\assets\\Textures")))
	{
		if(file.is_regular_file())
		Texture::m_textureList.emplace_back(file.path().string(), Texture(file.path().string()));
	}
}

engin::SceneRenderer::~SceneRenderer()
{
	clearPrograms();
}

void engin::SceneRenderer::render(SceneGraph* scene)
{
	typedef ShaderProgram shader;
	m_shaderProgram[0].bindProgram();

	shader::setUniformBufferData( "Matrices", sizeof(glm::mat4), sizeof(glm::mat4)
		, &scene->m_scenePerspectiveCamera->getViewProjMat()[0].x);

	m_shaderProgram[0].setUniValuefV("camPos", scene->m_scenePerspectiveCamera->getPos(), 3);
	for (auto& entt : scene->m_enttList)
	{
		shader::setUniformBufferData("Matrices", 0, sizeof(glm::mat4)
			, &entt.getComponent<TransformComponent>()->getModelMatrix()[0].x);
		shader::setUniformBufferData("Matrices", sizeof(glm::mat4) * 2, sizeof(glm::mat4)
			, &entt.getComponent<TransformComponent>()->getNormalMatrix()[0].x);
		auto material = entt.getComponent<MaterialComponent>();

		if (material->isTexture)
		{
			auto texture1 = Texture::getTexture(material->m_texMaterial.m_diffuse);
			auto texture2 = Texture::getTexture(material->m_texMaterial.m_specular);
			if (texture1)
			{
				texture1->bindTextureUnit(0);
				m_shaderProgram[0].setUniValue("diffTexUnit", 0);
			}
			if (texture2)
			{
				texture2->bindTextureUnit(1);
				m_shaderProgram[0].setUniValue("specTexUnit", 1);
			}
		}
		else {
			shader::setUniformBufferData("objMaterial",0,
				sizeof(glm::vec3), &material->m_colMaterial.m_ambient.x);
			shader::setUniformBufferData("objMaterial", sizeof(glm::vec4),
				sizeof(glm::vec3), &material->m_colMaterial.m_diffuse.x);
			shader::setUniformBufferData("objMaterial", sizeof(glm::vec4) * 2,
				sizeof(glm::vec3), &material->m_colMaterial.m_specular.x);

		}
		shader::setUniformBufferData("objMaterial", sizeof(glm::vec3) + sizeof(glm::vec4) * 2, sizeof(float)
			, &material->m_shininess);
		shader::setUniformBufferData("objMaterial", sizeof(glm::vec4) * 3, sizeof(bool)
			, &material->isTexture);

		entt.getComponent<MeshComponent>()->m_meshData->drawMesh();
	}
	shader::unbindProgram();
	glBindTexture(GL_TEXTURE_2D, 0);
}

void engin::SceneRenderer::createProgram()
{
	namespace fileSys = std::filesystem;
	auto currntPath = fileSys::current_path();

	m_shaderProgram.emplace_back(currntPath.string() + std::string("\\assets\\Shaders\\simpleObj.glsl"));
	m_shaderProgram.emplace_back(currntPath.string() + std::string("\\assets\\Shaders\\axisLine.glsl"));

}

void engin::SceneRenderer::clearPrograms()
{
	for (auto& shader : m_shaderProgram)
		shader.clearProgram();
}
