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
		Texture::m_textureList.emplace_back(file.path().string());
	}
}

engin::SceneRenderer::~SceneRenderer()
{
	clearPrograms();
	Texture::clearTexture();
}

void engin::SceneRenderer::render(SceneGraph* scene)
{
	typedef ShaderProgram shader;
	shader::setUniformBufferData( "Matrices", sizeof(glm::mat4), sizeof(glm::mat4)
		, &scene->m_scenePerspectiveCamera->getViewProjMat()[0].x);
	m_shaderProgram[2].bindProgram();
	m_shaderProgram[2].setUniValuefV("camPos", scene->m_scenePerspectiveCamera->getPos(), 3);
	for (auto& entt : scene->m_enttList)
	{
		std::string& tmpTag = entt.getComponent<TagComponent>()->m_tagName;
		if (tmpTag == "Light")
		{
			processLight(entt);
		}
	}
	scene->m_sceneFrame->bindFrameBuffer();
	FrameBuffers::clrBuffer({ 0.2f,0.2f,0.5f,1.f });
	for (auto& entt : scene->m_enttList)
	{
		std::string& tmpTag = entt.getComponent<TagComponent>()->m_tagName;
		if (tmpTag == "Object" || tmpTag == "Terrain")
		{
			auto flg = entt.getComponent<RenderComponent>();
			if (flg && flg->m_render)
			{	

				setDepth(flg);
				setStencil(flg);
				setFCull(flg);
				setBlend(flg);
				processEntt(m_shaderProgram[2],entt);	
			}

		}

	}

	FrameBuffers::unbindFrameBuffer();
	Texture::unBindTexture();
	shader::unbindProgram();
}

void engin::SceneRenderer::createProgram()
{
	namespace fileSys = std::filesystem;
	auto currntPath = fileSys::current_path();

	m_shaderProgram.emplace_back(currntPath.string() + std::string("\\assets\\Shaders\\simpleObj.glsl"));
	m_shaderProgram.emplace_back(currntPath.string() + std::string("\\assets\\Shaders\\simpLightShader.glsl"));
	m_shaderProgram.emplace_back(currntPath.string() + std::string("\\assets\\Shaders\\multiLitPhong.glsl"));
	m_shaderProgram.emplace_back(currntPath.string() + std::string("\\assets\\Shaders\\framBuffShader.glsl"));

}

