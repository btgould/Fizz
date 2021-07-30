#include "Polygon.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <Nutella/Renderer/Renderer.hpp>

namespace Fizz {
	using namespace Nutella;

	Polygon::Polygon(const std::vector<glm::vec2>& points, const glm::vec2& pos /*= {0.0, 0.0}*/)
		: m_Points(points), m_NumPoints(points.size()), m_Position(pos), m_Rotation(0.0f),
		  m_Scale(1.0f, 1.0f), m_Shader(Shader::Create("fizz/res/shaders/Mesh.shader")) {
		// use point data to create a Vertex Array
		VertexBufferLayout layout;
		layout.push(VertexAttribType::FLOAT, 2, false); // position

		Ref<VertexBuffer> vbo;
		vbo = VertexBuffer::Create(&m_Points[0], 2 * m_NumPoints * sizeof(float));

		unsigned int vertexOrder[3 * (m_NumPoints - 2)];
		for (unsigned int i = 0; i < m_NumPoints - 2; i++) {
			vertexOrder[3 * i] = 0;
			vertexOrder[3 * i + 1] = i + 1;
			vertexOrder[3 * i + 2] = i + 2;
		}

		Ref<IndexBuffer> ibo;
		ibo = IndexBuffer::Create(vertexOrder, sizeof(vertexOrder));

		m_VAO = VertexArray::Create(layout, vbo, ibo);
	}

	Polygon::Polygon(PolygonType type, float size, const glm::vec2& pos)
		: m_Position(pos), m_Rotation(0.0f), m_Scale(size, size),
		  m_Shader(Shader::Create("fizz/res/shaders/Mesh.shader")) {
		std::vector<uint32_t> vertexOrder;

		float halfSqrt3 = glm::sqrt(3) / 2;

		switch (type) {
		case PolygonType::TRIANGLE:
			m_NumPoints = 3;
			m_Points = {{-1.0f, -halfSqrt3}, {1.0f, -halfSqrt3}, {0.0f, halfSqrt3}};
			vertexOrder = {0, 1, 2};
			break;

		case PolygonType::SQUARE:
			m_NumPoints = 4;
			m_Points = {{-1.0f, -1.0f}, {1.0f, -1.0f}, {1.0f, 1.0f}, {-1.0f, 1.0f}};
			vertexOrder = {0, 1, 2, 0, 2, 3};
			break;

		case PolygonType::HEXAGON:
			m_NumPoints = 6;
			m_Points = {{1.0f, 0.0f},  {0.5f, halfSqrt3},	{-0.5f, halfSqrt3},
						{-1.0f, 0.0f}, {-0.5f, -halfSqrt3}, {0.5f, -halfSqrt3}};
			vertexOrder = {0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5};
			break;

		default:
			NT_ASSERT(false, "Unrecognized Polygon type!");
			break;
		}

		for (glm::vec2 coord : m_Points)
			coord *= size;

		VertexBufferLayout layout;
		layout.push(VertexAttribType::FLOAT, 2, false); // position

		Ref<VertexBuffer> vbo;
		vbo = VertexBuffer::Create(&m_Points[0], 2 * m_NumPoints * sizeof(float));

		Ref<IndexBuffer> ibo;
		ibo = IndexBuffer::Create(&vertexOrder[0], 3 * (m_NumPoints - 2) * sizeof(uint32_t));

		m_VAO = VertexArray::Create(layout, vbo, ibo);
	}

	Polygon::~Polygon() {}

	void Polygon::Render() {
		glm::mat4 trs = glm::scale(glm::mat4(1.0f), {m_Scale.x, m_Scale.y, 1.0f});
		trs = glm::translate(trs, {m_Position.x, m_Position.y, 0.0f});
		trs = glm::rotate(trs, m_Rotation, {0.0f, 0.0f, 1.0f});

		Renderer::Submit(m_VAO, m_Shader, trs);
	}
} // namespace Fizz
