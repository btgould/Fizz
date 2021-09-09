#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

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

		// Ref<PhysicsObject> moved = CreateRef<PhysicsObject>(
		// 	CreateRef<Circle>(0.4),
		// 	Transform({glm::vec2(0.4f, 0.6f), 0.0f, glm::vec2(0.2f, 0.2f)}));

		// Ref<PhysicsObject> floor = CreateRef<PhysicsObject>(
		// 	CreateRef<Polygon>(PolygonType::SQUARE),
		// 	Transform({glm::vec2(0.0f, -0.6f), 0.0f, glm::vec2(1.0f, 0.2f)}));
		// floor->SetInvMass(0.0f);

		// m_PhysicsEnv.Add(moved);
		// m_PhysicsEnv.Add(floor);

		srand(time(NULL));

		for (uint32_t i = 0; i < 50; i++) {
			Ref<PhysicsObject> object;
			Ref<Shape> shape;

			if (std::rand() % (int) PolygonType::COUNT == 0) {
				shape = CreateRef<Circle>(0.0f); // radius set by random transform
			} else {
				PolygonType polygonType =
					static_cast<PolygonType>(std::rand() % (int) PolygonType::COUNT);
				shape = CreateRef<Polygon>(polygonType);
			}

			float x = 10.0 * std::rand() / RAND_MAX - 5;
			float y = 10.0 * std::rand() / RAND_MAX - 5;
			float rot = 2 * 3.1415f * std::rand() / RAND_MAX;
			float scaleX = (float) std::rand() / RAND_MAX / 2;
			float scaleY = (float) std::rand() / RAND_MAX / 2;

			object = CreateRef<PhysicsObject>(
				shape, Transform({glm::vec2(x, y), rot, glm::vec2(scaleX, scaleY)}));

			m_PhysicsEnv.Add(object);
		}
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

		glm::vec2 distance =
			GJKDistance(m_PhysicsEnv.GetObjects()[0], m_PhysicsEnv.GetObjects()[1]);
		ImGui::Text("Distance between [0] and [1] <%-3.2f, %-3.2f>", distance.x, distance.y);
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
