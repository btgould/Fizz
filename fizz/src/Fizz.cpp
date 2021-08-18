#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Polygon.hpp"
#include "Simplex.hpp"

using namespace Nutella;
using namespace Fizz;

class FizzLayer : public Layer {
  public:
	FizzLayer()
		: Layer("Fizziks Simulation"),
		  m_CameraController((float) Application::get().getWindow().GetWidth() /
								 (float) Application::get().getWindow().GetHeight(),
							 true) {

		// m_PhysicsObjects.push_back(
		// 	CreateRef<Polygon>(PolygonType::SQUARE, 0.2, glm::vec2(0.6f, 0.1f)));

		// m_PhysicsObjects.push_back(
		// 	CreateRef<Polygon>(PolygonType::SQUARE, 0.2, glm::vec2(1.0f, 0.0f)));

		m_PhysicsObjects.push_back(CreateRef<Polygon>(
			std::vector<glm::vec2>(
				{{-0.25, -0.25}, {0.25, -0.25}, {0.25, 0.25}, {0, 0.5}, {-0.25, 0.25}}),
			glm::vec2(0.2, -0.28)));

		m_PhysicsObjects.push_back(
			CreateRef<Polygon>(PolygonType::TRIANGLE, 0.3, glm::vec2(1.0f, 0.0f)));
	}

	virtual void OnUpdate(Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		Renderer::BeginScene(m_CameraController.GetCamera());
		for (Ref<PhysicsObject>& object : m_PhysicsObjects)
			object->Render();
		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Fizziks Debug");

		ImGuiShowPhysicsObjects();
		ImGui::Separator();

		Collision collision = GJKGetCollision(m_PhysicsObjects[0], m_PhysicsObjects[1]);

		ImGui::Text("Collision");
		if (collision.exists) {
			ImGui::Text("Colliding: true");
			ImGui::Text("Penetration Depth: %.3f", collision.penetrationDist);
			ImGui::Text("Min. Translation Vector: <%.3f, %.3f>", collision.MTV.x, collision.MTV.y);
		} else {
			glm::vec2 dist = GJKDistance(m_PhysicsObjects[0], m_PhysicsObjects[1]);

			ImGui::Text("Colliding: false");
			ImGui::Text("Distance: <%.3f, %.3f>", dist.x, dist.y);
		}

		ImGui::End();
	}

	virtual void OnEvent(Event& event) override { m_CameraController.OnEvent(event); }

  private:
	void ImGuiShowPhysicsObjects() {
		for (uint32_t i = 0; i < m_PhysicsObjects.size(); i++) {
			Ref<PhysicsObject>& object = m_PhysicsObjects[i];

			glm::vec2 localPos = object->GetPos();
			float localRot = object->GetRot();
			glm::vec2 localScale = object->GetScale();

			ImGui::PushID(i);
			ImGui::Text("Polygon %u:", i + 1);
			ImGui::SliderFloat2("Position", glm::value_ptr(localPos), -2.0f, 2.0f);
			ImGui::SliderFloat("Rotation", &localRot, 0.0f, 2 * 3.1415f);
			ImGui::SliderFloat2("Scale", glm::value_ptr(localScale), 0.0f, 2.0f);
			ImGui::PopID();

			object->SetTRS(localPos, localRot, localScale);
		}
	}

  private:
	OrthoCamController m_CameraController;

	std::vector<Ref<PhysicsObject>> m_PhysicsObjects;
};

class Sandbox : public Application {
  public:
	Sandbox() { PushLayer(new FizzLayer()); }

	~Sandbox() {}
};

Application* Nutella::CreateApplication() {
	return new Sandbox();
}
