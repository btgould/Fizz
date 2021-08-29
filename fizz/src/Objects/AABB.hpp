#pragma once

#include <glm/glm.hpp>

namespace Fizz {
	struct AABB {
		AABB(const glm::vec2& minVec, const glm::vec2& maxVec);
		~AABB();

		inline float GetWidth() { return max.x - min.x; }
		inline float GetHeight() { return max.y - min.y; }

		bool Contains(const glm::vec2& point);
		bool Contains(const AABB& other);
		bool Intersects(const AABB& other);

		glm::vec2 min, max;
	};
} // namespace Fizz
