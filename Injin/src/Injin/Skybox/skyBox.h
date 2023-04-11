#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>
#include "glm/glm.hpp"
#include "../Renderer/ShaderProgram.h"


namespace engin
{
	struct skyBoxMat
	{
		std::string m_material[6];

		skyBoxMat() = default;
		skyBoxMat(const skyBoxMat&) = default;

	};

	class SkyBox
	{

	public:
		engin::ShaderProgram* program;
		static uint32_t VBO, IBO, VAO;
	private:
		skyBoxMat m_sbmat;
		uint32_t m_sbTexId;
	public:

		SkyBox();
		static SkyBox& getskyBox();
		~SkyBox();
		SkyBox(const SkyBox&) = delete;
		SkyBox& operator=(const SkyBox&) = delete;

		void bindTexture();
		void bindTextureUnit(uint32_t unit = 0);
		static void unbindTexture();

		void processSkyBox();

		void drawSkyBox(const glm::mat4& view,const glm::mat4& proj);

	private:
		void processSkyBoxMesh();
	};
}


#endif
