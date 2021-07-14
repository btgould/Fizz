#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Nutella/Renderer/VertexArray.hpp>

namespace Fizz {
	class Mesh {
	  public:
		Mesh(const std::vector<glm::vec2>& points, const glm::vec2& pos = {0.0, 0.0});
		~Mesh();

		Nutella::Ref<Nutella::VertexArray> GenVertexArray();

		inline glm::vec2 GetPos() { return m_Position; }

	  private:
		std::vector<glm::vec2> m_Points;
		unsigned int m_NumPoints;

		glm::vec2 m_Position;
		float m_Rotation;
	};
} // namespace Fizz
