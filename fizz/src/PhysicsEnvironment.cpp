#include "PhysicsEnvironment.hpp"
#include "Collisions/CollisionResolution.hpp"
#include "Collisions/Quadtree.hpp"
#include "Nutella/Core/Log.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

using namespace Nutella;
using namespace Fizz;

namespace Fizz {
	void PhysicsEnvironment::Update(Nutella::Timestep ts) {
		NT_PROFILE_FUNC();

		FindCollisions();
		ResolveConstraints(ts);
		UpdateObjects(ts);
		// ResolveCollisions();
	}

	void PhysicsEnvironment::Render() {
		for (Ref<PhysicsObject>& object : m_Objects)
			object->Render();
	}

	void PhysicsEnvironment::UpdateObjects(Nutella::Timestep ts) {
		for (Ref<PhysicsObject>& object : m_Objects)
			object->Update(ts);
	}
	void PhysicsEnvironment::ResolveConstraints(Nutella::Timestep ts) {
		glm::vec2 fExt = m_Objects[1]->GetForce();
		glm::vec2 pos = m_Objects[1]->GetPos();
		glm::vec2 vel = m_Objects[1]->GetVelocity();
		float mass = 1.0f / m_Objects[1]->GetInvMass();

		float constraintEval = glm::dot(pos, pos) - 0.25f;
		NT_INFO("cMag: {0}", glm::dot(pos, pos));
		float constraintLinEval = glm::dot(pos, vel);
		// float springConstant = 15.0f;
		// float dampingConstant = 1.0f;
		float feedback = springConstant * constraintEval + dampingConstant * constraintLinEval;

		float lambda =
			(-glm::dot(fExt, pos) - mass * glm::dot(vel, vel) - feedback) / glm::dot(pos, pos);
		m_Objects[1]->ApplyForce(lambda * pos);
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
