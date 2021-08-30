#pragma once

#include <vector>

#include "Objects/AABB.hpp"
#include "Objects/PhysicsObject.hpp"

namespace Fizz {
	using CollisionList =
		std::vector<std::pair<Nutella::Ref<PhysicsObject>, Nutella::Ref<PhysicsObject>>>;

	class Quadtree {
	  public:
		Quadtree(uint32_t level, const AABB& bounds);
		~Quadtree();

		void Clear();
		void Insert(Nutella::Ref<PhysicsObject>& object);
		CollisionList GetPossibleCollisions();
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
