#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H

#include "Meshes.h"
#include <vector>
namespace engin
{
	class FrameBuffers
	{
	private:
		uint32_t m_fbo = 0 , m_depAndStenAtt = 0;
		uint32_t* m_colAtt;
		uint32_t m_nColAtt = 1,m_width,m_height;

	public:
		FrameBuffers(uint32_t width = 1920 ,uint32_t height = 1080, uint16_t nCol = 1,bool isDepth = true,bool isStencil = true, bool depthStencilAsTex = true);
		~FrameBuffers();

		uint16_t getFbo()const;
		uint16_t getColAtt(uint32_t index = 0)const;
		uint16_t getDepSenAtt()const;

		void bindFrameBuffer();
		static void unbindFrameBuffer();
		static void clrBuffer(const glm::vec4& value);

		void renderFrameBuffer(Meshes* mesh,uint16_t colAtt);


	private:
		void processFrameBuffer(bool isDepth = true, bool isStencil = true, bool depthStencilAsTex = true);
	};
}



#endif
