#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Nutella/Renderer/VertexArray.hpp>
#include <Nutella/Renderer/Shader.hpp>

#include "PhysicsObject.hpp"

namespace Fizz {
	enum class PolygonType { TRIANGLE, SQUARE, HEXAGON };

	class Polygon : public PhysicsObject {
	  public:
		// Creates a polygon from a list of 2D points. Points should be given in counter-clockwise
		// winding order. Initializes position to the given value.
		Polygon(const std::vector<glm::vec2>& points, const glm::vec2& pos = {0.0, 0.0});
		// Creates a regular polygon of the given type, scaled by the given amount. Initializes
		// position to the given value.
		Polygon(PolygonType type, float size, const glm::vec2& pos = {0.0, 0.0});
		~Polygon();

		virtual void Render() override;

		virtual glm::vec2 Support(const glm::vec2& dir) const override;

		inline virtual glm::vec2 GetPos() const override { return m_Position; }
		inline virtual void SetPos(const glm::vec2& pos) override {
			m_Position = pos;
			UpdatePoints();
		}

		inline virtual float GetRot() const override { return m_Rotation; }
		inline virtual void SetRot(const float rot) override {
			m_Rotation = rot;
			UpdatePoints();
		}

		inline virtual glm::vec2 GetScale() const override { return m_Scale; }
		inline virtual void SetScale(const glm::vec2& scale) override {
			m_Scale = scale;
			UpdatePoints();
		}

	  private:
		void UpdateTRSMat();
		void UpdatePoints();

	  private:
		std::vector<glm::vec2> m_Points;
		std::vector<glm::vec2> m_TransformedPoints;
		uint32_t m_NumPoints;

		glm::vec2 m_Position;
		float m_Rotation;
		glm::vec2 m_Scale;

		glm::mat4 m_TRSMat;

		Nutella::Ref<Nutella::VertexArray> m_VAO;
		Nutella::Ref<Nutella::Shader> m_Shader;
	};
} // namespace Fizz
