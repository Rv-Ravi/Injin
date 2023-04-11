#include "skyBox.h"
#include <glad/glad.h>
#include <filesystem>
#include "../Renderer/Texture.h"

uint32_t engin::SkyBox::VBO = 0, engin::SkyBox::IBO = 0, engin::SkyBox::VAO = 0;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//        7--------6
	 1.0f, -1.0f,  1.0f,//       /|       /|
	 1.0f, -1.0f, -1.0f,//      4--------5 |
	-1.0f, -1.0f, -1.0f,//      | |      | |
	-1.0f,  1.0f,  1.0f,//      | 3------|-2
	 1.0f,  1.0f,  1.0f,//      |/       |/
	 1.0f,  1.0f, -1.0f,//      0--------1
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

std::vector<glm::vec3> block =
{   //POSITION	                   
	glm::vec3(-1.0f,1.0f,1.0f),
	glm::vec3(-1.0f,-1.0f,1.0f),
	glm::vec3(1.0f,-1.0f,1.0f),
	glm::vec3(1.0f,1.0f,1.0f),

	//BACK
	glm::vec3(1.0f,1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),	
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,1.0f,-1.0f),

	//RIGHT
	glm::vec3(1.0f,1.0f,1.0f),  
	glm::vec3(1.0f,-1.0f,1.0f),	  
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f,1.0f,-1.0f), 

	//LEFT
	glm::vec3(-1.0f,1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(-1.0f,-1.0f,1.0f),
	glm::vec3(-1.0f,1.0f,1.0f), 

	//TOP
	glm::vec3(-1.0f,1.0f,1.0f),   
	glm::vec3(1.0f,1.0f,1.0f),	 
	glm::vec3(1.0f,1.0f,-1.0f),	  
	glm::vec3(-1.0f,1.0f,-1.0f),

	//BOTTOM
	glm::vec3(-1.0f,-1.0f,1.0f),
	glm::vec3(-1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,-1.0f),
	glm::vec3(1.0f,-1.0f,1.0f)

};

std::vector<uint32_t> blockIndex =
{

	//FRONT
	0,1,3,
	1,2,3,
	//BACK
	4,5,7,
	5,6,7,
	//RIGHT
	8,9,11,
	9,10,11,
	//LEFT
	12,13,15,
	13,14,15,
	//TOP
	16,18,19,
	16,17,18,
	//BOTTOM
	20,21,23,
	21,22,23
};


engin::SkyBox::SkyBox()
{
	namespace fileSys = std::filesystem;
	auto currntPath = fileSys::current_path();
	m_sbmat.m_material[0] = currntPath.string() + std::string("\\assets\\Textures\\SkyBoxes\\Spacebox2\\Spacebox_left.png");//right
	m_sbmat.m_material[1] = currntPath.string() + std::string("\\assets\\Textures\\SkyBoxes\\Spacebox2\\Spacebox_right.png");//left
	m_sbmat.m_material[2] = currntPath.string() + std::string("\\assets\\Textures\\SkyBoxes\\Spacebox2\\Spacebox_top.png");//top
	m_sbmat.m_material[3] = currntPath.string() + std::string("\\assets\\Textures\\SkyBoxes\\Spacebox2\\Spacebox_bottom.png");//bottom
	m_sbmat.m_material[4] = currntPath.string() + std::string("\\assets\\Textures\\SkyBoxes\\Spacebox2\\Spacebox_front.png");//front
	m_sbmat.m_material[5] = currntPath.string() + std::string("\\assets\\Textures\\SkyBoxes\\Spacebox2\\Spacebox_back.png");//back
	
	program = new ShaderProgram(std::filesystem::current_path().string() + std::string("\\assets\\Shaders\\skyBoxShader.glsl"));

	glGenTextures(1, &m_sbTexId);
	processSkyBoxMesh();
	processSkyBox();
}

engin::SkyBox& engin::SkyBox::getskyBox()
{
	static SkyBox box;
	return box;
}

engin::SkyBox::~SkyBox()
{
	delete program;
}

void engin::SkyBox::bindTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_sbTexId);
}

void engin::SkyBox::bindTextureUnit(uint32_t unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_sbTexId);
}

void engin::SkyBox::unbindTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void engin::SkyBox::processSkyBox()
{
	bindTexture();
	Texture::processCubeMapTex(m_sbmat.m_material);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	unbindTexture();
}


void engin::SkyBox::processSkyBoxMesh() {
	

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * block.size(), &block[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

		
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * blockIndex.size(), &blockIndex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


void engin::SkyBox::drawSkyBox(const glm::mat4& view, const glm::mat4& proj) {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glDepthFunc(GL_LEQUAL);
	program->bindProgram();
	program->setUniValueM("view", glm::mat4(glm::mat3(view)), 4);
	program->setUniValueM("proj", proj, 4);
	bindTextureUnit(0);
	program->setUniValue("texUnit", 0);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, blockIndex.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	program->unbindProgram();
}