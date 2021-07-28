#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Nutella/Renderer/VertexArray.hpp>
#include <Nutella/Renderer/Shader.hpp>

#include "PhysicsObject.hpp"

using namespace Nutella;

namespace Fizz {
	class Polygon : public PhysicsObject {
	  public:
		Polygon(const std::vector<glm::vec2>& points, const glm::vec2& pos = {0.0, 0.0});
		~Polygon();

		virtual void Render() override;

		inline glm::vec2 GetPos() { return m_Position; }

	  private:
		std::vector<glm::vec2> m_Points;
		uint32_t m_NumPoints;

		glm::vec2 m_Position;
		float m_Rotation;
		glm::vec2 m_Scale;

		Ref<VertexArray> m_VAO;
		Ref<Shader> m_Shader;
	};
} // namespace Fizz
