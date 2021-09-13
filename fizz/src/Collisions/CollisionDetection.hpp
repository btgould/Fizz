#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

#include "Objects/PhysicsObject.hpp"

namespace Fizz {
	/** Structure describing the collision (if any) between two objects. If there is a collision,
	 *  this structure will contain information about how the objects are colliding. Otherwise, it
	 *  will contain information about the distance between the two objects.
	 */
	struct Collision {
		Nutella::Ref<PhysicsObject> collider, collided;

		/* Whether or not there actually is a collision between collider and collided. */
		bool exists;

		union {
			/** The distance that one object must be moved along the minimum translation vector in
			 * order to completely separate the objects. Set if the collision exists.
			 */
			float penetrationDepth;

			/** The distance between the two closest points the objects. Set if the collision does
			 * not exist.
			 */
			float separationDist;
		};

		union {
			/** The vector such that moving the collided object in this direction will separate the
			 *  objects in the shortest distance possible. Normalized. Set if the collision
			 *  exists.
			 */
			glm::vec2 MTV;

			/** The direction of the shortest vector from any point on collider to any point on
			 * collided. i.e. The vector such that moving the collider object in this direction will
			 * bring the objects into contact in the shortest distance possible. Normalized. Set if
			 * the collision does not exist.
			 */
			glm::vec2 closestDir;
		};
	};

	/** Gets a vector representing the farthest point in the given direction on the convex hull of
	 *  the Minkowski difference p1 - p2 (i.e. the point on p1 - p2 with the largest dot product
	 *  with the given direction).
	 *
	 *  @param p1: The first physics object
	 *  @param p2: The second physics object (subtracted from p1)
	 *  @param dir: The direction to get the support point in
	 *
	 *  @return The farthest point on p1 - p2 in the given direction
	 */
	inline glm::vec2 MinkowskiDiffSupport(const Nutella::Ref<PhysicsObject>& p1,
										  const Nutella::Ref<PhysicsObject>& p2,
										  const glm::vec2& dir) {
		NT_PROFILE_FUNC();

		return p1->GetShape()->Support(dir) - p2->GetShape()->Support(-dir);
	}

	/** Tests whether two physics bodies are colliding.
	 *
	 *	This should be used if the only information desired is whether or not the given bodies
	 *  are colliding. If information about how they are colliding is needed, GJKGetCollision
	 *  should be used instead, as calling both will result in lots of repeated computation.
	 *
	 *  @param p1: The first physics object
	 *  @param p2: The second physics object
	 *  @return true if the objects are colliding, false otherwise
	 */
	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2);

	/** Creates a structure describing the collision (if any) between the given objects. The
	 *  returned structure has a boolean value indicating whether the two objects were actually
	 *  found to be colliding or not.
	 *
	 *  If the objects are colliding, a structure with references to each object in the collision,
	 *  the penetration depth, and the collision normal (AKA minimum translation vector) is
	 *  returned. Additionally, if the edges of two objects are touching, this algorithm will return
	 *  that the objects are colliding with a penetration depth of 0.
	 *
	 *  If the objects are not colliding, a structure with references to each object, the separation
	 *  distance, and the direction of the shortest vector from any point on p1 to any point on p2
	 *  is returned.
	 *
	 *  @param p1: The first physics object
	 *  @param p2: The second physics object
	 *  @param tolerance: The acceptable error in the returned measurement (determines exit
	 *  condition)
	 *
	 *  @return Details about the collision between p1 and p2
	 */
	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance = glm::pow(10, -5));

} // namespace Fizz
