#include "Texture.h"
#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>

std::vector<std::pair<std::string, engin::Texture>> engin::Texture::m_textureList = {};

engin::Texture::Texture(const std::string& texture)
{
	processTexture(texture);
}

engin::Texture::~Texture()
{
}

void engin::Texture::bindTextureUnit(uint32_t unit)
{
	glBindTextureUnit(unit, m_textureId);
}

engin::Texture* engin::Texture::getTexture(const std::string& texture)
{
	auto iterator = std::find_if(m_textureList.begin(), m_textureList.end(), [&](std::pair<std::string, engin::Texture>& pair)
		{
			if (pair.first == texture)
				return true;
			return false;
		});
	return (iterator != m_textureList.end()) ? &(*iterator).second : nullptr;
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
