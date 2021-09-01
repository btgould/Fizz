#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

#include "Objects/PhysicsObject.hpp"

namespace Fizz {
	/* Structure describing the collision (if any) between two objects */
	struct Collision {
		Nutella::Ref<PhysicsObject> collider, collided;
		/* Whether or not there actually is a collision between collider and collided */
		bool exists;

		/* The distance that one object must be moved along the minimum translation vector in order
		   to completely separate the objects
		 */
		float penetrationDist;

		/* The vector such that moving the collided object in this direction will separate the
		   objects as quickly as possible. Should be normalized.
		 */
		glm::vec2 MTV;

		/* Factory constructor to create an object representing no collision between two physics \n
		   bodies.

		   @param collider: the first physics object
		   @param collided: the second physics object
		*/
		static Collision None(Nutella::Ref<PhysicsObject> collider,
							  Nutella::Ref<PhysicsObject> collided) {
			return {collider, collided, false, 0.0f, glm::vec2(0.0f, 0.0f)};
		}
	};

	/* Gets a vector representing the farthest point in the given direction on the convex hull of \n
	   the Minkowski difference p1 - p2 (i.e. the point on p1 - p2 with the largest dot product with
	   \n the given direction).

	   @param p1: The first physics object
	   @param p2: The second physics object (subtracted from p1)
	   @param dir: The direction to get the support point in

	   @return The farthest point on p1 - p2 in the given direction
	 */
	inline glm::vec2 MinkowskiDiffSupport(const Nutella::Ref<PhysicsObject>& p1,
										  const Nutella::Ref<PhysicsObject>& p2,
										  const glm::vec2& dir) {
		NT_PROFILE_FUNC();

		return p1->GetShape()->Support(dir) - p2->GetShape()->Support(-dir);
	}

	/* Tests whether two physics bodies are colliding.
	   \n
	   This should be used if the only information desired is whether or not the given bodies are \n
	   colliding. If information about how they are colliding is needed, GJKGetCollision should \n
	   be used instead, as calling both will result in lots of repeated computation.

	   @param p1: The first physics object
	   @param p2: The second physics object

	   @return true if the objects are colliding, false otherwise
	*/
	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2);

	/* Gets the shortest distance between any two points on the given objects. If the objects are
	   colliding, then the 0 vector is returned.

	   @param p1: The first physics object
	   @param p2: The second physics object
	   @param tolerance: The acceptable error in the returned measurement (determines exit
	   condition)

	   @return The shortest vector from any point on p1 to any point on p2
	 */
	glm::vec2 GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  float tolerance = glm::pow(10, -5));

	/* Creates a structure describing the collision (if any) between the given objects.

	   @param p1: The first physics object
	   @param p2: The second physics object
	   @param tolerance: The acceptable error in the returned measurement (determines exit
	   condition)

	   @return Details about the collision between p1 and p2
	 */
	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance = glm::pow(10, -5));

} // namespace Fizz
