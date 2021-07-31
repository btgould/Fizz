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

		// m_Polygon = CreateRef<Fizz::Polygon, std::vector<glm::vec2>, glm::vec2>(
		// 	{{-0.25, -0.25}, {0.25, -0.25}, {0.25, 0.25}, {0, 0.5}, {-0.25, 0.25}}, {0, 0});

		m_Polygon = CreateRef<Fizz::Polygon>(Fizz::PolygonType::SQUARE, 0.2);
		m_Polygon2 =
			CreateRef<Fizz::Polygon>(Fizz::PolygonType::TRIANGLE, 0.3, glm::vec2(2.0f, 0.0f));
	}

	virtual void OnAttach() override {
		Fizz::Simplex simplex({{0.0f, 0.0f}, {0.0f, 1.0f}});

		simplex.Add({1.0f, 0.0f});
		simplex.Remove({0.0f, 0.0f});

		for (glm::vec2 point : simplex) {
			NT_TRACE("<{0}, {1}>", point.x, point.y);
		}
	}

	virtual void OnUpdate(Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		Renderer::BeginScene(m_CameraController.GetCamera());
		m_Polygon->Render();
		m_Polygon2->Render();
		Renderer::EndScene();
	}

	virtual void OnImGuiRender() override {
		// ImGui::Begin("Fizziks Debug");

		ImGui::SliderFloat2("Support Direction", glm::value_ptr(m_SupportDir), -1.0f, 1.0f);

		glm::vec2 support1 = m_Polygon->Support(m_SupportDir);
		glm::vec2 support2 = m_Polygon2->Support(m_SupportDir);
		glm::vec2 mkSupport = m_Polygon->MinkowskiDiffSupport(m_Polygon2, m_SupportDir);

		ImGui::Text("P1 Support: <%.3f, %.3f>", support1.x, support1.y);
		ImGui::Text("P2 Support: <%.3f, %.3f>", support2.x, support2.y);
		ImGui::Text("Minkowski Support: <%.3f, %.3f>", mkSupport.x, mkSupport.y);
	}

	virtual void OnEvent(Event& event) override { m_CameraController.OnEvent(event); }

  private:
	OrthoCamController m_CameraController;

	glm::vec2 m_SupportDir;

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
