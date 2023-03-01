#ifndef MESHES_H
#define MESHES_H

#include <iostream>
#include <vector>
#include "../glContext/graphicsInit.h"
#include "glm/glm.hpp"
#include "../Core/Noise.h"

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
		size_t vertexDataSize = 0, indexDataSize = 0;

		
	public:
		Meshes(const std::vector<vertexData>& vData, const std::vector<uint32_t>& iData = {}
		,const std::string& name = "Mesh");
		Meshes(const std::string& name);
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

		void setDataSize(size_t t1, size_t t2 = 0);
	//methods
	public:
		void bindVertexArray();
		void unBindVertexArray();
		void drawMesh();
		void deleteBuffers();
		void setData(const std::vector<vertexData>& vertexData, const std::vector<uint32_t>& indexData = {});
	};

	class TerrainGeneration
	{
	public:
		int32_t m_octave,m_width = 100,m_height = 100;
		float m_amp, m_lucnarity, m_persistance,m_scale, m_xOffset, m_yOffset;
		Meshes m_terrainMesh;
		float fallOffs = 0, fallOffe = 1.f;
		
	private:
		bool isSettedUp = false;
		Noise noise;
	public:
		TerrainGeneration();
		~TerrainGeneration();

		void generateTerrain();

	private:
		void setBufferData(const std::vector<vertexData>& vertexData, const std::vector<uint32_t>& indexData = {});
		void generateNoiseMesh(std::vector<vertexData>& data, float& min, float& max);
		void generateMeshIndex(std::vector<uint32_t>& data);
		float getFallOffValue(uint32_t p1,uint32_t p2);
	};


	extern std::vector<uint32_t> blockIndex;
	extern std::vector<vertexData> block;
	extern std::vector<vertexData> square;
	extern std::vector<uint32_t> squareIndex;
	extern std::vector<vertexData> triangle;

	extern std::vector<vertexData> line2D;
}

#endif //MESHES_H
