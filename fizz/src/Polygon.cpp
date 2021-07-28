#include "Polygon.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <Nutella/Renderer/Renderer.hpp>

namespace Fizz {
	using namespace Nutella;

	Polygon::Polygon(const std::vector<glm::vec2>& points, const glm::vec2& pos /*= {0.0, 0.0}*/)
		: m_Points(points), m_NumPoints(points.size()), m_Position(pos), m_Rotation(0.0f),
		  m_Scale(1.0f, 1.0f) {
		// use point data to create a Vertex Array
		VertexBufferLayout layout;
		layout.push(VertexAttribType::FLOAT, 2, false); // position

		float vertices[2 * (m_Points.size())];
		for (int i = 0; i < m_Points.size(); i++) {
			vertices[2 * i] = m_Points[i].x;
			vertices[2 * i + 1] = m_Points[i].y;
		}

		Ref<VertexBuffer> vbo;
		vbo = VertexBuffer::Create(vertices, sizeof(vertices));

		unsigned int vertexOrder[3 * (m_NumPoints - 2)];
		for (unsigned int i = 0; i < m_NumPoints - 2; i++) {
			vertexOrder[3 * i] = 0;
			vertexOrder[3 * i + 1] = i + 1;
			vertexOrder[3 * i + 2] = i + 2;
		}

		Ref<IndexBuffer> ibo;
		ibo = IndexBuffer::Create(vertexOrder, sizeof(vertexOrder));

		m_VAO = VertexArray::Create(layout, vbo, ibo);

		// load default shader
		m_Shader = Shader::Create("fizz/res/shaders/Mesh.shader");
	}

	Polygon::~Polygon() {}

	void Polygon::Render() {
		glm::mat4 trs = glm::scale(glm::mat4(1.0f), {m_Scale.x, m_Scale.y, 1.0f});
		trs = glm::translate(trs, {m_Position.x, m_Position.y, 0.0f});
		trs = glm::rotate(trs, m_Rotation, {0.0f, 0.0f, 1.0f});

		Renderer::Submit(m_VAO, m_Shader, trs);
	}
} // namespace Fizz
