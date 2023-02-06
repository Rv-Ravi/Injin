#ifndef GRAPHICSINIT_H
#define GRAPHICSINIT_H

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
