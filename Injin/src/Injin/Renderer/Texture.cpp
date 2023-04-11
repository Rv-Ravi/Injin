#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>

std::vector<std::string> engin::Texture::m_textureList = {"None"};
std::unordered_map<std::string, uint32_t> engin::Texture::m_processedTexture = {};

engin::Texture::Texture(const std::string& texture)
	:m_texName(texture)
{
	processTexture(texture);
	m_processedTexture[texture] = m_textureId;
}

engin::Texture::Texture(const std::string& texture, uint32_t id)
	:m_texName(texture),m_textureId(id)
{
}

engin::Texture::~Texture()
{
	delTexture();
}

void engin::Texture::bindTextureUnit(uint32_t unit)
{
	glBindTextureUnit(unit, m_textureId);
}

void engin::Texture::unBindTextureUnit(uint32_t unit)
{
	glBindTextureUnit(unit, 0);
}

void engin::Texture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void engin::Texture::unBindTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t engin::Texture::getTexture(const std::string& texture)
{
	auto iterator = m_processedTexture.find(texture);
	return (iterator != m_processedTexture.end()) ? iterator->second : 0;
}

void engin::Texture::delTexture()
{
	m_textureId = 0;
	m_texName = "";
}

void engin::Texture::clearTexture()
{
	for (auto& value : m_processedTexture)
	{
		if (value.second != 0)
			glDeleteTextures(1, &value.second);
	}
	std::cout << "All done\n";
}

void engin::Texture::processCubeMapTex(std::string* material)
{
	for (uint16_t i = 0; i < 6; i++)
	{
		int32_t iWidth, iHeight, iNoc;
		stbi_set_flip_vertically_on_load(0);
		unsigned char* imgData = stbi_load(material[i].c_str(), &iWidth, &iHeight, &iNoc, 0);

		if (imgData)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, iWidth, iHeight, 0,
				iNoc < 4 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		}
		else
		{
			std::cerr << "Unable to load the image file.\n";
		}
		stbi_image_free(imgData);
	}

}

void engin::Texture::getTextureFile(const std::string& texture)
{
	int32_t iWidth, iHeight, iNoc;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* imgData = stbi_load(texture.c_str(), &iWidth, &iHeight, &iNoc, 0);

	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, iNoc < 4 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Unable to load the image file.\n";
	}
	stbi_image_free(imgData);
}

void engin::Texture::processTexture(const std::string& texture)
{
	
	glGenTextures(1, &m_textureId);
	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_textureId, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(m_textureId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTextureParameteri(m_textureId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	getTextureFile(texture);
	glBindTexture(GL_TEXTURE_2D, 0);

}
