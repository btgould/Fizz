#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

#include "Objects/PhysicsObject.hpp"

namespace Fizz {
	struct Collision {
		Nutella::Ref<PhysicsObject> collider, collided;
		bool exists;
		float penetrationDist;
		glm::vec2 MTV;

		// factory constructor to create an object representing no collision between two physics
		// bodies
		static Collision None(Nutella::Ref<PhysicsObject> collider,
							  Nutella::Ref<PhysicsObject> collided) {
			return {collider, collided, false, 0.0f, glm::vec2(0.0f, 0.0f)};
		}
	};

	inline glm::vec2 MinkowskiDiffSupport(const Nutella::Ref<PhysicsObject>& p1,
										  const Nutella::Ref<PhysicsObject>& p2,
										  const glm::vec2& dir) {
		NT_PROFILE_FUNC();

		return p1->GetShape()->Support(dir) - p2->GetShape()->Support(-dir);
	}
	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2);
	glm::vec2 GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  float tolerance = glm::pow(10, -5));
	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance = glm::pow(10, -5));

} // namespace Fizz
