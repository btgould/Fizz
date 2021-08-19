#include "Polygon.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <Nutella/Renderer/Renderer.hpp>

namespace Fizz {
	using namespace Nutella;

	Polygon::Polygon(const std::vector<glm::vec2>& points, const glm::vec2& pos /*= {0.0, 0.0}*/)
		: m_Points(points), m_NumPoints(points.size()), m_Position(pos), m_Rotation(0.0f),
		  m_Scale(1.0f, 1.0f), m_Velocity(0.0f, 0.0f), m_InvMass(1.0f), m_Restitution(0.8f),
		  m_Shader(Shader::Create("fizz/res/shaders/Mesh.shader")) {
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

		m_TransformedPoints.reserve(m_NumPoints);
		UpdatePoints();
	}

	Polygon::Polygon(PolygonType type, float size, const glm::vec2& pos)
		: m_Position(pos), m_Rotation(0.0f), m_Scale(size, size), m_Velocity(0.0f, 0.0f),
		  m_InvMass(1.0f), m_Restitution(0.8f),
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

		VertexBufferLayout layout;
		layout.push(VertexAttribType::FLOAT, 2, false); // position

		Ref<VertexBuffer> vbo;
		vbo = VertexBuffer::Create(&m_Points[0], 2 * m_NumPoints * sizeof(float));

		Ref<IndexBuffer> ibo;
		ibo = IndexBuffer::Create(&vertexOrder[0], 3 * (m_NumPoints - 2) * sizeof(uint32_t));

		m_VAO = VertexArray::Create(layout, vbo, ibo);

		m_TransformedPoints.reserve(m_NumPoints);
		UpdatePoints();
	}

	Polygon::~Polygon() {}

	void Polygon::Render() { Renderer::Submit(m_VAO, m_Shader, m_TRSMat); }

	void Polygon::Update() {
		NT_PROFILE_FUNC();

		m_Position += m_Velocity;
		UpdatePoints();
	}

	glm::vec2 Polygon::Support(const glm::vec2& dir) const {
		NT_PROFILE_FUNC();

		glm::vec2 supportPoint = m_TransformedPoints[0];
		float maxSupportDist = dir.x * supportPoint.x + dir.y * supportPoint.y;

		for (int i = 1; i < m_NumPoints; i++) {
			float currSupportDist =
				dir.x * m_TransformedPoints[i].x + dir.y * m_TransformedPoints[i].y;
			if (currSupportDist > maxSupportDist) {
				maxSupportDist = currSupportDist;
				supportPoint = m_TransformedPoints[i];
			}
		}

		return supportPoint;
	}

	void Polygon::UpdateTRSMat() {
		NT_PROFILE_FUNC();

		// reverse order b/c column major ordering?
		m_TRSMat = glm::translate(glm::mat4(1.0f), {m_Position.x, m_Position.y, 0.0f});
		m_TRSMat = glm::rotate(m_TRSMat, m_Rotation, {0.0f, 0.0f, 1.0f});
		m_TRSMat = glm::scale(m_TRSMat, {m_Scale.x, m_Scale.y, 1.0f});
	}

	void Polygon::UpdatePoints() {
		// TODO: is there a way to be more efficient about this so that I don't have to do this
		// calculation on both the CPU and GPU?
		NT_PROFILE_FUNC();

		UpdateTRSMat();

		for (uint32_t i = 0; i < m_NumPoints; i++) {
			m_TransformedPoints[i] = m_TRSMat * glm::vec4(m_Points[i].x, m_Points[i].y, 0.0f, 1.0f);
		}
	}
} // namespace Fizz
