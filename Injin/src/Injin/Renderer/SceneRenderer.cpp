#include "SceneGraph.h"



struct Matrices
{
	glm::mat4 transMat;
	glm::mat4 viewProjMat;
};

engin::SceneRenderer::SceneRenderer()
{
	createProgram();
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


	for (auto& entt : scene->m_enttList)
	{
		shader::setUniformBufferData("Matrices", 0, sizeof(glm::mat4)
			, &entt.getComponent<TransformComponent>()->getModelMatrix()[0].x);

		auto texture = entt.getComponent<TextureComponent>();
		auto material = entt.getComponent<MaterialComponent>();

		if (texture)
		{
			texture->texture->bindTextureUnit(0);
			m_shaderProgram[0].setUniValue("texUnit", 0);
			m_shaderProgram[0].setUniValue("isTextured", 1);
		}
		else {
			m_shaderProgram[0].setUniValuefV("uColor", entt.getComponent<MaterialComponent>()->m_materialColor, 3);
			m_shaderProgram[0].setUniValue("isTextured", 0);
		}
		
		m_shaderProgram[0].setUniValue("texUnit", 0);
		entt.getComponent<MeshComponent>()->m_meshData->drawMesh();
	}
	shader::unbindProgram();
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
