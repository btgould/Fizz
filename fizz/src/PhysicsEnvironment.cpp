#include "PhysicsEnvironment.hpp"

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

	void PhysicsEnvironment::UpdateObjects() {}

	void PhysicsEnvironment::FindCollisions() {
		// TODO: implement some sort of broad phase collision detection
		NT_PROFILE_FUNC();

		m_Collisions.clear();

		for (uint32_t i = 0; i < m_Objects.size(); i++) {
			for (uint32_t j = i + 1; j < m_Objects.size(); j++) {
				Collision collision = GJKGetCollision(m_Objects[i], m_Objects[j]);

				if (collision.MTV != glm::vec2(0.0f, 0.0f)) {
					m_Collisions.push_back(collision);
				}
			}
		}
	}

	void PhysicsEnvironment::ResolveCollisions() {}
} // namespace Fizz
