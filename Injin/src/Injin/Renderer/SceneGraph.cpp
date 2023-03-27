#include "SceneGraph.h"
#include <algorithm>

engin::Yentt* engin::SceneGraph::currentYentt = nullptr;
std::vector<std::string> engin::TagComponent::ms_tagList = { "Object","Light","Camera","Base","Terrain"};

std::vector<engin::Component*>::iterator engin::Yentt::tempComp;

engin::SceneGraph::SceneGraph()
	:m_scenePerspectiveCamera(std::make_unique<engin::PerspectiveCamera>(glm::vec3(0.f, 0.f, 0.f))),
	m_sceneRenderer(std::make_unique<engin::SceneRenderer>())
{

	auto mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	m_sceneFrame = new engin::FrameBuffers(mode->width, mode->height, 1);

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
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(new engin::Meshes(engin::square,engin::squareIndex,"Square"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addCubeEntt()
{
	m_enttList.emplace_back("Cube-" + std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(new engin::Meshes(engin::block, engin::blockIndex, "Cube"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addTriangleEntt()
{
	m_enttList.emplace_back("Triangle-" +std::to_string(m_enttList.size()));
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(new engin::Meshes(engin::triangle, {}, "Cube"));
	m_enttList[m_enttList.size() - 1].addComponent<MaterialComponent>();
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
}

void engin::SceneGraph::addDirectionalLit(){
	m_LightEntt.emplace_back("Light-" + std::to_string(m_LightEntt.size()));
	m_LightEntt[m_LightEntt.size() - 1].addComponent<TagComponent>("Light");
	m_LightEntt[m_LightEntt.size() - 1].addComponent<LightComponent>(engin::LightType::DIRECTIONAL);
	auto compo = m_LightEntt[m_LightEntt.size() - 1].getComponent<TransformComponent>();
	compo->m_rotation = { 270.f,30.f,0.f };
	currentYentt = &m_LightEntt[m_LightEntt.size() - 1];
}
void engin::SceneGraph::addPointLit(){
	m_LightEntt.emplace_back("Light-" + std::to_string(m_LightEntt.size()));
	m_LightEntt[m_LightEntt.size() - 1].addComponent<TagComponent>("Light");
	m_LightEntt[m_LightEntt.size() - 1].addComponent<LightComponent>(engin::LightType::POINT);
	currentYentt = &m_LightEntt[m_LightEntt.size() - 1];
}
void engin::SceneGraph::addSpotLit() {
	m_LightEntt.emplace_back("Light-" + std::to_string(m_LightEntt.size()));
	m_LightEntt[m_LightEntt.size() - 1].addComponent<TagComponent>("Light");
	m_LightEntt[m_LightEntt.size() - 1].addComponent<LightComponent>(engin::LightType::SPOT);
	currentYentt = &m_LightEntt[m_LightEntt.size() - 1];
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

void engin::SceneGraph::manipulateEntity()
{
	bool square = false,
		cube = false,
		terrain = false,
		triangle = false,
		base = false,
		dirLight = false,
		pointLight = false,
		spotLight = false;
	if (ImGui::BeginMenu("Add Entity"))
	{

		ImGui::MenuItem("plane", NULL, &square);
		ImGui::MenuItem("Cube", NULL, &cube);
		ImGui::MenuItem("Terrain", NULL, &terrain);
		ImGui::MenuItem("Triangle", NULL, &triangle);
		ImGui::MenuItem("Directional Light", NULL, &dirLight);
		ImGui::MenuItem("Point Light", NULL, &pointLight);
		ImGui::MenuItem("Spot Light", NULL, &spotLight);
		ImGui::EndMenu();
	}
	if (square)
		addSquareEntt();
	if (cube)
		addCubeEntt();
	if (terrain)
		addTerrain();
	if (triangle)
		addTriangleEntt();
	if (dirLight)
		addDirectionalLit();
	if (pointLight)
		addPointLit();
	if (spotLight)
		addSpotLit();

	loadModelGui();
	ImGui::SameLine();
	if (ImGui::Button("Remove Entity"))
		removeEntity();
}

void engin::SceneGraph::removeEntity()
{
	if (currentYentt)
	{
		if (currentYentt->getComponent<TagComponent>()->m_tagName == "Light")
		{
			auto iterator = std::find_if(m_LightEntt.begin(), m_LightEntt.end(), [&](engin::Yentt& yentt)
				{
					if (&yentt == currentYentt)
					return true;
			return false;
				});
			if (iterator != m_LightEntt.end())
				m_LightEntt.erase(iterator);
		}
		else
		{
			auto iterator = std::find_if(m_enttList.begin(), m_enttList.end(), [&](engin::Yentt& yentt)
				{
					if (&yentt == currentYentt)
					return true;
			return false;
				});
			if (iterator != m_enttList.end())
				m_enttList.erase(iterator);
		}
	}
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
	manipulateEntity();
	ImGui::Text("Entities : \n");
	if (ImGui::Button("Main Camera"))
	{
		currentYentt = nullptr;
		CameraSettings = true;
	}
	for (auto& entt : m_LightEntt)
	{
		if (ImGui::Button(entt.enttName.c_str()))
		{
			CameraSettings = false; currentYentt = &entt;
		}
	}
	for (auto& entt : m_enttList)
	{
		if (ImGui::Button(entt.enttName.c_str()))
		{
			CameraSettings = false; currentYentt = &entt;
		}
	}
	ImGui::End();

	ImGui::ShowDemoWindow();

	ImGui::Begin("Properties");
	if (currentYentt)
		currentYentt->compManipulation();
	else if(CameraSettings)
		m_scenePerspectiveCamera->imGuiWindows();
	ImGui::End();

	ImGui::Begin("Editor Panel");
	ImVec2 panelSize = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)m_sceneFrame->getColAtt(0),
		panelSize,
		ImVec2{ 0,1 }, ImVec2{ 1,0 });
	ImGui::End();

}

void engin::SceneGraph::loadModelGui()
{
	using namespace std::filesystem;
	static path mainPath = current_path(), currentPath = mainPath / "assets";
	static std::vector<path> filename = {currentPath };
	if (ImGui::Button("Import Model"))
		ImGui::OpenPopup("File Explorer");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	static std::string fileName;

	if (ImGui::BeginPopupModal("File Explorer", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::SmallButton("../"))
		{
			if (currentPath.parent_path() != mainPath)
				currentPath = currentPath.parent_path();
		}
		ImGui::SameLine(0);
		for (uint32_t i = 0;i < filename.size();i++)
		{
			if (ImGui::SmallButton(filename[i].filename().string().c_str()))
			{
				currentPath = filename[i];
				if(i != filename.size() - 1)
					filename.erase(filename.begin() + i + 1,filename.end());

			}
			if(i != filename.size() - 1)ImGui::SameLine(0);
		}
		ImGui::Separator();

		for (auto& val : directory_iterator{ currentPath })
		{
			auto tmpPath = val.path().filename();
			if (val.is_directory())
			{
				if (ImGui::Button(tmpPath.string().c_str()))
				{
					currentPath /= tmpPath;
					filename.push_back(val.path());
					ImGui::OpenPopup("File Explorer");
				}
					
			}
			else {
				if (ImGui::Button(tmpPath.string().c_str()))
					fileName = tmpPath.string();
					ImGui::OpenPopup("File Explorer");
			}
		}

		ImGui::InputText("File Name", (char*)fileName.c_str(), fileName.size() + 5);
		if (ImGui::Button("OK", ImVec2(120, 0))) { 
			fileName = (currentPath / fileName).string();
			loadModel(fileName);
			fileName.clear();
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { fileName.clear(); ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void engin::SceneGraph::loadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_Triangulate | aiProcess_FlipUVs
	);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		std::cerr << "Error in loading Model\n";
	else
		modelProcessScene(scene, scene->mRootNode);

}

void engin::SceneGraph::modelProcessScene(const aiScene* scene,aiNode* node)
{
	for (uint16_t i = 0; i < node->mNumMeshes; i++)
	{
		modelProcessMesh(scene, scene->mMeshes[node->mMeshes[i]]);
	}
	for (uint16_t j = 0; j < node->mNumChildren; j++)
	{
		modelProcessScene(scene, node->mChildren[j]);
	}
}

void engin::SceneGraph::modelProcessMesh(const aiScene* scene, const aiMesh* mesh)
{
	std::vector<vertexData> meshData;
	std::vector<unsigned int> meshIndicies;
	for (uint32_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertexData data;
		if (mesh->HasPositions())
			data.vertexPoints = { mesh->mVertices[i].x,
				mesh->mVertices[i].y,mesh->mVertices[i].z };
		if (mesh->HasVertexColors(i) && mesh->mColors[0])
			data.vertexColor = { mesh->mColors[0][i].r,mesh->mColors[0][i].g,mesh->mColors[0][i].b };
		else
			data.vertexColor = { 1.f,1.f,1.f };

		if (mesh->mTextureCoords[0])
			data.textureCoord = { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y };
		else
			data.textureCoord = { 0.f,0.f };

		data.vertexNormal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		meshData.push_back(data);

	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			meshIndicies.push_back(face.mIndices[j]);
	}
	engin::Meshes* opMesh = new engin::Meshes(meshData, meshIndicies,mesh->mName.C_Str());
	m_enttList.emplace_back(mesh->mName.C_Str());
	m_enttList[m_enttList.size() - 1].addComponent<MeshComponent>(opMesh);
	m_enttList[m_enttList.size() - 1].addComponent<TagComponent>("Object");
	m_enttList[m_enttList.size() - 1].addComponent<RenderComponent>();
	currentYentt = &m_enttList[m_enttList.size() - 1];
	modelProcessMaterial(scene, mesh, *currentYentt);
}

void engin::SceneGraph::modelProcessMaterial(const aiScene* scene, const aiMesh* mesh, engin::Yentt& entt)
{

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		entt.addComponent<MaterialComponent>();
		auto mate = entt.getComponent<MaterialComponent>();

		float shinny;
		aiColor4D content;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &content);
		mate->m_colMaterial.m_ambient = { content.r,content.g,content.b };
		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &content);
		mate->m_colMaterial.m_diffuse = { content.r,content.g,content.b };
		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &content);
		mate->m_colMaterial.m_specular = { content.r,content.g,content.b };
		material->Get(AI_MATKEY_SHININESS, shinny);
		if(shinny < 1) mate->m_shininess = 1;

		bool flg1 = modelTextureProcess(material, aiTextureType_DIFFUSE,entt);
		bool flg2 = modelTextureProcess(material, aiTextureType_SPECULAR,entt);
		//modelTextureProcess(material, aiTextureType_NORMALS);

		if (flg1 || flg2)
			mate->isTexture = true;
	}
}

bool engin::SceneGraph::modelTextureProcess(aiMaterial* meshMaterial, aiTextureType type,engin::Yentt& entt)
{
	uint32_t count = meshMaterial->GetTextureCount(type);
	auto mate = entt.getComponent<MaterialComponent>();
	bool flg = false;
	if (count) {
		aiString path;
		meshMaterial->GetTexture(type, 0, &path);
		uint32_t id = Texture::getTexture(path.C_Str());
		if (id == 0)
		{	
			if(type == 1)
				mate->m_texMaterial.m_diffuse = new Texture(path.C_Str());
			else if(type ==2)
				mate->m_texMaterial.m_specular = new Texture(path.C_Str());
		}
		else
		{
			if (type == 1)
				mate->m_texMaterial.m_diffuse = new Texture(path.C_Str(), id);
			else if (type == 2)
				mate->m_texMaterial.m_specular = new Texture(path.C_Str(), id);
		}
			
		auto iterator = std::find_if(Texture::m_textureList.begin(), Texture::m_textureList.end(), [&](std::string& str) {
				if (str == path.C_Str())
					return true;
				return false;
			});
		if (iterator == Texture::m_textureList.end())
		{
			Texture::m_textureList.push_back(path.C_Str());
		}
				
		flg = true;
	}
	return flg;
}
