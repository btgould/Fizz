#include "PhysicsEnvironment.hpp"

#include "Collisions/CollisionResolution.hpp"

using namespace Nutella;
using namespace Fizz;

namespace Fizz {
	void PhysicsEnvironment::Update() {
		NT_PROFILE_FUNC();

		UpdateObjects();
		FindCollisions();
		ResolveCollisions();
	}

	void PhysicsEnvironment::Render() {
		for (Ref<PhysicsObject>& object : m_Objects)
			object->Render();
	}

	void PhysicsEnvironment::UpdateObjects() {
		for (Ref<PhysicsObject>& object : m_Objects)
			object->Update();
	}

	void PhysicsEnvironment::FindCollisions() {
		// TODO: implement some sort of broad phase collision detection
		NT_PROFILE_FUNC();

		m_Collisions.clear();

		for (uint32_t i = 0; i < m_Objects.size(); i++) {
			for (uint32_t j = i + 1; j < m_Objects.size(); j++) {
				Collision collision = GJKGetCollision(m_Objects[i], m_Objects[j]);

				if (collision.exists) {
					m_Collisions.push_back(collision);
				}
			}
		}
	}

	void PhysicsEnvironment::ResolveCollisions() {
		NT_PROFILE_FUNC();

		for (Collision collision : m_Collisions)
			ResolveCollision(collision);
	}
} // namespace Fizz
