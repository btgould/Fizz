#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include "Polygon.hpp"
#include "Simplex.hpp"

using namespace Nutella;

class FizzLayer : public Layer {
  public:
	FizzLayer()
		: Layer("Fizziks Simulation"),
		  m_CameraController((float) Application::get().getWindow().GetWidth() /
								 (float) Application::get().getWindow().GetHeight(),
							 true) {

		// m_Polygon = CreateRef<Fizz::Polygon>(Fizz::PolygonType::SQUARE, 0.2);
		// m_Polygon2 =
		// 	CreateRef<Fizz::Polygon>(Fizz::PolygonType::SQUARE, 0.2, glm::vec2(1.0f, 0.0f));

		m_Polygon = CreateRef<Fizz::Polygon, std::vector<glm::vec2>, glm::vec2>(
			{{-0.25, -0.25}, {0.25, -0.25}, {0.25, 0.25}, {0, 0.5}, {-0.25, 0.25}}, {0.2, -0.28});

		m_Polygon2 =
			CreateRef<Fizz::Polygon>(Fizz::PolygonType::TRIANGLE, 0.3, glm::vec2(1.0f, 0.0f));
	}

	virtual void OnUpdate(Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		Renderer::BeginScene(m_CameraController.GetCamera());
		m_Polygon->Render();
		m_Polygon2->Render();
		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Fizziks Debug");

		glm::vec2 p1LocalPos = m_Polygon->GetPos();
		float p1LocalRot = m_Polygon->GetRot();
		glm::vec2 p1LocalScale = m_Polygon->GetScale();

		ImGui::Text("Polygon 1:");
		ImGui::SliderFloat2("Position##1", glm::value_ptr(p1LocalPos), -2.0f, 2.0f);
		ImGui::SliderFloat("Rotation##1", &p1LocalRot, 0.0f, 2 * 3.1415f);
		ImGui::SliderFloat2("Scale##1", glm::value_ptr(p1LocalScale), 0.0f, 2.0f);

		m_Polygon->SetPos(p1LocalPos);
		m_Polygon->SetRot(p1LocalRot);
		m_Polygon->SetScale(p1LocalScale);

		ImGui::Separator();

		glm::vec2 p2LocalPos = m_Polygon2->GetPos();
		float p2LocalRot = m_Polygon2->GetRot();
		glm::vec2 p2LocalScale = m_Polygon2->GetScale();

		ImGui::Text("Polygon 2:");
		ImGui::SliderFloat2("Position##2", glm::value_ptr(p2LocalPos), -2.0f, 2.0f);
		ImGui::SliderFloat("Rotation##2", &p2LocalRot, 0.0f, 2 * 3.1415f);
		ImGui::SliderFloat2("Scale##2", glm::value_ptr(p2LocalScale), 0.0f, 2.0f);

		m_Polygon2->SetPos(p2LocalPos);
		m_Polygon2->SetRot(p2LocalRot);
		m_Polygon2->SetScale(p2LocalScale);

		ImGui::Separator();

		glm::vec2 separationDist = GJKDistance(m_Polygon, m_Polygon2);

		ImGui::Text("Collision");
		ImGui::Text("Colliding: %s", GJKColliding(m_Polygon, m_Polygon2) ? "true" : "false");
		ImGui::Text("Distance: <%0.3f, %0.3f>", separationDist.x, separationDist.y);

		ImGui::End();
	}

	virtual void OnEvent(Event& event) override { m_CameraController.OnEvent(event); }

  private:
	OrthoCamController m_CameraController;

	Ref<Fizz::PhysicsObject> m_Polygon;
	Ref<Fizz::PhysicsObject> m_Polygon2;
};

class Sandbox : public Application {
  public:
	Sandbox() { PushLayer(new FizzLayer()); }

	~Sandbox() {}
};

Application* Nutella::CreateApplication() {
	return new Sandbox();
}
