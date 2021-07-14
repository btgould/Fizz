#include <Nutella.hpp>
#include <Nutella/Core/EntryPoint.hpp>

#include <imgui.h>

#include "glm/gtc/matrix_transform.hpp"
#include "Geometry/Mesh.hpp"

class FizzLayer : public Nutella::Layer {
  public:
	FizzLayer()
		: Layer("Fizziks Simulation"),
		  m_CameraController((float) Nutella::Application::get().getWindow().GetWidth() /
								 (float) Nutella::Application::get().getWindow().GetHeight(),
							 true),
		  m_Polygon({{-0.25, -0.25}, {0.25, -0.25}, {0.25, 0.25}, {0, 0.5}, {-0.25, 0.25}},
					{0, 0}) {
		m_Vao = m_Polygon.GenVertexArray();
		m_Shader = Nutella::Shader::Create("fizz/res/shaders/Mesh.shader");
	};

	virtual void OnUpdate(Nutella::Timestep ts) override {
		m_CameraController.OnUpdate(ts);

		glm::mat4 modelTRS =
			glm::translate(glm::mat4(1.0f), {m_Polygon.GetPos().x, m_Polygon.GetPos().y, 0.0});

		Nutella::Renderer::BeginScene(m_CameraController.GetCamera());
		Nutella::Renderer::Submit(m_Vao, m_Shader, modelTRS);
		Nutella::Renderer::EndScene();
	}

	virtual void OnEvent(Nutella::Event& event) override { m_CameraController.OnEvent(event); }

  private:
	Nutella::OrthoCamController m_CameraController;
	Nutella::Ref<Nutella::Shader> m_Shader;

	Fizz::Mesh m_Polygon;
	Nutella::Ref<Nutella::VertexArray> m_Vao;
};

class Sandbox : public Nutella::Application {
  public:
	Sandbox() { PushLayer(new FizzLayer()); }

	~Sandbox() {}
};

Nutella::Application* Nutella::CreateApplication() {
	return new Sandbox();
}
