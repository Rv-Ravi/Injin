#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "../Renderer/Camera.h"
#include "Yentt.h"
#include "../Renderer/ShaderProgram.h"
#include <filesystem>
#include <unordered_map>

namespace engin {

	class SceneGraph
	{
	private:
		friend class SceneRenderer;

		std::vector<engin::Yentt> m_enttList;
		std::unique_ptr<engin::PerspectiveCamera> m_scenePerspectiveCamera;
		static std::unordered_map<std::string, Meshes*> m_meshList;

		static engin::Yentt* currentYentt;
		bool CameraSettings = false;

		std::unique_ptr<engin::SceneRenderer> m_sceneRenderer;



	public:
		SceneGraph();
		~SceneGraph();

		void addEntt();

		void addSquareEntt();
		void addCubeEntt();
		void addTriangleEntt();

		void addDirectionalLit();
		void addPointLit();
		void addSpotLit();

		void addBase();

		void sceneUpdate(engin::WindowGL& window,float dtime);
		void drawScene(engin::WindowGL& window,float dtime);

		void ImGuiWindows();

		void deleteMesh();

		void addTerrain();

	private:
		static void processMesh();
	};


	class SceneRenderer
	{
	private:
		std::vector<ShaderProgram> m_shaderProgram;

	public:
		SceneRenderer();
		~SceneRenderer();

		void render(SceneGraph* scene);
		void clearPrograms();
		
	private:
		void createProgram();
		void processEntt(ShaderProgram& prog,engin::Yentt& entt);
		void processLight(engin::Yentt& entt);
		
		

	};

}


#endif //SCENEGRAPH_H
