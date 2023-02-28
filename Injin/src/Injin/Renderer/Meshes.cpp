#include "Meshes.h"
#include <fstream>
engin::Meshes::Meshes(const std::vector<vertexData>& vData, const std::vector<uint32_t>& iData, const std::string& name)
	:vertexDataSize(vData.size()),indexDataSize(iData.size()),m_meshName(name)
{

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	if(indexDataSize != 0)
		glGenBuffers(1, &m_IBO);

	setData(vData, iData);

}

engin::Meshes::Meshes(const std::string& name)
	:m_meshName(name)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);
}

engin::Meshes::Meshes(const Meshes& mesh) noexcept
{
	m_VAO = mesh.m_VAO;
	m_VBO = mesh.m_VBO;
	m_IBO = mesh.m_IBO;
	vertexDataSize = mesh.vertexDataSize;
	indexDataSize = mesh.indexDataSize;
}

engin::Meshes::Meshes(Meshes&& mesh) noexcept
{
	m_VAO = std::move(mesh.m_VAO);
	m_VBO = std::move(mesh.m_VBO);
	m_IBO = std::move(mesh.m_IBO);
	vertexDataSize = std::move(mesh.vertexDataSize);
	indexDataSize =  std::move(mesh.indexDataSize);
}

engin::Meshes::~Meshes()
{

}

engin::Meshes& engin::Meshes::operator=(const Meshes& mesh) noexcept
{
	if (&mesh != this)
	{
		m_VAO = mesh.m_VAO;
		m_VBO = mesh.m_VBO;
		m_IBO = mesh.m_IBO;
		vertexDataSize = mesh.vertexDataSize;
		indexDataSize = mesh.indexDataSize;
	}
	return *this;
}

engin::Meshes& engin::Meshes::operator=(Meshes&& mesh) noexcept
{
	if (&mesh != this)
	{
		m_VAO = std::move(mesh.m_VAO);
		m_VBO = std::move(mesh.m_VBO);
		m_IBO = std::move(mesh.m_IBO);
		vertexDataSize = std::move(mesh.vertexDataSize);
		indexDataSize = std::move(mesh.indexDataSize);
	}
	return *this;
}

void engin::Meshes::setDataSize(size_t t1, size_t t2)
{
	vertexDataSize = t1;
	indexDataSize = t2;
}

void engin::Meshes::bindVertexArray()
{
	glBindVertexArray(m_VAO);
}

void engin::Meshes::unBindVertexArray()
{
	glBindVertexArray(0);
}

void engin::Meshes::drawMesh()
{
	this->bindVertexArray();
	if (indexDataSize != 0)
		glDrawElements(engin::drawMode, indexDataSize, GL_UNSIGNED_INT, (void*)0);
	else
		glDrawArrays(engin::drawMode, 0, (GLsizei)vertexDataSize);
	this->unBindVertexArray();
}

void engin::Meshes::deleteBuffers()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

void engin::Meshes::setData(const std::vector<vertexData>& vData, const std::vector<uint32_t>& iData)
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) * vertexDataSize, &vData[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexPoints);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexColor);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->textureCoord);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexNormal);

	if (indexDataSize != 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indexDataSize, &iData[0], GL_STATIC_DRAW);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

std::vector<uint32_t> engin::blockIndex =
{
	//TOP
	0,1,2,
	0,2,3,
	//BOTTOM
	4,5,6,
	4,6,7,
	//FRONT
	8,9,10,
	8,10,11,
	//BACK
	12,13,14,
	12,14,15,
	//RIGHT
	9,10,13,
	10,14,13,
	//LEFT
	8,11,12,
	11,15,12
};
std::vector<engin::vertexData> engin::block =
{   //POSITION	                   //COLOR					  //TEX-CO					//NORMAL
	//TOP
	{glm::vec3(-1.0f,1.0f,1.0f),   glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),     glm::vec3(-0.33f,0.33f,0.33f)},
	{glm::vec3(1.0f,1.0f,1.0f),	  glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.9f),	    glm::vec3(0.33f,0.33f,0.33f)},
	{glm::vec3(1.0f,1.0f,-1.0f),	  glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.9f),	    glm::vec3(0.33f,0.33f,-0.33f)},
	{glm::vec3(-1.0f,1.0f,-1.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,1.0f),	    glm::vec3(-0.33f,0.33f,-0.33f)},

	//BOTTOM
	{glm::vec3(-1.0f,-1.0f,1.0f),  glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.0f)},
	{glm::vec3(1.0f,-1.0f,1.0f),	  glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.0f)},
	{glm::vec3(1.0f,-1.0f,-1.0f),  glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.0f)},
	{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.0f)},

	//FRONT
	{glm::vec3(-1.0f,1.0f,1.0f),   glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),     glm::vec3(-0.33f,0.33f,0.33f)},
	{glm::vec3(1.0f,1.0f,1.0f),	  glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),	    glm::vec3(0.33f,0.33f,0.33f)},
	{glm::vec3(1.0f,-1.0f,1.0f),	  glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.33f,-0.33f,0.33f)},
	{glm::vec3(-1.0f,-1.0f,1.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(-0.33f,-0.33f,0.33f)},

	//BACK
	{glm::vec3(-1.0f,1.0f,-1.0f),   glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(1.0f,1.0f),        glm::vec3(-0.33f,0.33f,-0.33f)},
	{glm::vec3(1.0f,1.0f,-1.0f),	   glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,1.0f),	    glm::vec3(0.33f,0.33f,-0.33f)},
	{glm::vec3(1.0f,-1.0f,-1.0f),   glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.33f,-0.33f,-0.33f)},
	{glm::vec3(-1.0f,-1.0f,-1.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(-0.33f,-0.33f,-0.33f)}

};



