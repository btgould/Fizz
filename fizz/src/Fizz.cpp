#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Polygon.hpp"
#include "Simplex.hpp"
#include "PhysicsEnvironment.hpp"

using namespace Nutella;
using namespace Fizz;

class FizzLayer : public Layer {
  public:
	FizzLayer()
		: Layer("Fizziks Simulation"),
		  m_CameraController((float) Application::get().getWindow().GetWidth() /
								 (float) Application::get().getWindow().GetHeight(),
							 true) {

		Ref<PhysicsObject> moved =
			CreateRef<PhysicsObject>(CreateRef<Polygon>(PolygonType::SQUARE));
		moved->ApplyImpulse(glm::vec2(0.01f, 0.0f));

		Ref<PhysicsObject> wallRight = CreateRef<PhysicsObject>(
			CreateRef<Polygon>(PolygonType::SQUARE),
			Transform({glm::vec2(1.0f, 0.0f), 0.0f, glm::vec2(0.2f, 1.0f)}));
		wallRight->SetInvMass(0.0f);

		Ref<PhysicsObject> wallLeft = CreateRef<PhysicsObject>(
			CreateRef<Polygon>(PolygonType::SQUARE),
			Transform({glm::vec2(-1.0f, 0.0f), 0.0f, glm::vec2(0.2f, 1.0f)}));
		wallLeft->SetInvMass(0.0f);

		m_PhysicsEnv.Add(moved);
		m_PhysicsEnv.Add(wallRight);
		m_PhysicsEnv.Add(wallLeft);
	}

	virtual void OnUpdate(Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		m_PhysicsEnv.Update();

		Renderer::BeginScene(m_CameraController.GetCamera());
		m_PhysicsEnv.Render();
		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Fizziks Debug");

		ImGuiShowPhysicsObjects();
		ImGui::Separator();
		ImGuiShowCollisions();

		ImGui::End();
	}

	virtual void OnEvent(Event& event) override { m_CameraController.OnEvent(event); }

  private:
	void ImGuiShowPhysicsObjects() {
		for (uint32_t i = 0; i < m_PhysicsEnv.GetObjects().size(); i++) {
			Ref<PhysicsObject>& object = m_PhysicsEnv.GetObjects()[i];

			glm::vec2 localPos = object->GetPos();
			float localRot = object->GetRot();
			glm::vec2 localScale = object->GetScale();

			ImGui::PushID(i);
			ImGui::Text("Polygon %u:", i + 1);
			ImGui::SliderFloat2("Position", glm::value_ptr(localPos), -2.0f, 2.0f);
			ImGui::SliderFloat("Rotation", &localRot, 0.0f, 2 * 3.1415f);
			ImGui::SliderFloat2("Scale", glm::value_ptr(localScale), 0.0f, 2.0f);
			ImGui::PopID();

			object->SetPos(localPos);
			object->SetRot(localRot);
			object->SetScale(localScale);
		}
	}

	void ImGuiShowCollisions() {
		std::vector<Collision>& collisions = m_PhysicsEnv.GetCollisions();

		ImGui::Text("Collisions");
		if (collisions.size() == 0) {
			ImGui::Text("None");
		} else {
			for (uint32_t i = 0; i < collisions.size(); i++) {
				Collision collision = collisions[i];

				ImGui::PushID(i);
				ImGui::Text("Collision %d: ", i + 1);
				ImGui::Text("Penetration Depth: %.3f", collision.penetrationDist);
				ImGui::Text("Min. Translation Vector: <%.3f, %.3f>", collision.MTV.x,
							collision.MTV.y);
				ImGui::PopID();
			}
		}
	}

  private:
	OrthoCamController m_CameraController;
	PhysicsEnvironment m_PhysicsEnv;
};

class Sandbox : public Application {
  public:
	Sandbox() { PushLayer(new FizzLayer()); }

	~Sandbox() {}
};

Application* Nutella::CreateApplication() {
	return new Sandbox();
}
