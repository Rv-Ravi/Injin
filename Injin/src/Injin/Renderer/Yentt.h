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

namespace engin {

	enum class LightType : uint16_t
	{
		DIRECTIONAL = 0,POINT,SPOT
	};

	struct ColorMaterial {
		glm::vec3 m_ambient = { 0.1f,0.1f,0.1f }, m_diffuse = { 0.8f,0.8f,0.8f }, m_specular = {1.f,1.f,1.f};
	};

	struct TextureMaterial {
		std::string m_diffuse = "";
		std::string m_specular = "";
	};
	struct Component {
		std::string m_compId;

		Component(const std::string& compId) : m_compId(compId){

		}
		virtual ~Component() {};
		virtual void ImGuiWindow() = 0;
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

		void ImGuiWindow()
		{
			ImGui::Text("Transform: \n");
			ImGui::DragFloat3("Position", &m_position.x, 0.1f, -100.f, 100.f, "%.2f");
			ImGui::DragFloat3("Rotation", &m_rotation.x, 0.1f, -360.f, 360.f, "%.2f");
			ImGui::DragFloat3("Scale", &m_scale.x, 0.1f, 1.f, 10.f, "%.2f");
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

		}

		void ImGuiWindow()
		{
			ImGui::Text("Mesh Component: \n");
			ImGui::Text("Mesh : %s",m_meshData->m_meshName.c_str());
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

		void ImGuiWindow()
		{

			ImGui::Text("Material Component: \n");
			ImGui::Checkbox("Texture", &isTexture);
			if (!isTexture)
			{
				ImGui::ColorEdit3("Material ambient color", &m_colMaterial.m_ambient.x);
				ImGui::ColorEdit3("Material diffuse color", &m_colMaterial.m_diffuse.x);
				ImGui::ColorEdit3("Material specular color", &m_colMaterial.m_specular.x);
			}
			else {
				if (ImGui::BeginCombo("##Diffuse texture", m_texMaterial.m_diffuse.c_str()))
				{
					for (auto& data:Texture::m_textureList)
					{
						bool is_selected = (m_texMaterial.m_diffuse.c_str() == data.first.c_str());
						if (ImGui::Selectable(data.first.c_str(), is_selected))
						{
							m_texMaterial.m_diffuse = data.first;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
				if (ImGui::BeginCombo("##Specular texture", m_texMaterial.m_specular.c_str()))
				{
					for (auto& data : Texture::m_textureList)
					{
						bool is_selected = (m_texMaterial.m_specular.c_str() == data.first.c_str());
						if (ImGui::Selectable(data.first.c_str(), is_selected))
						{
							m_texMaterial.m_specular = data.first;
							if (is_selected) ImGui::SetItemDefaultFocus();
						}
					}
					ImGui::EndCombo();
				}
			}
			ImGui::DragFloat("Shinniness", &m_shininess, 1.f, 0.f, 255.f, "%.2f");
			
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

		void ImGuiWindow()
		{
			ImGui::Text("Light Component: \n");
			std::vector<std::string> litVal = { "Directional Light","Point Light","Spot Light" };
			bool is_selected;
			if (ImGui::BeginCombo("##Light Type", litVal[(uint16_t)m_litTyp].c_str()))
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
	};

	struct TagComponent : public Component {
		static std::vector<std::string> ms_tagList;
		std::string m_tagName;

		TagComponent(const std::string& tagName):Component(typeid(TagComponent).name()),m_tagName(tagName){}
		~TagComponent() {}

		void ImGuiWindow()
		{
			ImGui::Text("Tag Component: \n");
			ImGui::Text("Tag :        %s", m_tagName.c_str());
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
	};

	

}



#endif //ENTT_H
