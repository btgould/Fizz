#pragma once

#include "Constraints/Constraint.hpp"
#include "Objects/PhysicsObject.hpp"
#include "Collisions/CollisionDetection.hpp"

namespace Fizz {
	/* Groups multiple physics objects together and manages each of them. Provides a centralized way
	   to update, render, and resolve collisions between all physics objects in the environment.
	 */
	class PhysicsEnvironment {
	  public:
		/* Updates each physics object in the environment.

		   @param ts: The timestep to use when updating
		 */
		void Update(Nutella::Timestep ts);

		/* Renders each physics object in the environment */
		void Render();

		/* Adds a physics object to the environment.

		   @param object: The physics object to add
		*/
		inline void AddObject(Nutella::Ref<Fizz::PhysicsObject> object) {
			m_Objects.push_back(object);
		}

		inline void AddConstraint(Nutella::Ref<Fizz::PhysicsConstraint> constraint) {
			m_Constraints.push_back(constraint);
		}

		/* Gets a list of objects in the environment

		   @return A vector of physics objects in the environment
		 */
		inline std::vector<Nutella::Ref<Fizz::PhysicsObject>>& GetObjects() { return m_Objects; }

		/* Gets a list of collisions found between objects in the environment the last time the
		   environment was updated.

		   @return A vector of collision structures representing each collision in the environment
		 */
		inline std::vector<Fizz::Collision>& GetCollisions() { return m_Collisions; };

	  private:
		void UpdateObjects(Nutella::Timestep ts);
		void ResolveConstraints(Nutella::Timestep ts);

		void FindCollisions();
		void ResolveCollisions();

	  private:
		std::vector<Nutella::Ref<Fizz::PhysicsObject>> m_Objects;
		std::vector<Nutella::Ref<Fizz::PhysicsConstraint>> m_Constraints;
		std::vector<Fizz::Collision> m_Collisions;
	};
} // namespace Fizz
