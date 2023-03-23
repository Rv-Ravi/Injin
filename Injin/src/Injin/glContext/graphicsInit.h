#ifndef GRAPHICSINIT_H
#define GRAPHICSINIT_H

#pragma warning( push )
#pragma warning(disable : 4251)
#pragma warning(disable : 4244)
#pragma warning(disable : 4018)
#pragma warning(disable : 4005)
#pragma warning(disable : 4267)

#include <glad/glad.h>
#include "../Windows/WindowGL.h"
#include "glm/glm.hpp"

namespace engin
{
	extern uint16_t drawMode;

	class grpicsInit {

	public:
		
	public:
		grpicsInit();
		~grpicsInit();

	//methods
	public:
		static void clrBuffr();
		static void clrColrBuffr(const glm::vec4& bgColor);
		static void grpicInit();
	};



}

#endif //GRAPHICSINIT_H
