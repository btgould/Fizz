#include "PointDistanceConstraint.hpp"
#include "Nutella/Core/Core.hpp"

namespace Fizz {
	PointDistanceConstraint::PointDistanceConstraint(const Nutella::Ref<Fizz::PhysicsObject>& body,
	                                                 const glm::vec2& point, float distance,
	                                                 float springConstant, float dampingConstant)
		: m_ConstrainedObject(body), m_Point(point), m_Distance(distance),
		  m_SpringConstant(springConstant), m_DampingConstant(dampingConstant) {}

	float PointDistanceConstraint::Evaluate() const {
		glm::vec2 displacement = m_ConstrainedObject->GetPos() - m_Point;
		return 0.5f * (glm::dot(displacement, displacement) - m_Distance * m_Distance);
	}

	float PointDistanceConstraint::EvaluateDerivative() const {
		glm::vec2 displacement = m_ConstrainedObject->GetPos() - m_Point;
		return glm::dot(displacement, m_ConstrainedObject->GetVelocity());
	}

	void PointDistanceConstraint::Resolve() const {
		glm::vec2 fExt = m_ConstrainedObject->GetForce();
		glm::vec2 pos = m_ConstrainedObject->GetPos();
		glm::vec2 vel = m_ConstrainedObject->GetVelocity();
		float mass = 1.0f / m_ConstrainedObject->GetInvMass();

		float feedback = m_SpringConstant * Evaluate() + m_DampingConstant * EvaluateDerivative();

		float lambda =
			(-glm::dot(fExt, pos) - mass * glm::dot(vel, vel) - feedback) / glm::dot(pos, pos);
		m_ConstrainedObject->ApplyForce(lambda * pos);
	}
} // namespace Fizz
