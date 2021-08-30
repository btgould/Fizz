#include "AABB.hpp"

namespace Fizz {
	AABB::AABB(const glm::vec2& minVec, const glm::vec2& maxVec) : min(minVec), max(maxVec) {}

	AABB::~AABB() {}

	bool AABB::Contains(const glm::vec2& point) {
		bool inX = min.x < point.x && point.x < max.x;
		bool inY = min.y < point.y && point.y < max.y;
		return inX && inY;
	}

	bool AABB::Contains(const AABB& other) {
		bool inX = min.x < other.min.x && other.max.x < max.x;
		bool inY = min.y < other.min.y && other.max.y < max.y;
		return inX && inY;
	}

	bool AABB::Intersects(const AABB& other) {
		return !(min.x > other.max.x || max.x < other.min.x || min.y > other.max.y ||
				 max.y < other.min.y);
	}
} // namespace Fizz
