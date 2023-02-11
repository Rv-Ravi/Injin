#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>



namespace engin
{
	class Texture
	{
	private:
		uint32_t m_textureId;
		std::string m_textureName;

	public:
		Texture(const std::string& texture);
		~Texture();

		std::string getName() const {
			return m_textureName;
		}

		uint32_t getTxtureId() const {
			return m_textureId;
		}

		void bindTextureUnit(uint32_t unit);

	private:
		void getTextureFile(const std::string& texture);
		void processTexture(const std::string& texture);

	};
}



#endif //TEXTURE_H

