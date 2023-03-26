#ifndef YENTT_H
#define YENTT_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../ImGui/ImguiLayer.h"
#include <functional>
#include <string>
#include "Meshes.h"
#include "Texture.h"
#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace engin {

	enum class LightType : uint16_t
	{
		DIRECTIONAL = 0,POINT,SPOT
	};

	struct ColorMaterial {
		glm::vec3 m_ambient = { 0.1f,0.1f,0.1f }, m_diffuse = { 0.8f,0.8f,0.8f }, m_specular = {1.f,1.f,1.f};
	};

	struct TextureMaterial {
		Texture* m_diffuse = nullptr;
		Texture* m_specular = nullptr;
	};
	struct Component {
		std::string m_compId;

		Component(const std::string& compId) : m_compId(compId){

		}
		virtual ~Component() {};
		virtual bool ImGuiWindow() = 0;
	};

	struct TransformComponent : public Component
	{
		glm::vec3 m_position, m_rotation, m_scale;

		TransformComponent()
			:Component(typeid(TransformComponent).name()),m_position(0.f),m_rotation(0.f), m_scale(1.f)
		{

		}
		~TransformComponent(){}

		glm::mat4 getModelMatrix()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f)) *
				glm::rotate(glm::mat4(1.f), glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f)) *
				glm::rotate(glm::mat4(1.f), glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));

			return glm::translate(glm::mat4(1.f), m_position) * rotation * glm::scale(glm::mat4(1.f), m_scale);
		}

		glm::mat4 getNormalMatrix()
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.f), glm::radians(m_rotation.z), glm::vec3(0.f, 0.f, 1.f)) *
				glm::rotate(glm::mat4(1.f), glm::radians(m_rotation.y), glm::vec3(0.f, 1.f, 0.f)) *
				glm::rotate(glm::mat4(1.f), glm::radians(m_rotation.x), glm::vec3(1.f, 0.f, 0.f));

			return glm::scale(glm::mat4(1.f), glm::vec3(1.f / m_scale.x, 1.f / m_scale.y, 1.f / m_scale.z)) * rotation;
		}

		bool ImGuiWindow()
		{
			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Transform Component", &closable_group))
			{
				ImGui::DragFloat3("Position", &m_position.x, 0.1f, -100.f, 100.f, "%.2f");
				ImGui::DragFloat3("Rotation", &m_rotation.x, 0.1f, -360.f, 360.f, "%.2f");
				ImGui::DragFloat3("Scale", &m_scale.x, 0.1f, 1.f, 10.f, "%.2f");
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;

		}
	};

	struct MeshComponent : public Component
	{
		engin::Meshes* m_meshData;

		MeshComponent(engin::Meshes* mesh)
			:Component(typeid(MeshComponent).name()),m_meshData(mesh)
		{

		}
		~MeshComponent() {
			delete m_meshData;
		}

		bool ImGuiWindow()
		{
			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Mesh Component"))
			{
				ImGui::Text("Mesh : %s", m_meshData->m_meshName.c_str());
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;
		}
	};

	struct MaterialComponent : public Component
	{
		engin::ColorMaterial m_colMaterial;
		engin::TextureMaterial m_texMaterial;
		float m_shininess = 12.f;
		bool isTexture = false;

		MaterialComponent()
			:Component(typeid(MaterialComponent).name())
		{

		}
		~MaterialComponent()
		{

		}

		bool ImGuiWindow()
		{

			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Material Component", &closable_group))
			{
				ImGui::Checkbox("Texture", &isTexture);
				if (!isTexture)
				{
					ImGui::ColorEdit3("Material ambient color", &m_colMaterial.m_ambient.x);
					ImGui::ColorEdit3("Material diffuse color", &m_colMaterial.m_diffuse.x);
					ImGui::ColorEdit3("Material specular color", &m_colMaterial.m_specular.x);
				}
				else {
					std::string nameDiff = (m_texMaterial.m_diffuse == nullptr) ? "None" : m_texMaterial.m_diffuse->m_texName;
					if (ImGui::BeginCombo("Diffuse texture", nameDiff.c_str()))
					{
						for (auto& data : Texture::m_textureList)
						{
							bool is_selected = (nameDiff == data);
							if (ImGui::Selectable(data.c_str(), is_selected))
							{
								if (data == "None")
								{
									if (m_texMaterial.m_diffuse)
										delete m_texMaterial.m_diffuse;
									m_texMaterial.m_diffuse = nullptr;
								}
								else {
									if (m_texMaterial.m_diffuse && data != m_texMaterial.m_diffuse->m_texName)
									{
										delete m_texMaterial.m_diffuse;
										uint32_t id = Texture::getTexture(data);
										if (id == 0)
											m_texMaterial.m_diffuse = new Texture(data);
										else
											m_texMaterial.m_diffuse = new Texture(data, id);
									}
									else if (!m_texMaterial.m_diffuse)
									{
										uint32_t id = Texture::getTexture(data);
										if (id == 0)
											m_texMaterial.m_diffuse = new Texture(data);
										else
											m_texMaterial.m_diffuse = new Texture(data, id);
									}
								}
								if (is_selected) ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
					std::string nameSpec = m_texMaterial.m_specular == nullptr ? "None" : m_texMaterial.m_specular->m_texName;
					if (ImGui::BeginCombo("Specular texture", nameSpec.c_str()))
					{
						for (auto& data : Texture::m_textureList)
						{
							bool is_selected = (nameSpec == data);

							if (ImGui::Selectable(data.c_str(), is_selected))
							{
								if (data == "None")
								{
									if (m_texMaterial.m_specular)
										delete m_texMaterial.m_specular;
									m_texMaterial.m_specular = nullptr;
								}
								else {
									if (m_texMaterial.m_specular && data != m_texMaterial.m_specular->m_texName)
									{
										delete m_texMaterial.m_specular;
										uint32_t id = Texture::getTexture(data);
										if (id == 0)
											m_texMaterial.m_specular = new Texture(data);
										else
											m_texMaterial.m_specular = new Texture(data, id);
									}
									else if (!m_texMaterial.m_specular)
									{
										uint32_t id = Texture::getTexture(data);
										if (id == 0)
											m_texMaterial.m_specular = new Texture(data);
										else
											m_texMaterial.m_specular = new Texture(data, id);
									}
								}

								if (is_selected) ImGui::SetItemDefaultFocus();
							}
						}
						ImGui::EndCombo();
					}
				}
				ImGui::DragFloat("Shinniness", &m_shininess, 1.f, 0.f, 255.f, "%.2f");
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;
		}
	};

	struct LightComponent:public Component
	{
		engin::LightType m_litTyp;
		glm::vec3 m_litColor = {1.f,1.f,1.f};
		float m_intensity = 0.8f,m_radius = 3.f,m_innerAngle = 0.8f,m_outerAngle = 0.7f;

		LightComponent(engin::LightType typ) : Component(typeid(LightComponent).name()), m_litTyp(typ) {}
		~LightComponent(){}

		glm::vec3 lightDirection(const glm::vec3& rotation)
		{
			return 
			{ cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y)),
				sin(glm::radians(rotation.x)),
				cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y)) };
		}

		bool ImGuiWindow()
		{
			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Light Component", &closable_group))
			{
				std::vector<std::string> litVal = { "Directional Light","Point Light","Spot Light" };
				bool is_selected;
				if (ImGui::BeginCombo("Light Type", litVal[(uint16_t)m_litTyp].c_str()))
				{
					is_selected = (litVal[(uint16_t)m_litTyp].c_str() == "Directional Light");
					if (ImGui::Selectable("Directional Light", is_selected))
					{
						m_litTyp = LightType::DIRECTIONAL;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					is_selected = (litVal[(uint16_t)m_litTyp].c_str() == "Point Light");
					if (ImGui::Selectable("Point Light", is_selected))
					{
						m_litTyp = LightType::POINT;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					is_selected = (litVal[(uint16_t)m_litTyp].c_str() == "Spot Light");
					if (ImGui::Selectable("Spot Light", is_selected))
					{
						m_litTyp = LightType::SPOT;
						if (is_selected) ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				ImGui::ColorEdit3("Light color", &m_litColor.x);
				ImGui::DragFloat("Intensity", &m_intensity, 0.001f, 0.f, 1.f, "%.4f");

				if (m_litTyp != engin::LightType::DIRECTIONAL)
				{
					ImGui::DragFloat("Radius", &m_radius, 0.5f, 0.f, 20.f, "%.2f");
					if (m_litTyp == engin::LightType::SPOT)
					{
						ImGui::DragFloat("Inner Angle", &m_innerAngle, 0.001f, 0.f, 1.f, "%.4f");
						ImGui::DragFloat("Outer Angle", &m_outerAngle, 0.001f, 0.f, 1.f, "%.4f");
					}
				}
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;
		}
	};

	struct TagComponent : public Component {
		static std::vector<std::string> ms_tagList;
		std::string m_tagName;

		TagComponent(const std::string& tagName):Component(typeid(TagComponent).name()),m_tagName(tagName){}
		~TagComponent() {}

		bool ImGuiWindow()
		{
			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Tag Component", &closable_group))
			{
				ImGui::Text("Tag : %s", m_tagName.c_str());
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;
		}
	};

	struct TerrainComponent : public Component
	{
		TerrainGeneration* m_terrain;

		TerrainComponent():Component(typeid(TerrainComponent).name()),m_terrain(new TerrainGeneration()){}
		~TerrainComponent(){
			delete m_terrain;
		}
		bool ImGuiWindow()
		{
			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Terrain Component", &closable_group))
			{
				ImGui::Text("Terrain Component: \n");
				ImGui::Checkbox("Flat Edges", &m_terrain->isFallOff);
				ImGui::DragFloat("Amplitude", &m_terrain->m_amp, 0.01f, 0.f, 200.f, "%.2f");
				ImGui::DragFloat("Persistance", &m_terrain->m_persistance, 0.001f, 0.0001f, 1.f, "%.4f");
				ImGui::DragFloat("Lucnarity", &m_terrain->m_lucnarity, 0.01f, 0.f, 10.f, "%.2f");

				ImGui::DragFloat("Scale", &m_terrain->m_scale, 0.01f, 1.f, 100.f, "%.4f");

				ImGui::DragInt("Octave", &m_terrain->m_octave, 1, 0, 5);
				ImGui::DragFloat("X-Offset", &m_terrain->m_xOffset, 0.1f, 0.f, 50.f);
				ImGui::DragFloat("Y-Offset", &m_terrain->m_yOffset, 0.1f, 0.f, 50.f);
				ImGui::DragInt("Plane width", &m_terrain->m_width, 1, 2, 200.f);
				ImGui::DragInt("Plane Height", &m_terrain->m_height, 1, 2, 200.f);
				ImGui::DragFloat("Fall off Y", &m_terrain->fallOffe, 0.001f, -1.f, 1.f, "%.4f");

				if (ImGui::Button("Generate"))
				{
					m_terrain->generateTerrain();
				}
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;
		}

	};

	struct RenderComponent : public Component
	{
		bool m_render = true,m_depth = true, m_stencil = false,m_dMask = true,m_fCull = false,m_blend = false;

		uint16_t m_dFunc = GL_LESS, m_sFunc = GL_ALWAYS,
			m_sOpFace = GL_FRONT, m_sFuncFace = GL_FRONT_AND_BACK, m_sMaskFace = GL_FRONT_AND_BACK,
			m_cullFace = GL_BACK, m_frontFace = GL_CCW,
			m_srcColFac = GL_SRC_ALPHA,
			m_dstColFac = GL_ONE_MINUS_SRC_ALPHA,
			m_colFunc = GL_FUNC_ADD;
		glm::vec4 m_constCol = glm::vec4(1);
		int32_t m_sMask = 0,m_sRefVal = 1,m_sRefMask = 0xFF;



		RenderComponent() : Component(typeid(RenderComponent).name()) {}
		~RenderComponent(){}

		bool ImGuiWindow()
		{
			std::array<std::string, 8> funName = {
							"NEVER",
							"LESS",
							"EQUAL",
							"LEQUAL",
							"GREATER",
							"NOTEQUAL",
							"GEQUAL",
							"ALWAYS",
			};
			std::array<std::string, 5> faceName = {
				"FRONT",
				"BACK",
				"LEFT",
				"RIGHT",
				"FRONT AND BACK"
			};
			static bool closable_group = true;
			if (ImGui::CollapsingHeader("Render Component", &closable_group))
			{
				ImGui::Checkbox("Render", &m_render);
				ImGui::SameLine(0);
				ImGui::Checkbox("Depth", &m_depth);
				ImGui::SameLine(0);
				ImGui::Checkbox("Stencil", &m_stencil);
				ImGui::SameLine(0);
				ImGui::Checkbox("Face Cull", &m_fCull);
				ImGui::SameLine(0);
				ImGui::Checkbox("Blending", &m_blend);

				if (m_depth)
				{
					ImGui::Text("\n");
					ImGui::Text("Depth Functions: \n");
					ImGui::Checkbox("Mask Value\n\n", &m_dMask);
					funcName("Function Depth", funName, m_dFunc);
				}

				if (m_stencil)
				{
					ImGui::Text("\n");
					ImGui::Text("Stencil Functions: \n");
					ImGui::DragInt("Mask value", &m_sMask, 1.f, 0, 255);
					ImGui::DragInt("Reference value", &m_sRefVal, 1.f, 0, 255);
					ImGui::DragInt("Reference Mask value", &m_sRefMask, 1.f, 0, 255);

					funcName("Function Stencil", funName, m_sFunc);
					faceStencil("Mask Face Stencil", faceName, m_sMaskFace);
					faceStencil("Operation Face Stencil", faceName, m_sOpFace);
					faceStencil("Function Face Stencil", faceName, m_sFuncFace);
				}

				if (m_fCull)
				{
					ImGui::Text("\n");
					std::array<std::string, 2> fFace = {
						"Clockwise",
						"Counter Clockwise"
					};
					ImGui::Text("Face culling Functions: \n");
					if (ImGui::BeginCombo("Cull Face", faceName[m_cullFace - 0x0404].c_str()))
					{
						for (uint16_t i = 0; i < 2; i++)
						{
							bool is_selected = (faceName[m_cullFace - 0x0404] == faceName[i]);
							if (ImGui::Selectable(faceName[i].c_str(), is_selected))
							{
								m_cullFace = 0x0404 + i;
								if (is_selected) ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndCombo();
					}
					if (ImGui::BeginCombo("Front Facing", fFace[m_frontFace - 0x0900].c_str()))
					{
						for (uint16_t i = 0; i < 2; i++)
						{
							bool is_selected = (fFace[m_frontFace - 0x0900] == fFace[i]);
							if (ImGui::Selectable(fFace[i].c_str(), is_selected))
							{
								m_frontFace = 0x0900 + i;
								if (is_selected) ImGui::SetItemDefaultFocus();
							}
						}

						ImGui::EndCombo();
					}
				}
				if (m_blend)
				{
					ImGui::Text("\n");
					ImGui::Text("Blending Functions: \n");
					std::array<std::string, 19> facName = {
						"Zero","One",
						"Source Color","One Minus Source Color","Source Alpha","One Minus Source Alpha",
						"Destnation Alpha","One Minus Destination Alpha","Destination Color","One Minus Destination Color","Source Alpha Saturate",
						"Constant Color","One Minus Constant Color","Constant Alpha","One Minus Constant Alpha",
						"Source 1 Alpha",
						"Source 1 Color","One Minus Source 1 Color","One Minus Source 1 Alpha"
					};
					std::array<std::string, 6> bFuncName = {
						"Addition",
						"Minimum",
						"Maximum",
						"Blend",
						"Subtract",
						"Reverse Subtract"
					};
					factorSelection("Source Color Factor", facName, m_srcColFac);
					factorSelection("Destination Color Factor", facName, m_dstColFac);
					ImGui::ColorEdit4("Constant Color", &m_constCol.x);
					bFuncSelection("Color Function", bFuncName, m_colFunc);
				}
			}
			if (closable_group == false)
			{
				closable_group = true;
				return !closable_group;
			}
			return true;
		}

		private:
			void factorSelection(const char* name, std::array<std::string, 19>& fName, uint16_t& fac)
			{
				std::array<uint16_t, 19> arr = {
					GL_ZERO,GL_ONE,GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,
			GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,GL_SRC_ALPHA_SATURATE,GL_CONSTANT_COLOR,GL_ONE_MINUS_CONSTANT_COLOR,GL_CONSTANT_ALPHA,
			GL_ONE_MINUS_CONSTANT_ALPHA,GL_SRC1_ALPHA,GL_SRC1_COLOR,GL_ONE_MINUS_SRC1_COLOR,GL_ONE_MINUS_SRC1_ALPHA
				};
				auto tmpFac = std::find(arr.begin(), arr.end(), fac) - arr.begin();
				const char* mainName = fName[(uint16_t)tmpFac].c_str();
				if (ImGui::BeginCombo(name, mainName))
				{
					for (uint16_t i = 0; i < 19; i++)
					{
						bool is_selected = (fName[(uint16_t)tmpFac] == fName[i]);
						if (ImGui::Selectable(fName[i].c_str(), is_selected))
						{
							fac = arr[i];
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
			}
			void bFuncSelection(const char* name, std::array<std::string, 6>& fName, uint16_t& func)
			{
				if (ImGui::BeginCombo(name, fName[func - 0x8006].c_str()))
				{
					for (uint16_t i = 0; i < 6; i++)
					{
						bool is_selected = (fName[func - 0x8006] == fName[i]);
						if (ImGui::Selectable(fName[i].c_str(), is_selected))
						{
							if (i != 3) func = 0x8006 + i;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
			}
			void faceStencil(const char* name, std::array<std::string, 5>& fName, uint16_t& face)
			{
				
				if (ImGui::BeginCombo(name, fName[face - 0x0404].c_str()))
				{
					for (uint16_t i = 0; i < 5; i++)
					{
						bool is_selected = (fName[face - 0x0404] == fName[i]);
						if (ImGui::Selectable(fName[i].c_str(), is_selected))
						{
							face = 0x0404 + i;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
			}

			void funcName(const char* name, std::array<std::string, 8>& fName, uint16_t& func)
			{
				if (ImGui::BeginCombo(name, fName[func - 512].c_str()))
				{
					for (uint16_t i = 0; i < 8; i++)
					{
						bool is_selected = (fName[func - 512] == fName[i]);
						if (ImGui::Selectable(fName[i].c_str(), is_selected))
						{
							func = 512 + i;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}
			}
	};

	class Yentt {
	private:
		static std::vector<Component*>::iterator tempComp;
		bool m_yenttState = false;

	public:
		std::vector<Component*> m_yenttComponents;
		std::string enttName;
		Yentt(const std::string& name = "Entity")
			:enttName(name)
		{
			addComponent<TransformComponent>();
		}
		~Yentt(){}

		bool isStatic() const {
			return m_yenttState;
		}

		void mkStatic(bool staticFlg = false) {
			m_yenttState = staticFlg;
		}

		template<typename typ, typename... Args>
		void addComponent(Args&&... args)
		{
			if(!hasComponent<typ>())
				m_yenttComponents.push_back(new typ(std::forward<Args>(args)...));
		}

		template<typename typ>
		typ* getComponent() {
			if (hasComponent<typ>())
			{	
				return (typ*)(*tempComp);
			}
				
			return nullptr;
		}

		template<typename typ>
		bool removeComponent()
		{
			if (hasComponent<typ>())
			{
				m_yenttComponents.erase(tempComp);
				return true;
			}

			return false;
		}

		template<typename T>
		bool hasComponent()
		{
			auto iterator = std::find_if(m_yenttComponents.begin(), m_yenttComponents.end(),
				[&](Component* comp)->bool {
					if (comp->m_compId == typeid(T).name())
					return true;
			return false;
				}
			);
			if (iterator != m_yenttComponents.end())
			{
				tempComp = iterator;
				return true;
			}
				
			return false;
		}

		void compManipulation()
		{
			addComponentGui();
			for (uint32_t i = 0;i < this->m_yenttComponents.size();i++)
			{
				bool flg = this->m_yenttComponents[i]->ImGuiWindow();
				if (!flg)
				{
					
					this->m_yenttComponents.erase(this->m_yenttComponents.begin() + i);
				}
				ImGui::SeparatorText("");
			}
		}

	private:
		void addComponentGui()
		{
			std::string names[7] = {
				"Transform Component",
				"Mesh Component",
				"Material Component",
				"Terrain Component",
				"Render Component",
				"Light Component",
				"Tag Component"
			};
			
			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("my_select_popup");
			int16_t flg = -1;
			if (ImGui::BeginPopup("my_select_popup"))
			{
				for (int i = 0; i < IM_ARRAYSIZE(names); i++)
					if (ImGui::Selectable(names[i].c_str()))
						flg = i;
				ImGui::EndPopup();
			}
			if(flg == 0)
				addComponent<TransformComponent>();
			else if(flg == 1)
				addComponent<MeshComponent>(nullptr);
			else if(flg == 2)
				addComponent<MaterialComponent>();
			else if (flg == 3)
				addComponent<TerrainComponent>();
			else if (flg == 4)
				addComponent<RenderComponent>();
			else if (flg == 5)
				addComponent<LightComponent>(engin::LightType::DIRECTIONAL);
			else if (flg == 6)
				addComponent<TagComponent>("Object");
		}
	};

	

}



#endif //ENTT_H
