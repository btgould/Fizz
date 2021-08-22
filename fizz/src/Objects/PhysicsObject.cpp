#include "PhysicsObject.hpp"

#include "Collisions/Simplex.hpp"

namespace Fizz {
	PhysicsObject::PhysicsObject(Nutella::Ref<Shape> shape, Transform transform)
		: m_Shape(shape), m_Transform(transform), m_Velocity(glm::vec2(0.0f)), m_InvMass(1.0f),
		  m_Restitution(0.8f) {
		m_Shape->SetTransform(m_Transform);
	}

	void PhysicsObject::Update() {
		NT_PROFILE_FUNC();

		m_Transform.position += m_Velocity;
		m_Shape->SetTransform(m_Transform);
	}

	void PhysicsObject::Render() { m_Shape->Render(); }

	
} // namespace Fizz
