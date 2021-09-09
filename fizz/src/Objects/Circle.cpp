#include "Circle.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Nutella/Core/Application.hpp>
#include <Nutella/Renderer/Renderer.hpp>

namespace Fizz {
	using namespace Nutella;

	Circle::Circle(float radius)
		: m_Radius(radius), m_Shader(Shader::Create("fizz/res/shaders/Circle.shader")) {
		VertexBufferLayout layout;
		layout.push(VertexAttribType::FLOAT, 2, false); // position

		float points[] = {-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
		Ref<VertexBuffer> vbo;
		vbo = VertexBuffer::Create(points, sizeof(points));

		uint32_t vertexOrder[] = {0, 1, 2, 2, 3, 0};
		Ref<IndexBuffer> ibo;
		ibo = IndexBuffer::Create(vertexOrder, sizeof(vertexOrder));

		m_VAO = VertexArray::Create(layout, vbo, ibo);
	}

	Circle::~Circle() {}

	void Circle::Render() {
		m_Shader->Bind();
		m_Shader->SetUniformVec2f("u_Position", m_Position);
		m_Shader->SetUniform1f("u_Radius", m_Radius);

		Renderer::Submit(m_VAO, m_Shader, m_TRSMat);
	}

	glm::vec2 Circle::Support(const glm::vec2& dir) const {
		return m_Position + m_Radius * glm::normalize(dir);
	}

	AABB Circle::GetAABB() const {
		glm::vec2 min = glm::vec2(m_Position.x - m_Radius, m_Position.y - m_Radius);
		glm::vec2 max = glm::vec2(m_Position.x + m_Radius, m_Position.y + m_Radius);
		return {min, max};
	}

	void Circle::SetTransform(const Transform& transform) {
		m_Position = transform.position;
		m_TRSMat = glm::translate(glm::mat4(1.0f), {m_Position.x, m_Position.y, 0.0f});
		m_Radius = transform.scale.x;
		m_TRSMat = glm::scale(m_TRSMat, {m_Radius, m_Radius, 1.0f});
	}
} // namespace Fizz
