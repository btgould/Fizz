#pragma once

#include <glm/glm.hpp>
#include <Nutella.hpp>

namespace Fizz {
	// forward declaration
	struct Collision;

	class PhysicsObject {
	  public:
		virtual void Render() = 0;

		virtual glm::vec2 Support(const glm::vec2& dir) const = 0;
		inline glm::vec2 MinkowskiDiffSupport(Nutella::Ref<PhysicsObject>& other,
											  const glm::vec2& dir) const {
			NT_PROFILE_FUNC();

			return this->Support(dir) - other->Support(-dir);
		}

		virtual glm::vec2 GetPos() const = 0;
		virtual void SetPos(const glm::vec2& pos) = 0;
		virtual float GetRot() const = 0;
		virtual void SetRot(const float rot) = 0;
		virtual glm::vec2 GetScale() const = 0;
		virtual void SetScale(const glm::vec2& scale) = 0;
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

	bool GJKColliding(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2);
	glm::vec2 GJKDistance(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
						  float tolerance = glm::pow(10, -5));
	Collision GJKGetCollision(Nutella::Ref<PhysicsObject>& p1, Nutella::Ref<PhysicsObject>& p2,
							  float tolerance = glm::pow(10, -5));
} // namespace Fizz
