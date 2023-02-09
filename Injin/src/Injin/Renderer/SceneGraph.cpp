#include "SceneGraph.h"

engin::Yentt* engin::SceneGraph::currentYentt = nullptr;


engin::Component* engin::Yentt::tempComp = nullptr;
std::unordered_map<std::string, engin::Meshes*> engin::SceneGraph::m_meshList = {};

engin::SceneGraph::SceneGraph()
	:m_scenePerspectiveCamera(std::make_unique<engin::PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f))),
	program("D:\\Coding\\GameEngine\\Injin\\Editor\\assets\\Shaders\\simpleObj.glsl")
{
	processMesh();
	addTriangleEntt();
	addCubeEntt();
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
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Square"));

	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addCubeEntt()
{
	m_enttList.emplace_back("Cube");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Cube"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addTriangleEntt()
{
	m_enttList.emplace_back("Triangle");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(m_meshList.at("Triangle"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}



void engin::SceneGraph::sceneUpdate(engin::WindowGL& window, float dtime)
{
	m_scenePerspectiveCamera->mvCam(window, dtime);
	m_scenePerspectiveCamera->cameraCalculation();
}

void engin::SceneGraph::drawScene(engin::WindowGL& window, float dtime)
{
	program.bindProgram();
	
	program.setUniValueM("viewProj", m_scenePerspectiveCamera->getViewProjMat(), 4);

	for (auto& entt : m_enttList)
	{
		program.setUniValueM("transMat", entt.getComponent<TransformComponent>()->getModelMatrix(), 4);
		program.setUniValuefV("uColor", entt.getComponent<MaterialComponent>()->m_materialColor, 3);
		entt.getComponent<MeshComponent>()->m_meshData->drawMesh();
	}
	program.unbindProgram();
}

void engin::SceneGraph::ImGuiWindows()
{
	ImGui::Begin("Scene Graph");
	ImGui::Text("Entities : \n");
	for (auto& entt : m_enttList)
	{
		if(ImGui::Button(entt.enttName.c_str()))
			currentYentt = &entt;
	}
	ImGui::End();

	ImGui::Begin("Main Camera settings");

	m_scenePerspectiveCamera->imGuiWindows();

	ImGui::End();

	ImGui::Begin("Properties");

	for (auto& component : currentYentt->m_yenttComponents)
	{
		component->ImGuiWindow();
	}

	ImGui::End();

}

void engin::SceneGraph::processMesh()
{
	m_meshList["Square"] = new engin::Meshes(engin::square, engin::squareIndex,"Square");
	m_meshList["Triangle"] = new engin::Meshes(engin::triangle, {}, "Triangle");
	m_meshList["Cube"] = new engin::Meshes(engin::block, engin::blockIndex,"Cube");

}
