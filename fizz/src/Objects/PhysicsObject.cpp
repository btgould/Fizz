#include "PhysicsObject.hpp"

#include "Collisions/Simplex.hpp"

namespace Fizz {
	PhysicsObject::PhysicsObject(Nutella::Ref<Shape> shape, Transform transform, float density)
		: m_Shape(shape), m_Transform(transform), m_Velocity(glm::vec2(0.0f)),
		  m_Force(glm::vec2(0.0f)), m_Restitution(0.8f) {
		m_Shape->SetTransform(m_Transform);
		m_MassInfo = shape->GetMassInfo(density); // must set transform first
	}

	void PhysicsObject::Update(Nutella::Timestep ts) {
		NT_PROFILE_FUNC();

		// symplectic Euler integration
		m_Velocity += m_Force * m_MassInfo.invMass * float(ts);
		m_Transform.position += m_Velocity * float(ts);
		m_Shape->SetTransform(m_Transform);

		// zero out force for next run loop
		m_Force = glm::vec2(0.0f);
	}

	void PhysicsObject::Render() { m_Shape->Render(); }

} // namespace Fizz
