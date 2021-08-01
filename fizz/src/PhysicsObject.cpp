#include "PhysicsObject.hpp"

#include "Simplex.hpp"

namespace Fizz {
	bool PhysicsObject::GJKColliding(Nutella::Ref<PhysicsObject>& other) {
		NT_PROFILE_FUNC();

		glm::vec2 nextDir = glm::vec2(1.0f, 0.0f);

		// add first point to simplex
		glm::vec2 supportPoint(this->MinkowskiDiffSupport(other, nextDir));
		Simplex s({supportPoint});
		nextDir = -supportPoint;

		// add second point to simpex
		supportPoint = this->MinkowskiDiffSupport(other, nextDir);
		if (glm::dot(nextDir, supportPoint) < 0) {
			// simplex cannot possibly contain origin
			return false;
		}
		s.Add(supportPoint);

		while (true) {
			glm::vec2 C = s[0];
			glm::vec2 B = s[1];

			// caculate next direction
			glm::vec3 BC = glm::vec3(C.x - B.x, C.y - B.y, 0.0f);
			glm::vec3 BO = glm::vec3(-B.x, -B.y, 0.0f);
			nextDir = glm::cross(glm::cross(BC, BO), BC);

			if (nextDir == glm::vec2(0.0f, 0.0f)) {
				// origin lies on BC (BC is guaranteed longer than BO)
				return true;
			}

			// add point to simplex
			glm::vec2 A = this->MinkowskiDiffSupport(other, nextDir);
			if (glm::dot(nextDir, A) < 0) {
				// simplex cannot possibly contain origin
				return false;
			}
			s.Add(A);

			// calculate where origin is
			glm::vec3 AB = glm::vec3(B.x - A.x, B.y - A.y, 0.0f);
			glm::vec3 AC = glm::vec3(C.x - A.x, C.y - A.y, 0.0f);
			glm::vec3 AO = glm::vec3(-A.x, -A.y, 0.0f);
			glm::vec3 ABNorm = glm::cross(glm::cross(AC, AB), AB);
			glm::vec3 ACNorm = glm::cross(glm::cross(AB, AC), AC);

			if (glm::dot(ABNorm, AO) > 0) {
				// origin is in region 1
				s.Remove(C);
			} else if (glm::dot(ACNorm, AO) > 0) {
				// origin is in region 2
				s.Remove(B);
			} else {
				// origin is in simplex -> collision
				return true;
			}
		}
	}
} // namespace Fizz
