#pragma once

#include <glm/glm.hpp>
#include <vector>

#include <Nutella/Renderer/VertexArray.hpp>
#include <Nutella/Renderer/Shader.hpp>

#include "Shape.hpp"

namespace Fizz {
	enum class PolygonType { TRIANGLE = 0, SQUARE, HEXAGON, COUNT };

	/* A shape defined as the region eclosed by a series of points (vertices) and straight lines
	   between them (edges)
	 */
	class Polygon : public Shape {
	  public:
		/* Creates a polygon from a list of 2D points. Points should be given in counter-clockwise
		   winding order.

		   @param points: A list of the polygon's vertices
		*/
		Polygon(const std::vector<glm::vec2>& points);

		/* Creates a regular polygon of the given type

		   @param type: The type of polygon to create
		 */
		Polygon(PolygonType type);
		~Polygon();

		virtual void Render() override;

		virtual glm::vec2 Support(const glm::vec2& dir) const override;
		virtual AABB GetAABB() const override;

		virtual void SetTransform(const Transform& transform) override;

	  private:
		std::vector<glm::vec2> m_Points;
		uint32_t m_NumPoints;

		Transform m_Transform;
		std::vector<glm::vec2> m_TransformedPoints;
		glm::mat4 m_TRSMat;

		Nutella::Ref<Nutella::VertexArray> m_VAO;
		// TODO: this should be shared between instances
		Nutella::Ref<Nutella::Shader> m_Shader;
	};
} // namespace Fizz
