#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include "glm/gtc/matrix_transform.hpp"
#include "Polygon.hpp"

class FizzLayer : public Nutella::Layer {
  public:
	FizzLayer()
		: Layer("Fizziks Simulation"),
		  m_CameraController((float) Nutella::Application::get().getWindow().GetWidth() /
								 (float) Nutella::Application::get().getWindow().GetHeight(),
							 true),
		  m_Polygon({{-0.25, -0.25}, {0.25, -0.25}, {0.25, 0.25}, {0, 0.5}, {-0.25, 0.25}},
					{0, 0}) {};

	virtual void OnUpdate(Nutella::Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		Nutella::Renderer::BeginScene(m_CameraController.GetCamera());
		m_Polygon.Render();
		Nutella::Renderer::EndScene();
	}

	virtual void OnEvent(Nutella::Event& event) override { m_CameraController.OnEvent(event); }

  private:
	Nutella::OrthoCamController m_CameraController;

	Fizz::Polygon m_Polygon;
};

class Sandbox : public Nutella::Application {
  public:
	Sandbox() { PushLayer(new FizzLayer()); }

	~Sandbox() {}
};

Nutella::Application* Nutella::CreateApplication() {
	return new Sandbox();
}
