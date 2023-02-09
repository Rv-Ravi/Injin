#ifndef MESHES_H
#define MESHES_H

#include <iostream>
#include <vector>
#include "../glContext/graphicsInit.h"
#include "glm/glm.hpp"

namespace engin
{
	struct vertexData
	{
		glm::vec3 vertexPoints;
		glm::vec3 vertexColor;
		glm::vec2 textureCoord;
		glm::vec3 vertexNormal;

	};

	class Meshes
	{
	private:
		uint32_t m_VAO, m_VBO, m_IBO;
		size_t vertexDataSize, indexDataSize;

		
	public:
		Meshes(const std::vector<vertexData>& vData, const std::vector<uint32_t>& iData = {}
		,const std::string& name = "Mesh");
		Meshes(const Meshes& mesh) noexcept;
		Meshes(Meshes&& mesh)noexcept;
		~Meshes();

		std::string m_meshName;

	//operators
		Meshes& operator=(const Meshes& mesh)noexcept;
		Meshes& operator=(Meshes&& mesh)noexcept;
	//getters and setters
	public:
		uint32_t getVAO() const { return m_VAO; }
		uint32_t getVBO() const { return m_VBO; }
		uint32_t getIBO() const { return m_IBO; }
	//methods
	public:
		void bindVertexArray();
		void unBindVertexArray();
		void drawMesh();
	private:
		void setData(const std::vector<vertexData>& vertexData, const std::vector<uint32_t>& indexData);

	};
	extern std::vector<uint32_t> blockIndex;
	extern std::vector<vertexData> block;
	extern std::vector<vertexData> square;
	extern std::vector<uint32_t> squareIndex;
	extern std::vector<vertexData> triangle;

	extern std::vector<vertexData> line2D;
}

#endif //MESHES_H
