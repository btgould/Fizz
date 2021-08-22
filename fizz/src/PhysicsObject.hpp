#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

#include "Shape.hpp"

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

	struct Collision {
		Nutella::Ref<PhysicsObject> collider, collided;
		bool exists;
		float penetrationDist;
		glm::vec2 MTV;

		// factory constructor to create an object representing no collision between two physics
		// bodies
		static Collision None(Nutella::Ref<PhysicsObject> collider,
							  Nutella::Ref<PhysicsObject> collided) {
			return {collider, collided, false, 0.0f, glm::vec2(0.0f, 0.0f)};
		}
	};

	inline glm::vec2 MinkowskiDiffSupport(const Nutella::Ref<PhysicsObject>& p1,
										  const Nutella::Ref<PhysicsObject>& p2,
										  const glm::vec2& dir) {
		NT_PROFILE_FUNC();

		return p1->GetShape()->Support(dir) - p2->GetShape()->Support(-dir);
	}
	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2);
	glm::vec2 GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  float tolerance = glm::pow(10, -5));
	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance = glm::pow(10, -5));
} // namespace Fizz