std::vector<engin::vertexData> engin::square = {
	{glm::vec3(-1.0f,-1.0f,0.0f),   glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),     glm::vec3(0.f,0.f,1.f)},
	{glm::vec3(1.0f,-1.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.f,0.f,1.f)},
	{glm::vec3(1.0f,1.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,1.0f),	    glm::vec3(0.f,0.f,1.f)},
	{glm::vec3(-1.0f,1.0f,0.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,1.0f),	    glm::vec3(0.f,0.f,1.f)},

};

std::vector<engin::vertexData> engin::triangle = {
	{glm::vec3(-1.0f,-1.0f,0.0f),   glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),     glm::vec3(0.f,0.f,1.f)},
	{glm::vec3(1.0f,-1.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.f,0.f,1.f)},
	{glm::vec3(0.0f,1.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,1.0f),	    glm::vec3(0.f,0.f,1.f)},
};

std::vector<uint32_t> engin::squareIndex = {
		0,1,2,
		0,2,3
};


std::vector<engin::vertexData> engin::line2D = {
	{glm::vec3(0.0f,0.0f,0.0f),   glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),     glm::vec3(0.f,0.f,0.f)},
	{glm::vec3(1.0f,0.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.f,0.f,0.f)},
};


engin::TerrainGeneration::TerrainGeneration()
	:m_octave(2), m_xOffset(0), m_yOffset(0), m_amp(3.f), m_lucnarity(2.f),
	m_persistance(0.5f), m_scale(15.f), m_terrainMesh("Terrain"),noise(121)
{
	generateTerrain();
}

engin::TerrainGeneration::~TerrainGeneration()
{
}

void engin::TerrainGeneration::generateTerrain()
{
	std::vector<vertexData> tmpVertex;
	std::vector<uint32_t> tmpIndex;

	float minNoise = 100.f, maxNoise = -100.f;
	generateNoiseMesh(tmpVertex, minNoise, maxNoise);
	generateMeshIndex(tmpIndex);
	m_terrainMesh.setDataSize(tmpVertex.size(), tmpIndex.size());
	setBufferData(tmpVertex, tmpIndex);
}

void engin::TerrainGeneration::setBufferData(const std::vector<vertexData>& vData, const std::vector<uint32_t>& iData)
{
	glBindVertexArray(m_terrainMesh.getVAO());
	glBindBuffer(GL_ARRAY_BUFFER, m_terrainMesh.getVBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_terrainMesh.getIBO());
	if (!isSettedUp)
	{
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) * 201 * 201, nullptr, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexPoints);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexColor);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->textureCoord);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexNormal);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 201 * 201 * 6, nullptr, GL_DYNAMIC_DRAW);


		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData) * vData.size(), &vData[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * iData.size(), &iData[0]);
		isSettedUp = true;
		
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData) * vData.size(), &vData[0]);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(uint32_t) * iData.size(), &iData[0]);
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void engin::TerrainGeneration::generateNoiseMesh(std::vector<vertexData>& data, float& min, float& max)
{
	vertexData vData = vertexData();
	vData.vertexColor = glm::vec3(1.f, 0.f, 0.f);
	vData.vertexNormal = glm::vec3(0.f, 1.f, 0.f);

	for (uint16_t i = 0; i <= m_height; i++)
	{
		for (uint16_t j = 0; j <= m_width; j++)
		{
			vData.vertexPoints.y = 0;
			float tmpFreq = 1,noiseVal = 0, tmpAmp = m_amp;
			for (uint16_t oct = m_octave; oct > 0; oct--)
			{
				noiseVal = noise.noise2d({ (float)j / m_scale * tmpFreq + m_xOffset
					, (float)i / m_scale * tmpFreq + m_yOffset }) * 2.f - 1.f;

				if (i > m_fallOff && i < m_width - m_fallOff && j > m_fallOff && j < m_height - m_fallOff)
					vData.vertexPoints.y += noiseVal * tmpAmp;
				else
					vData.vertexPoints.y += noiseVal;
				tmpFreq *= m_lucnarity;
				tmpAmp *= m_persistance;
			}
			if (noiseVal > max) max = noiseVal;
			if (noiseVal < min) min = noiseVal;
			vData.vertexPoints.x = j - (m_width / 2.f); vData.vertexPoints.z = i - (m_height / 2.f);
			vData.textureCoord.x = (1.f / m_width) * j; vData.textureCoord.y = 1.f - (1.f / m_height) * i;
			data.push_back(vData);
		}
	}
}

void engin::TerrainGeneration::generateMeshIndex(std::vector<uint32_t>& data)
{
	for (uint32_t i = 0; i < m_height; i++)
	{
		for (uint32_t j = 0; j < m_width; j++)
		{
			uint32_t d1 = (m_width + 1) * (i + 1) + j,
				d2 = (m_width + 1) * i + (1 + j),
				d3 = (m_width + 1) * i + j;

				data.emplace_back(d1);
				data.emplace_back(d2);
				data.emplace_back(d3);
				data.emplace_back(d1);
				data.emplace_back(d1 + 1);
				data.emplace_back(d2);
		}
	}
}

