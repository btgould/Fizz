#pragma once

#include <glm/glm.hpp>

namespace Fizz {
	/* Represents a rectangle aligned on the x and y axes. It is stored as two vectors; the minimum
	 * (lower left) and maximum (upper right) corners.
	 */
	struct AABB {
		AABB(const glm::vec2& minVec, const glm::vec2& maxVec);
		~AABB();

		/* Gets the width of this bounding box */
		inline float GetWidth() { return max.x - min.x; }

		/* Gets the height of this bounding box */
		inline float GetHeight() { return max.y - min.y; }

		/* Tests whether this bounding box completely contains the given point. If the point is on
		   the borders of the box, it is not counted as contained within the box.

		   @param point: The point to check for containment

		   @return true if the point is contained in this bounding box, false otherwise
		 */
		bool Contains(const glm::vec2& point);

		/* Tests whether this bounding box completely contains the other box.

		   @param other: The other bounding box to check for containment

		   @return true if the other bounding box is completely contained within this box, false
		   otherwise
		 */
		bool Contains(const AABB& other);

		/* Tests whether any part of this bounding box overlaps with the other bounding box.

		   @param other: The other bounding box to check for intersection

		   @return true if this bounding box intersects the other bounding box, false otherwise
		 */
		bool Intersects(const AABB& other);

		glm::vec2 min, max;
	};
} // namespace Fizz
