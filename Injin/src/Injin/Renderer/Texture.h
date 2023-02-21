#ifndef TEXTURE_H
#define TEXTURE_H

#include <vector>
#include <string>
#include <unordered_map>


namespace engin
{
	class Texture
	{

	public:
		static std::vector<std::pair<std::string,Texture>> m_textureList;
	private:
		uint32_t m_textureId;	

	public:
		Texture() = default;
		Texture(const std::string& texture);
		~Texture();

		uint32_t getTxtureId() const {
			return m_textureId;
		}

		void bindTextureUnit(uint32_t unit);

		static Texture* getTexture(const std::string& texture);

	private:
		void getTextureFile(const std::string& texture);
		void processTexture(const std::string& texture);

	};
}



#endif //TEXTURE_H

