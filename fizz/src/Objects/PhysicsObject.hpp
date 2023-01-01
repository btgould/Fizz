#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

#include "Objects/Shape.hpp"

namespace Fizz {
	/** Represents a physics object. Objects can move, collide, and interact with each other in a
	 *  physics environment.
	 */
	class PhysicsObject {
	  public:
		PhysicsObject(Nutella::Ref<Shape> shape,
		              Transform transform = {glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.2f, 0.2f)},
		              float density = 1.0f);

		/** Updates the position of the physics object according to its velocity and acceleration.
		 *
		 *  @param ts: The timestep to use when updating
		 */
		void Update(Nutella::Timestep ts);

		/** Renders the physics object on the screen */
		void Render();

		/** Immediately changes velocity by the full size of the given impulse vector.
		 *
		 *  @param impulse: The impulse vector to add to the velocity
		 */
		inline void ApplyImpulse(const glm::vec2& impulse) { m_Velocity += impulse; }

		/** Adds an force to the object. When this object is updated, the object's velocity will be
		 *  changed by a portion of the force vector proportional to the inverse mass of the object
		 *  and the timestep used to update it.
		 *
		 *  @param force: The force vector to add to the object
		 */
		inline void ApplyForce(const glm::vec2& force) { m_Force += force; }

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
		inline const glm::vec2& GetForce() const { return m_Force; }

		inline float GetInvMass() const { return m_MassInfo.invMass; }
		inline void SetInvMass(float invMass) { m_MassInfo.invMass = invMass; }
		inline float GetRestitution() const { return m_Restitution; }
		inline void SetRestitution(float restitution) { m_Restitution = restitution; }

		/** Gets the shape that this physics object uses for collision checks and rendering */
		inline Nutella::Ref<Shape> GetShape() const { return m_Shape; }

	  private:
		Nutella::Ref<Shape> m_Shape;
		Transform m_Transform;

		glm::vec2 m_Velocity;
		glm::vec2 m_Force;

		MassInfo m_MassInfo;

		float m_Restitution;
	};
} // namespace Fizz
