#include "Mesh.hpp"

namespace Fizz {
	Mesh::Mesh(const std::vector<glm::vec2>& points, const glm::vec2& pos /*= {0.0, 0.0}*/)
		: m_Points(points), m_NumPoints(points.size()), m_Position(pos), m_Rotation(0) {}

	Mesh::~Mesh() {}

	Nutella::Ref<Nutella::VertexArray> Mesh::GenVertexArray() {
		Nutella::VertexBufferLayout layout;
		layout.push(Nutella::VertexAttribType::FLOAT, 2, false); // position

		float vertices[2 * (m_Points.size())];
		for (int i = 0; i < m_Points.size(); i++) {
			vertices[2 * i] = m_Points[i].x;
			vertices[2 * i + 1] = m_Points[i].y;
		}

		Nutella::Ref<Nutella::VertexBuffer> vbo;
		vbo = Nutella::VertexBuffer::Create(vertices, sizeof(vertices));

		unsigned int vertexOrder[3 * (m_NumPoints - 2)];
		for (unsigned int i = 0; i < m_NumPoints - 2; i++) {
			vertexOrder[3 * i] = 0;
			vertexOrder[3 * i + 1] = i + 1;
			vertexOrder[3 * i + 2] = i + 2;
		}

		Nutella::Ref<Nutella::IndexBuffer> ibo;
		ibo = Nutella::IndexBuffer::Create(vertexOrder, sizeof(vertexOrder));

		return Nutella::VertexArray::Create(layout, vbo, ibo);
	}
} // namespace Fizz
