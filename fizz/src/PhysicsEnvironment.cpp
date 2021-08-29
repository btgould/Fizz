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

		uint32_t checks = 0;
		NT_TRACE("Collision sets: {0}", possibleCollisions.size());
		for (uint32_t i = 0; i < possibleCollisions.size(); i++) {
			auto collisionSet = possibleCollisions[i];
			uint32_t size = collisionSet.size();
			NT_TRACE("Collision set {0}: {1} elements", i + 1, size);
			checks += (size - 1) * size / 2;
		}
		NT_TRACE("Total checks: {0}", checks);
		NT_TRACE("Expected checks (w/o quadtree): {0}",
				 (m_Objects.size() - 1) * m_Objects.size() / 2);

		// narrow phase
		m_Collisions.clear();

		for (auto collisionSet : possibleCollisions) {
			for (uint32_t i = 0; i < collisionSet.size(); i++) {
				for (uint32_t j = i + 1; j < collisionSet.size(); j++) {
					Collision collision = GJKGetCollision(collisionSet[i], collisionSet[j]);

					if (collision.exists) {
						m_Collisions.push_back(collision);
					}
				}
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
