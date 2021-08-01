#include "PhysicsObject.hpp"

#include "Simplex.hpp"

namespace Fizz {
	// checks is simplex contains origin, returning true if it does. Otherwise, it removes any
	// redundant points on the simplex
	bool UpdateSimplex(Simplex& s);
	// calculates the normal of the simplex pointed towards the origin
	glm::vec2 NextDir(const Simplex& s);

	bool PhysicsObject::GJKColliding(Nutella::Ref<PhysicsObject>& other) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = glm::vec2(1.0f, 0.0f);

		// add first point to simplex
		glm::vec2 supportPoint(this->MinkowskiDiffSupport(other, nextDir));
		Simplex s({supportPoint});
		nextDir = -supportPoint;

		// add second point to simplex
		supportPoint = this->MinkowskiDiffSupport(other, nextDir);
		if (glm::dot(nextDir, supportPoint) < 0) {
			// simplex cannot possibly contain origin
			return false;
		}
		s.Add(supportPoint);
		nextDir = NextDir(s);

		while (true) {
			// calculate + add next point
			supportPoint = this->MinkowskiDiffSupport(other, nextDir);
			if (glm::dot(nextDir, supportPoint) < 0) {
				// simplex cannot possibly contain origin
				return false;
			}
			s.Add(supportPoint);

			// check if simplex contains origin + remove redundant points
			if (UpdateSimplex(s)) {
				// Simplex contains origin -> collision
				return true;
			}

			// update seach direction
			nextDir = NextDir(s);
		}
	}

	bool UpdateSimplex(Simplex& s) {
		// calculate where origin is
		glm::vec3 side1 = glm::vec3(s[1].x - s[2].x, s[1].y - s[2].y, 0.0f);
		glm::vec3 side2 = glm::vec3(s[0].x - s[2].x, s[0].y - s[2].y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[2].x, -s[2].y, 0.0f);
		glm::vec3 norm1 = glm::cross(glm::cross(side2, side1), side1);
		glm::vec3 norm2 = glm::cross(glm::cross(side1, side2), side2);

		if (glm::dot(norm1, toOrigin) > 0) {
			// origin is in region 1
			s.Remove(s[0]);
		} else if (glm::dot(norm2, toOrigin) > 0) {
			// origin is in region 2
			s.Remove(s[1]);
		} else {
			// origin is in simplex -> collision
			return true;
		}

		return false;
	}

	glm::vec2 NextDir(const Simplex& s) {
		glm::vec3 segment = glm::vec3(s[0].x - s[1].x, s[0].y - s[1].y, 0.0f);
		glm::vec3 toOrigin = glm::vec3(-s[1].x, -s[1].y, 0.0f);
		return glm::cross(glm::cross(segment, toOrigin), segment);
	}
} // namespace Fizz
