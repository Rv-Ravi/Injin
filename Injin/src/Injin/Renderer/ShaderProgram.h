#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "../glContext/graphicsInit.h"
#include <string>
#include <unordered_map>

namespace engin {

	enum class SHADERTYPE : int32_t{
		NONE = -1,VERTEX = 0,FRAGMENT = 1
	};
	class ShaderProgram
	{
		//data members
	public:
		uint32_t m_programId;
		static uint32_t shadrTyp[2];
		std::unordered_map<std::string, int32_t> m_uLocVal;
		static std::unordered_map<std::string, std::array<uint32_t, 3>> m_uniformBlockLink;

		static uint32_t bindSlot;

	public:
		ShaderProgram(const std::string& fileName);
		~ShaderProgram();

	//methods
	public:
		uint32_t getProgramId() const { return m_programId; }
		void bindProgram();
		static void unbindProgram();

		void clearProgram();

		

		template<typename varType>
		void setUniValueV(const char* varName, const varType& value, uint16_t size)
		{
			int32_t tmpVal = getUlocation(varName);

			if (typeid(varType) == typeid(float*))
			{
				switch (size)
				{
				case 1: glUniform1f(tmpVal, value[0]); break;
				case 2: glUniform2f(tmpVal, value[0], value[1]); break;
				case 3: glUniform3f(tmpVal, value[0], value[1], value[2]); break;
				case 4: glUniform4f(tmpVal, value[0], value[1], value[2], value[4]); break;
				default: printf("No such value is found for setting float* type as uniform."); break;
				}
			}

			else if (typeid(varType) == typeid(int32_t*))
			{
				switch (size)
				{
				case 1: glUniform1i(tmpVal, value[0]); break;
				case 2: glUniform2i(tmpVal, value[0], value[1]); break;
				case 3: glUniform3i(tmpVal, value[0], value[1], value[2]); break;
				case 4: glUniform4i(tmpVal, value[0], value[1], value[2], value[4]); break;
				default: printf("No such value is found for setting int* type as uniform."); break;
				}
			}

		}

		template<typename varType>
		void setUniValuefV(const char* varName, const varType& value, uint16_t size)
		{
			int32_t tmpVal = getUlocation(varName);
			switch (size)
			{
			case 1: glUniform1fv(tmpVal, 1, &value.x); break;
			case 2: glUniform2fv(tmpVal, 1, &value.x); break;
			case 3: glUniform3fv(tmpVal, 1, &value.x); break;
			case 4: glUniform4fv(tmpVal, 1, &value.x); break;
			default: printf("No such value is found for setting int* type as uniform."); break;
			}
		}

		template<typename varType>
		void setUniValueiV(const char* varName, const varType& value, uint16_t size)
		{
			int32_t tmpVal = getUlocation(varName);
			switch (size)
			{
			case 1: glUniform1iv(tmpVal, 1, &value.x); break;
			case 2: glUniform2iv(tmpVal, 1, &value.x); break;
			case 3: glUniform3iv(tmpVal, 1, &value.x); break;
			case 4: glUniform4iv(tmpVal, 1, &value.x); break;
			default: printf("No such value is found for setting int* type as uniform."); break;
			}
		}

		template<typename varType>
		void setUniValueM(const char* varName, const varType& value, uint16_t size)
		{
			int32_t tmpVal = getUlocation(varName);
			switch (size)
			{
			case 2: glUniformMatrix2fv(tmpVal, 1, GL_FALSE, &value[0].x); break;
			case 3: glUniformMatrix3fv(tmpVal, 1, GL_FALSE, &value[0].x); break;
			case 4: glUniformMatrix4fv(tmpVal, 1, GL_FALSE, &value[0].x); break;
			default: printf("No such value is found for setting float* type as uniform."); break;
			}
		}

		template<typename varType>
		void setUniValue(const char* varName, const varType value)
		{
			int32_t tmpVal = getUlocation(varName);

			if (typeid(varType) == typeid(float))
			{
				glUniform1f(tmpVal, value);
			}

			else if (typeid(varType) == typeid(int32_t*))
			{
				glUniform1i(tmpVal, value);
			}
		}

		static uint32_t getUniformBuffer(const std::string& bufName);

		static void setUniformBufferData(const std::string& bufName,GLintptr offset,GLsizeiptr size,const void* data);
	private:
		void createProgram(const std::string& fileName);
		void processShaderFile(const std::string& fileName);
		void processShader(const engin::SHADERTYPE& shaderType,const char* shaderCode);
		int32_t getUlocation(const char* varName);

		void getUniformBlock();

	};
}

#endif //SHADERPROGRAM_H

