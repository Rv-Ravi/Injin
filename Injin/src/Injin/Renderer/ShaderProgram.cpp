#include "ShaderProgram.h"

#include <fstream>
#include <sstream>
#include <iostream>

uint32_t engin::ShaderProgram::shadrTyp[2] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};
std::unordered_map<std::string, std::array<uint32_t, 3>> engin::ShaderProgram::m_uniformBlockLink = {};

uint32_t engin::ShaderProgram::bindSlot = 0;


engin::ShaderProgram::ShaderProgram(const std::string& fileName)
{
	m_programId = glCreateProgram();
	createProgram(fileName);

	getUniformBlock();
}

engin::ShaderProgram::~ShaderProgram()
{
}

void engin::ShaderProgram::bindProgram()
{
	glUseProgram(m_programId);
}

void engin::ShaderProgram::unbindProgram()
{
	glUseProgram(0);
}

void engin::ShaderProgram::clearProgram()
{
	glDeleteProgram(m_programId);
}

int32_t engin::ShaderProgram::getUlocation(const char* varName)
{
	std::unordered_map<std::string, int32_t>::iterator iterator =  m_uLocVal.find(varName);
	if (iterator == m_uLocVal.end())
	{
		int32_t tmpVal = glGetUniformLocation(m_programId, varName);
		m_uLocVal[varName] = tmpVal;
		return tmpVal;
	}
	return iterator->second;
}

void engin::ShaderProgram::getUniformBlock()
{
	int32_t bcount;
	glGetProgramiv(m_programId, GL_ACTIVE_UNIFORM_BLOCKS, &bcount);

	uint32_t bufSize, length, size;;

	for (uint32_t i = 0; i < bcount; i++)
	{
		glGetActiveUniformBlockiv(m_programId, i, GL_UNIFORM_BLOCK_NAME_LENGTH, (int32_t*)&bufSize);
		char* blockName = new char[bufSize + 1];
		glGetActiveUniformBlockiv(m_programId, i, GL_UNIFORM_BLOCK_DATA_SIZE, (int32_t*)&size);
		glGetActiveUniformBlockName(m_programId, i, bufSize,(int32_t*) & length, blockName);

		auto iterator = m_uniformBlockLink.find(std::string(blockName));
		if (iterator == m_uniformBlockLink.end())
		{
			std::array<uint32_t, 3> tmpVal = { ShaderProgram::bindSlot,size,0 };

			glUniformBlockBinding(m_programId, i, ShaderProgram::bindSlot++);
			glGenBuffers(1, &tmpVal[2]);
			glBindBuffer(GL_UNIFORM_BUFFER, tmpVal[2]);
			glBufferData(GL_UNIFORM_BUFFER, tmpVal[1], nullptr, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, tmpVal[0], tmpVal[2]);

			m_uniformBlockLink[std::string(blockName)] = tmpVal;
		}
		else {
			glUniformBlockBinding(m_programId, i, iterator->second[0]);
		}

		delete[] blockName;
	}
}

uint32_t engin::ShaderProgram::getUniformBuffer(const std::string& bufName)
{
	return m_uniformBlockLink.at(bufName)[2];
}



void engin::ShaderProgram::createProgram(const std::string& fileName)
{
	processShaderFile(fileName);
	glLinkProgram(m_programId);
	glValidateProgram(m_programId);

	int32_t valFlg, lnkFlg;
	glGetProgramiv(m_programId, GL_VALIDATE_STATUS, &valFlg);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &lnkFlg);

	if (!lnkFlg || !valFlg)
	{
		int32_t infoLogLen;
		glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &infoLogLen);
		char* programInfo = new char[infoLogLen + 1];
		glGetProgramInfoLog(m_programId, infoLogLen, nullptr, programInfo);
		std::cerr << "The error in : " << (!lnkFlg && !valFlg ? "Link and Validate : " : (!lnkFlg) ? "Link : " 
			: "Validate : ") << programInfo << std::endl;

		delete[] programInfo;
	}

}

void engin::ShaderProgram::processShaderFile(const std::string& fileName)
{
	std::ifstream shaderFile(fileName, std::ios::in);
	if (shaderFile.is_open())
	{
		std::stringstream strStrm[2];
		std::string line;
		engin::SHADERTYPE type = engin::SHADERTYPE::NONE;
		while (std::getline(shaderFile, line))
		{
			if (line != "\n" || line != "")
			{
				if (line.find("!#!shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						type = engin::SHADERTYPE::VERTEX;
					else if (line.find("fragment") != std::string::npos)
						type = engin::SHADERTYPE::FRAGMENT;
				}
				else
					strStrm[uint32_t(type)] << line << "\n";
			}
		}
		for (uint16_t typ = 0; typ < 2; typ++)
			processShader((engin::SHADERTYPE)typ, strStrm[typ].str().c_str());

	}
	else {
		printf("Unable to find the file : %s", fileName.c_str());
	}
	shaderFile.close();
}

void engin::ShaderProgram::processShader(const engin::SHADERTYPE& shaderType, const char* shaderCode)
{
	uint32_t shader = glCreateShader(shadrTyp[uint16_t(shaderType)]);
	int32_t srcLen = (int32_t)std::strlen(shaderCode);
	glShaderSource(shader, 1, &shaderCode, &srcLen);
	glCompileShader(shader);

	int32_t cplFlg;
	glGetShaderiv(shader,GL_COMPILE_STATUS, &cplFlg);
	if (!cplFlg)
	{
		int32_t infoLogLen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
		char* shaderInfo = new char[infoLogLen + 1];
		glGetShaderInfoLog(shader, infoLogLen,nullptr, shaderInfo);
		std::cerr << "The error in " << shaderInfo << std::endl;

		delete[] shaderInfo;
	}
	glAttachShader(m_programId, shader);
	glDeleteShader(shader);

}