void engin::SceneRenderer::processEntt(ShaderProgram& prog, engin::Yentt& entt)
{
	typedef ShaderProgram shader;
	

	shader::setUniformBufferData("Matrices", 0, sizeof(glm::mat4)
		, &entt.getComponent<TransformComponent>()->getModelMatrix()[0].x);
	shader::setUniformBufferData("Matrices", sizeof(glm::mat4) * 2, sizeof(glm::mat4)
		, &entt.getComponent<TransformComponent>()->getNormalMatrix()[0].x);
	auto material = entt.getComponent<MaterialComponent>();

	if (material && material->isTexture)
	{
		if (material->m_texMaterial.m_diffuse)
		{
			material->m_texMaterial.m_diffuse->bindTextureUnit(0);
			prog.setUniValue("diffTexUnit", 0);
		}	
		else
		{
			material->m_texMaterial.m_diffuse->unBindTextureUnit(0);
			prog.setUniValue("diffTexUnit", 0);
		}
		if (material->m_texMaterial.m_specular)
		{
			material->m_texMaterial.m_specular->bindTextureUnit(1);
			prog.setUniValue("specTexUnit", 1);
		}
			
		else
		{
			material->m_texMaterial.m_specular->unBindTextureUnit(1);
			prog.setUniValue("specTexUnit", 0);
			
		}
		
	}
	else {
		shader::setUniformBufferData("objMaterial", 0,
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


void engin::SceneRenderer::processLight(engin::Yentt& entt)
{
	typedef ShaderProgram shader;
	auto tComp = entt.getComponent<TransformComponent>();
	auto lComp = entt.getComponent<LightComponent>();

	glm::vec3 litDir = lComp->lightDirection(tComp->m_rotation);
	int32_t type = (int32_t)lComp->m_litTyp;
	glm::vec3 litColor = lComp->m_litColor * lComp->m_intensity;

	shader::setUniformBufferData("Light", 0,
		sizeof(glm::vec3), &litColor.x);
	shader::setUniformBufferData("Light", sizeof(glm::vec4),
		sizeof(glm::vec3), &tComp->m_position.x);
	shader::setUniformBufferData("Light", sizeof(glm::vec4) * 2,
		sizeof(glm::vec3), &litDir.x);

	shader::setUniformBufferData("Light", sizeof(glm::vec3),
		sizeof(float), &lComp->m_radius);
	shader::setUniformBufferData("Light", sizeof(glm::vec4) + sizeof(glm::vec3),
		sizeof(float), &lComp->m_innerAngle);
	shader::setUniformBufferData("Light", sizeof(glm::vec4) * 2 + sizeof(glm::vec3),
		sizeof(float), &lComp->m_outerAngle);
	shader::setUniformBufferData("Light", sizeof(glm::vec4) * 3,
		sizeof(int), &type);
}

void engin::SceneRenderer::setDepth(RenderComponent* component)
{
	if (component->m_depth)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(component->m_dMask);
		glDepthFunc(component->m_dFunc);
	}
	else
		glDisable(GL_DEPTH_TEST);
}

void engin::SceneRenderer::setStencil(RenderComponent* component)
{
	if (component->m_stencil)
	{
		glEnable(GL_STENCIL_TEST);
		if (component->m_sMaskFace == GL_FRONT_AND_BACK)
		{
			glStencilMaskSeparate(component->m_sMaskFace, component->m_sMask);
		}
		else if (component->m_sMaskFace == GL_FRONT)
		{
			glStencilMaskSeparate(component->m_sMaskFace, component->m_sMask);
			glStencilMaskSeparate(GL_BACK, 0x00);
		}
		else if (component->m_sMaskFace == GL_BACK)
		{
			glStencilMaskSeparate(component->m_sMaskFace, component->m_sMask);
			glStencilMaskSeparate(GL_FRONT, 0x00);
		}

		if (component->m_sOpFace == GL_FRONT_AND_BACK)
		{
			glStencilOpSeparate(component->m_sOpFace, GL_KEEP, GL_KEEP, GL_REPLACE);
		}
		else if (component->m_sOpFace == GL_FRONT)
		{
			glStencilOpSeparate(component->m_sOpFace, GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_KEEP);
		}
		else if (component->m_sOpFace == GL_BACK)
		{
			glStencilOpSeparate(component->m_sOpFace, GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_KEEP);
		}

		if (component->m_sFuncFace == GL_FRONT_AND_BACK)
		{
			glStencilFuncSeparate(component->m_sFuncFace, component->m_sFunc, component->m_sRefVal, component->m_sRefMask);
		}
		else if (component->m_sFuncFace == GL_FRONT)
		{
			glStencilFuncSeparate(component->m_sFuncFace, component->m_sFunc, component->m_sRefVal, component->m_sRefMask);
			glStencilFuncSeparate(GL_BACK, GL_NEVER, 0, 0x00);
		}
		else if (component->m_sFuncFace == GL_BACK)
		{
			glStencilFuncSeparate(component->m_sFuncFace, component->m_sFunc, component->m_sRefVal, component->m_sRefMask);
			glStencilFuncSeparate(GL_FRONT, GL_NEVER, 0, 0x00);
		}
	}
	else
		glDisable(GL_STENCIL_TEST);
}

void engin::SceneRenderer::setFCull(RenderComponent* component)
{
	if (component->m_fCull)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(component->m_frontFace);
		glCullFace(component->m_cullFace);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void engin::SceneRenderer::setBlend(RenderComponent* component)
{
	if (component->m_blend)
	{
		glEnable(GL_BLEND);
		glBlendColor(component->m_constCol.x, component->m_constCol.y, component->m_constCol.z, component->m_constCol.a);
		glBlendEquationSeparate(component->m_colFunc, component->m_colFunc);
		glBlendFuncSeparate(component->m_srcColFac, component->m_dstColFac, component->m_srcColFac, component->m_dstColFac);
	}
	else {
		glDisable(GL_BLEND);
	}
}


void engin::SceneRenderer::clearPrograms()
{
	for (auto& shader : m_shaderProgram)
		shader.clearProgram();
}
