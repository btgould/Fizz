#pragma once

#include "PhysicsObject.hpp"

namespace Fizz {
	class PhysicsEnvironment {
	  public:
		void Update();
		void Render();

		inline void Add(Nutella::Ref<Fizz::PhysicsObject> object) { m_Objects.push_back(object); }

		inline std::vector<Nutella::Ref<Fizz::PhysicsObject>>& GetObjects() { return m_Objects; }
		inline std::vector<Fizz::Collision>& GetCollisions() { return m_Collisions; };

	  private:
		void UpdateObjects();
		void FindCollisions();
		void ResolveCollisions();

	  private:
		std::vector<Nutella::Ref<Fizz::PhysicsObject>> m_Objects;
		std::vector<Fizz::Collision> m_Collisions;
	};
} // namespace Fizz
