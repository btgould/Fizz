#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Nutella/Core/Core.hpp"
#include "Objects/PhysicsObject.hpp"
#include "Objects/Polygon.hpp"
#include "Objects/Circle.hpp"
#include "Collisions/Simplex.hpp"
#include "PhysicsEnvironment.hpp"

using namespace Nutella;
using namespace Fizz;

class FizzLayer : public Layer {
  public:
	FizzLayer()
		: Layer("Fizziks Simulation"),
		  m_CameraController((float) Application::get().getWindow().GetWidth() /
	                         (float) Application::get().getWindow().GetHeight()) {

		Ref<Shape> circle = CreateRef<Circle>(1);
		Ref<Shape> square = CreateRef<Polygon>(PolygonType::SQUARE);

		Ref<PhysicsObject> pivot = CreateRef<PhysicsObject>(
			square, Transform({glm::vec2(0, 0), 0, glm::vec2(0.1f, 0.1f)}));
		Ref<PhysicsObject> pend = CreateRef<PhysicsObject>(
			circle, Transform({glm::vec2(0, -0.5f), 0, glm::vec2(0.1f, 0.1f)}));
		m_PhysicsEnv.Add(pivot);
		m_PhysicsEnv.Add(pend);
	}

	virtual void OnUpdate(Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		// for (Ref<PhysicsObject> object : m_PhysicsEnv.GetObjects())
		// 	object->ApplyForce(glm::vec2(0.0f, -0.5f)); // gravity
		m_PhysicsEnv.Update(ts);

		Renderer::BeginScene(m_CameraController.GetCamera());
		m_PhysicsEnv.Render();
		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Fizziks Debug");

		Collision collision =
			GJKGetCollision(m_PhysicsEnv.GetObjects()[0], m_PhysicsEnv.GetObjects()[1]);

		ImGui::Text("Collision between Objects 1 and 2:");
		if (collision.exists) {
			ImGui::Text("Penetration Depth: %.3f", collision.penetrationDepth);
			ImGui::Text("Collision Normal: <%.3f. %.3f>", collision.MTV.x, collision.MTV.y);
		} else {
			ImGui::Text("Separation Distance: %.3f", collision.separationDist);
			ImGui::Text("Closest Direction: <%.3f. %.3f>", collision.closestDir.x,
			            collision.closestDir.y);
			ImGui::Text("Witness Point 1: <%.3f. %.3f>", collision.witness1.x,
			            collision.witness1.y);
			ImGui::Text("Witness Point 2: <%.3f. %.3f>", collision.witness2.x,
			            collision.witness2.y);
		}

		ImGui::Separator();

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
			ImGui::Text("Object %u:", i + 1);
			ImGui::SliderFloat2("Position", glm::value_ptr(localPos), -2.0f, 2.0f);
			ImGui::SliderFloat("Rotation", &localRot, 0.0f, 2 * 3.1415f);
			ImGui::SliderFloat2("Scale", glm::value_ptr(localScale), 0.0f, 2.0f);
			ImGui::PopID();

			object->SetTransform(localPos, localRot, localScale);
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
				ImGui::Text("Penetration Depth: %.3f", collision.penetrationDepth);
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
