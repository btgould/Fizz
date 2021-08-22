#pragma once

#include "Objects/PhysicsObject.hpp"
#include "Collisions/CollisionDetection.hpp"

namespace Fizz {
	class PhysicsEnvironment {
	  public:
		void Update(Nutella::Timestep ts);
		void Render();

		inline void Add(Nutella::Ref<Fizz::PhysicsObject> object) { m_Objects.push_back(object); }

		inline std::vector<Nutella::Ref<Fizz::PhysicsObject>>& GetObjects() { return m_Objects; }
		inline std::vector<Fizz::Collision>& GetCollisions() { return m_Collisions; };

	  private:
		void UpdateObjects(Nutella::Timestep ts);
		void FindCollisions();
		void ResolveCollisions();

	  private:
		std::vector<Nutella::Ref<Fizz::PhysicsObject>> m_Objects;
		std::vector<Fizz::Collision> m_Collisions;
	};
} // namespace Fizz
