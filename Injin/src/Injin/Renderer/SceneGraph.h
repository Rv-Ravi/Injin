#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "../Renderer/Camera.h"
#include "Yentt.h"
#include <unordered_map>
#include "../Renderer/ShaderProgram.h"

namespace engin {

	class SceneGraph
	{
	private:
		friend class SceneRenderer;

		std::vector<engin::Yentt> m_enttList;
		std::unique_ptr<engin::PerspectiveCamera> m_scenePerspectiveCamera;
		static std::unordered_map<std::string, Meshes*> m_meshList;

		engin::ShaderProgram program;

		static engin::Yentt* currentYentt;

	public:
		SceneGraph();
		~SceneGraph();

		void addEntt();

		void addSquareEntt();
		void addCubeEntt();
		void addTriangleEntt();

		void addBase();

		void sceneUpdate(engin::WindowGL& window,float dtime);
		void drawScene(engin::WindowGL& window,float dtime);

		void ImGuiWindows();

	private:
		static void processMesh();
	};

}


#endif //SCENEGRAPH_H
