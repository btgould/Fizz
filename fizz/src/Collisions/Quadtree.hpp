#pragma once

#include <vector>

#include "Objects/AABB.hpp"
#include "Objects/PhysicsObject.hpp"

namespace Fizz {
	using CollisionList =
		std::vector<std::pair<Nutella::Ref<PhysicsObject>, Nutella::Ref<PhysicsObject>>>;

	/* Linked list-eque data structure used for a broad phase collision detection filter. As the
	   name suggests, each node has four children, which are dynamically allocated / freed as
	   necessary.
	 */
	class Quadtree {
	  public:
		Quadtree(uint32_t level, const AABB& bounds);
		~Quadtree();

		/* Removes all objects from the quadtree and deletes all children. */
		void Clear();

		/* Adds a physics object to the quadtee. Objects are recursively inserted into the smallest
		   child quadtree that can completely contain them.

		   @param object: the physics object to insert
		 */
		void Insert(Nutella::Ref<PhysicsObject>& object);

		/* Returns a pairwise list of all possible collisions in the quadtree. Collisions are
		   filtered by their location in the quatree as while as by AABB intersection checks.

		   @return A list of pairs of physics objects that may be colliding
		*/
		CollisionList GetPossibleCollisions();

		/* Returns a list of possible collisions with the given bounds. Collisions are
		   filtered by their location in the quatree as while as by AABB intersection checks.

		   @param bounds: an AABB to check for collisions with
		   @return A list of physics objects that may be colliding with the bounds
		*/
		std::vector<Nutella::Ref<PhysicsObject>> GetPossibleCollisions(const AABB& bounds);

	  private:
		void GetPossibleCollisions(CollisionList& collisions);
		void GetPossibleChildCollisions(const Nutella::Ref<PhysicsObject>& object,
										CollisionList& collisions);
		void GetPossibleCollisions(const AABB& bounds,
								   std::vector<Nutella::Ref<PhysicsObject>>& collisions);

	  private:
		static const uint32_t MAX_LEVELS;

		uint32_t m_Level;
		Quadtree* m_Nodes;

		std::vector<Nutella::Ref<PhysicsObject>> m_Objects;
		AABB m_Bounds;
	};
} // namespace Fizz
