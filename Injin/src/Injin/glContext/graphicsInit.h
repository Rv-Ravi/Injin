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
		static glm::vec4 bgColor;
	public:
		grpicsInit();
		~grpicsInit();

	//methods
	public:
		static void clrBuffr();
		static void clrColrBuffr();
		static void grpicInit();
		static void imGuiBg();
	};



}

#endif //GRAPHICSINIT_H
