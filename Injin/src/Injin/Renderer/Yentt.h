#ifndef YENTT_H
#define YENTT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../ImGui/ImguiLayer.h"
#include <memory>
#include <functional>
#include <string>

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
			ImGui::DragFloat3("Position", &m_position.x, 0.5f, 0.f, 100.f, "%.2f");
			ImGui::DragFloat3("Position", &m_rotation.x, 0.5f, 0.f, 360.f, "%.2f");
			ImGui::DragFloat3("Position", &m_scale.x, 0.5f, 1.f, 10.f, "%.2f");
		}
	};

	class Yentt {
	private:
		std::vector<std::unique_ptr<Component>> m_yenttComponents;
		static Component* tempComp;


	public:
		Yentt(){}
		~Yentt(){}

		template<typename typ, typename... Args>
		void addEntt(Args&&... args)
		{
			if(!hasComponent<typ>())
				m_yenttComponents.push_back(std::make_unique<typ>(std::forward<Args>(args)...));
		}

		template<typename typ>
		typ* getEntt() {
			if (hasComponent<typ>())
			{	
				return (typ*)tempComp;
			}
				
			return nullptr;
		}

		template<typename T>
		bool hasComponent()
		{
			auto iterator = std::find_if(m_yenttComponents.begin(), m_yenttComponents.end(),
				[&](const std::unique_ptr<Component>& comp)->bool {
					if (comp->m_compId == typeid(T).name())
					return true;
			return false;
				}
			);
			if (iterator != m_yenttComponents.end())
			{
				tempComp = iterator->get();
				return true;
			}
				
			return false;
		}
	};

	Component* Yentt::tempComp = nullptr;
}



#endif //ENTT_H
