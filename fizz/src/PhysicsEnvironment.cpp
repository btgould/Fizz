#include "PhysicsEnvironment.hpp"

#include "Collisions/CollisionResolution.hpp"
#include "Collisions/Quadtree.hpp"

using namespace Nutella;
using namespace Fizz;

namespace Fizz {
	void PhysicsEnvironment::Update(Nutella::Timestep ts) {
		NT_PROFILE_FUNC();

		UpdateObjects(ts);
		FindCollisions();
		ResolveCollisions();
	}

	void PhysicsEnvironment::Render() {
		for (Ref<PhysicsObject>& object : m_Objects)
			object->Render();
	}

	void PhysicsEnvironment::UpdateObjects(Nutella::Timestep ts) {
		for (Ref<PhysicsObject>& object : m_Objects)
			object->Update(ts);
	}

	void PhysicsEnvironment::FindCollisions() {
		NT_PROFILE_FUNC();

		// broad phase
		CollisionList possibleCollisions;
		{
			NT_PROFILE_SCOPE("Broad Phase Collision Detection");
			// TODO: figure out better way to do these borders
			Quadtree qt(0, AABB(glm::vec2(-8.0f), glm::vec2(8.0f)));
			for (Ref<PhysicsObject> object : m_Objects)
				qt.Insert(object);

			possibleCollisions = qt.GetPossibleCollisions();
		}

		// narrow phase
		m_Collisions.clear();

		for (auto& [A, B] : possibleCollisions) {
			Collision collision = GJKGetCollision(A, B);

			if (collision.exists) {
				m_Collisions.push_back(collision);
			}
		}
	}

	void PhysicsEnvironment::ResolveCollisions() {
		NT_PROFILE_FUNC();

		for (Collision collision : m_Collisions) {
			ResolveCollision(collision);
			SinkingCorrection(collision);
		}
	}
} // namespace Fizz
