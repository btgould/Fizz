#pragma once

#include "Objects/PhysicsObject.hpp"
#include "CollisionDetection.hpp"

namespace Fizz {
	/* Resolves a collision between two physics bodies by applying an impulse along the collision
	 * normal.

	 @param collision: The collision to resolve
	 */
	void ResolveCollision(const Collision& collision);

	/* Corrects for floating point errors in collision resolution by directly moving colliding\n
	   objects away from each other.

	 @param collision: The collision to correct
	 @param correctionWeight: The fraction of the penetration depth to move objects away
	 @param penetrationThreshold: The penetration distance to allow before correction
	 */
	void SinkingCorrection(const Collision& collision, const float correctionWeight = 0.4f,
						   const float penetrationThreshold = 0.01);
} // namespace Fizz
