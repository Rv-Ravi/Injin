#include "SceneGraph.h"

engin::Yentt* engin::SceneGraph::currentYentt = nullptr;
std::vector<std::string> engin::TagComponent::ms_tagList = { "Object","Light","Camera","Base"};

std::vector<engin::Component*>::iterator engin::Yentt::tempComp;
std::unordered_map<std::string, engin::Meshes*> engin::SceneGraph::m_meshList = {};

engin::SceneGraph::SceneGraph()
	:m_scenePerspectiveCamera(std::make_unique<engin::PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f))),
	m_sceneRenderer(std::make_unique<engin::SceneRenderer>())
{
	processMesh();
	addBase();
	addTriangleEntt();
	addCubeEntt();
	addSpotLit();


}

engin::SceneGraph::~SceneGraph()
{
	
}
void engin::SceneGraph::addEntt()
{
	m_enttList.emplace_back();
}

void engin::SceneGraph::addSquareEntt()
{
	m_enttList.emplace_back("Square");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Square"));

	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addCubeEntt()
{
	m_enttList.emplace_back("Cube");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Cube"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addTriangleEntt()
{
	m_enttList.emplace_back("Triangle");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Triangle"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addBase()
{
	m_enttList.emplace_back("Base");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Base");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Square"));
	auto compo = m_enttList[m_enttList.size() - 1].getComponent<TransformComponent>();
	compo->m_scale = { 50.f,50.0f,0.f };
}

void engin::SceneGraph::addDirectionalLit(){
	m_enttList.emplace_back("Light");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Light");
	m_enttList[m_enttList.size() - 1].addComponent<LightComponent>(engin::LightType::DIRECTIONAL);
	currentYentt = &m_enttList[m_enttList.size() - 1];
}
void engin::SceneGraph::addPointLit(){
	m_enttList.emplace_back("Light");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Light");
	m_enttList[m_enttList.size() - 1].addComponent<LightComponent>(engin::LightType::POINT);
	currentYentt = &m_enttList[m_enttList.size() - 1];
}
void engin::SceneGraph::addSpotLit() {
	m_enttList.emplace_back("Light");
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Light");
	m_enttList[m_enttList.size() - 1].addComponent<LightComponent>(engin::LightType::SPOT);
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
