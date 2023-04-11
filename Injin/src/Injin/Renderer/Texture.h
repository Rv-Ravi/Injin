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
		static std::vector<std::string> m_textureList;
		static std::unordered_map<std::string, uint32_t> m_processedTexture;
		std::string m_texName;
	private:
		uint32_t m_textureId;	
		
	public:
		Texture() = default;
		Texture(const std::string& texture);
		Texture(const std::string& texture, uint32_t id);
		~Texture();

		uint32_t getTxtureId() const {
			return m_textureId;
		}

		void bindTextureUnit(uint32_t unit);
		void unBindTextureUnit(uint32_t unit);

		void bindTexture();
		static void unBindTexture();

		static uint32_t getTexture(const std::string& texture);

		void delTexture();
		static void clearTexture();

		static void processCubeMapTex(std::string* material);

	private:
		void getTextureFile(const std::string& texture);
		void processTexture(const std::string& texture);

	};
}



#endif //TEXTURE_H

