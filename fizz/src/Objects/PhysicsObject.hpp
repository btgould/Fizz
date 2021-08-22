#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

#include "Objects/Shape.hpp"

namespace Fizz {
	class PhysicsObject {
	  public:
		PhysicsObject(Nutella::Ref<Shape> shape,
					  Transform transform = {glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.2f, 0.2f)});

		void Update();
		void Render();

		inline void ApplyImpulse(const glm::vec2& impulse) { m_Velocity += impulse; }

		inline const glm::vec2& GetPos() const { return m_Transform.position; }
		inline void SetPos(const glm::vec2& position) {
			m_Transform.position = position;
			m_Shape->SetTransform(m_Transform);
		}
		inline float GetRot() const { return m_Transform.rotation; }
		inline void SetRot(float rotation) {
			m_Transform.rotation = rotation;
			m_Shape->SetTransform(m_Transform);
		}
		inline const glm::vec2& GetScale() const { return m_Transform.scale; }
		inline void SetScale(const glm::vec2& scale) {
			m_Transform.scale = scale;
			m_Shape->SetTransform(m_Transform);
		}
		inline void SetTransform(const Transform& transform) {
			m_Transform = transform;
			m_Shape->SetTransform(m_Transform);
		}
		inline void SetTransform(const glm::vec2& pos, const float rot, const glm::vec2& scale) {
			m_Transform = {pos, rot, scale};
			m_Shape->SetTransform(m_Transform);
		}

		inline const glm::vec2& GetVelocity() const { return m_Velocity; }

		inline float GetInvMass() const { return m_InvMass; }
		inline void SetInvMass(float invMass) { m_InvMass = invMass; }
		inline float GetRestitution() const { return m_Restitution; }
		inline void SetRestitution(float restitution) { m_Restitution = restitution; }

		inline Nutella::Ref<Shape> GetShape() const { return m_Shape; }

	  private:
		Nutella::Ref<Shape> m_Shape;
		Transform m_Transform;

		glm::vec2 m_Velocity;

		float m_InvMass;
		float m_Restitution;
	};
} // namespace Fizz
