#pragma once

#include <Nutella/Renderer/VertexArray.hpp>
#include <Nutella/Renderer/Shader.hpp>

#include "Shape.hpp"

namespace Fizz {
	class Circle : public Shape {
	  public:
		Circle(float radius);
		~Circle();

		virtual void Render() override;

		virtual glm::vec2 Support(const glm::vec2& dir) const override;
		virtual AABB GetAABB() const override;

		virtual void SetTransform(const Transform& transform) override;

	  private:
		glm::vec2 m_Position;
		float m_Radius;

		glm::mat4 m_TRSMat;
		Nutella::Ref<Nutella::VertexArray> m_VAO;
		// TODO: this should be shared between instances
		Nutella::Ref<Nutella::Shader> m_Shader;
	};
} // namespace Fizz
