#include "CollisionResolution.hpp"

using namespace Nutella;

namespace Fizz {
	void ResolveCollision(const Collision& collision) {
		NT_PROFILE_FUNC();

		glm::vec2 vRel = collision.collided->GetVelocity() - collision.collider->GetVelocity();
		float collisionVel = glm::dot(vRel, collision.MTV);

		// only resolve collision if velocities are not separating the objects
		if (collisionVel < 0) {
			// calculate impulse needed to separate objects
			float restitution = glm::min(collision.collider->GetRestitution(),
										 collision.collided->GetRestitution());

			float invMassA = collision.collider->GetInvMass();
			float invMassB = collision.collided->GetInvMass();
			float impulseMag = -(1 + restitution) * collisionVel;
			impulseMag /= invMassA + invMassB;

			glm::vec2 impulse = impulseMag * collision.MTV;

			// apply impulse (proportionally to mass and in proper direction)
			collision.collider->ApplyImpulse(-invMassA * impulse);
			collision.collided->ApplyImpulse(invMassB * impulse);
		}
	}

	void SinkingCorrection(const Collision& collision, const float correctionWeight,
						   const float penetrationThreshold) {
		// TODO: bug where after using this to resolve collisions objects will coninue inching away
		// on the wrong axis (try breaking when x-comp != 0)

		float correctionDist;
		if ((correctionDist = collision.penetrationDepth - penetrationThreshold) > 0) {
			Ref<PhysicsObject> A = collision.collider;
			Ref<PhysicsObject> B = collision.collided;

			float sysMass = A->GetInvMass() + B->GetInvMass();
			glm::vec2 correction = collision.MTV * correctionDist * correctionWeight / sysMass;

			A->SetPos(A->GetPos() - A->GetInvMass() * correction);
			B->SetPos(B->GetPos() + B->GetInvMass() * correction);
		}
	}

} // namespace Fizz
