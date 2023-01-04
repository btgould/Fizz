#pragma once

#include "Constraint.hpp"
#include "Nutella/Core/Core.hpp"
#include "Objects/PhysicsObject.hpp"
#include "glm/fwd.hpp"

namespace Fizz {
	class PointDistanceConstraint : public PhysicsConstraint {
	  public:
		PointDistanceConstraint(const Nutella::Ref<Fizz::PhysicsObject>& body,
		                        const glm::vec2& point = glm::vec2(0.0f, 0.0f),
		                        float distance = 0.1f, float springConstant = 0.7f,
		                        float dampingConstant = 0.15f);
		virtual void Resolve() const override;

	  public:
		// TEMP: this is just for interactive debugging using IMGUI
		inline float GetSpringConstant() { return m_SpringConstant; }
		inline float GetDampingConstant() { return m_DampingConstant; }
		inline void SetSpringConstant(float springConstant) { m_SpringConstant = springConstant; }
		inline void SetDampingConstant(float dampingConstant) {
			m_DampingConstant = dampingConstant;
		}

	  private:
		virtual float Evaluate() const override;
		virtual float EvaluateDerivative() const override;

		Nutella::Ref<Fizz::PhysicsObject> m_ConstrainedObject;
		glm::vec2 m_Point;
		float m_Distance;

		float m_SpringConstant, m_DampingConstant;
	};
} // namespace Fizz
