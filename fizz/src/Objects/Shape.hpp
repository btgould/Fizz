#pragma once

#include <glm/glm.hpp>
#include "Objects/AABB.hpp"

namespace Fizz {
	/** Structure representing the position, rotation, and scale of an object in 2 dimesional
	 * space
	 */
	struct Transform {
		glm::vec2 position;
		float rotation;
		glm::vec2 scale;

		bool operator==(const Transform& rhs) const {
			return this->position == rhs.position && this->rotation == rhs.rotation &&
				   this->scale == rhs.scale;
		}

		bool operator!=(const Transform& rhs) const { return !(*this == rhs); }
	};

	/** Structure describing how responsive an object should be to forces */
	struct MassInfo {
		float density;

		float mass;
		float invMass;

		float rotInertia;
		float invRotIntertia;
	};

	/** Represents a contigious collection of points in 2D space */
	class Shape {
	  public:
		/* Draws the shape on the screen */
		virtual void Render() = 0;

		/** Gets the farthest point in the given direction on the shape (i.e. the point with the
		 *  largest dot product in that direction).
		 *
		 *  @param dir: The direction to get the support in
		 *
		 *  @return The support point in the given direction
		 */
		virtual glm::vec2 Support(const glm::vec2& dir) const = 0;

		/** Gets the aligned axis bounding box of the shape (i.e. the smallest orthagonal rectangle
		 *  that completely contains the shape)
		 *
		 *	@return The AABB containing this shape
		 */
		virtual AABB GetAABB() const = 0;

		/** Sets the transform of this shape in 2D space. While some properties of the shape are
		 *  completely intrinsic, having data like a transform is useful for things like testing
		 *  collision with other shapes.
		 *
		 *  @param transform: The new posision, rotation, and scale of this shape
		 */
		virtual void SetTransform(const Transform& transform) = 0;

		/** Gets information about how responsive the object represented by the shape should be to
		 *  forces. This should be determined by the density of the object, the size of the shape,
		 *  and inherent properties about the shape (for rotational inertial purposes).
		 *
		 *  @param density: The density of the object to use for mass calculations
		 *
		 */
		virtual MassInfo GetMassInfo(const float density) = 0;
	};
} // namespace Fizz
