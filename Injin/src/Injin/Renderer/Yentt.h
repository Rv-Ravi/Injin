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

namespace engin {


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
		glm::vec3 m_materialColor;
		MaterialComponent(const glm::vec3& color = { 0.2f,0.2f,0.2f })
			:Component(typeid(MaterialComponent).name()),m_materialColor(color)
		{

		}
		~MaterialComponent()
		{

		}
		void ImGuiWindow()
		{
			ImGui::Text("Material Component: \n");
			ImGui::ColorEdit3("Material Color",&m_materialColor.x);
		}
	};

	struct TextureComponent : public Component
	{
		engin::Texture* texture;
		TextureComponent(const std::string& name)
			:Component(typeid(TextureComponent).name()), texture(new Texture(name))
		{
		}
		~TextureComponent()
		{
			delete texture;
		}
		void ImGuiWindow()
		{
			ImGui::Text("Texture Component: \n");
			ImGui::Text("Texture used : %s", texture->getName().c_str());
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
