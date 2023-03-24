#include "SceneGraph.h"

engin::Yentt* engin::SceneGraph::currentYentt = nullptr;
std::vector<std::string> engin::TagComponent::ms_tagList = { "Object","Light","Camera","Base","Terrain"};

std::vector<engin::Component*>::iterator engin::Yentt::tempComp;
std::unordered_map<std::string, engin::Meshes*> engin::SceneGraph::m_meshList = {};

engin::SceneGraph::SceneGraph()
	:m_scenePerspectiveCamera(std::make_unique<engin::PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f))),
	m_sceneRenderer(std::make_unique<engin::SceneRenderer>())
{

	auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_sceneFrame = new engin::FrameBuffers(mode->width, mode->height, 1);

	processMesh();
	addTerrain();
	addSquareEntt();
	addDirectionalLit();


}

engin::SceneGraph::~SceneGraph()
{
	delete m_sceneFrame;
}
void engin::SceneGraph::addEntt()
{
	m_enttList.emplace_back();
}

void engin::SceneGraph::addSquareEntt()
{
	m_enttList.emplace_back("Square-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Square"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addCubeEntt()
{
	m_enttList.emplace_back("Cube-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Cube"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addTriangleEntt()
{
	m_enttList.emplace_back("Triangle-" +std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Triangle"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addBase()
{
	m_enttList.emplace_back("Base-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Base");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Square"));
	auto compo = m_enttList[m_enttList.size() - 1].getComponent<TransformComponent>();
	compo->m_scale = { 50.f,50.0f,0.f };
}

void engin::SceneGraph::addDirectionalLit(){
	m_enttList.emplace_back("Light-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Light");
	m_enttList[m_enttList.size() - 1].addComponent<LightComponent>(engin::LightType::DIRECTIONAL);
	auto compo = m_enttList[m_enttList.size() - 1].getComponent<TransformComponent>();
	compo->m_rotation = { 270.f,30.f,0.f };
	currentYentt = &m_enttList[m_enttList.size() - 1];
}
void engin::SceneGraph::addPointLit(){
	m_enttList.emplace_back("Light-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Light");
	m_enttList[m_enttList.size() - 1].addComponent<LightComponent>(engin::LightType::POINT);
	currentYentt = &m_enttList[m_enttList.size() - 1];
}
void engin::SceneGraph::addSpotLit() {
	m_enttList.emplace_back("Light-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Light");
	m_enttList[m_enttList.size() - 1].addComponent<LightComponent>(engin::LightType::SPOT);
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addTerrain()
{
	m_enttList.emplace_back("Terrain-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Terrain");
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<TerrainComponent>();
	auto terrain = m_enttList[m_enttList.size() - 1].getComponent<TerrainComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(&terrain->m_terrain->m_terrainMesh);
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::sceneUpdate(engin::WindowGL& window, float dtime)
{
	m_scenePerspectiveCamera->mvCam(window, dtime);
	m_scenePerspectiveCamera->cameraCalculation();
}

void engin::SceneGraph::drawScene(engin::WindowGL& window, float dtime)
{
	m_sceneRenderer->render(this);
}

void engin::SceneGraph::ImGuiWindows()
{
	ImGui::Begin("Scene Graph");
	ImGui::Text("Entities : \n");
	if (ImGui::Button("Main Camera"))
	{
		currentYentt = nullptr;
		CameraSettings = true;
	}
	for (auto& entt : m_enttList)
	{
		if (entt.getComponent<TagComponent>()->m_tagName != "Base")
		{
			if (ImGui::Button(entt.enttName.c_str()))
			{
				CameraSettings = false; currentYentt = &entt;
			}
		}	
	}
	ImGui::End();

	ImGui::Begin("Properties");

	if (currentYentt)
	{
		for (auto& component : currentYentt->m_yenttComponents)
		{
			component->ImGuiWindow();
		}
	}
	else if(CameraSettings){
		m_scenePerspectiveCamera->imGuiWindows();
	}


	ImGui::End();

}

void engin::SceneGraph::processMesh()
{
	m_meshList["Square"] = new engin::Meshes(engin::square, engin::squareIndex,"Square");
	m_meshList["Triangle"] = new engin::Meshes(engin::triangle, {}, "Triangle");
	m_meshList["Cube"] = new engin::Meshes(engin::block, engin::blockIndex,"Cube");

}

void engin::SceneGraph::deleteMesh() {

	for (auto& mesh : m_meshList)
	{
		mesh.second->deleteBuffers();
	}
}
